/**
 * @file boot.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-01-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <efi.h>
#include <efilib.h>

#include "file/fat32.h"

/**
 * @brief ヒープからメモリを取得します
 *
 * @param BS ブートサービス
 * @param length 確保するメモリのサイズ
 * @return 確保したメモリのポインタ
 */
static VOID *
Malloc(const EFI_BOOT_SERVICES *BS, const UINT64 length)
{
    VOID *memory;
    EFI_STATUS status;

    status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, length, (VOID **)&memory);
    if (status != EFI_SUCCESS) {
        memory = NULL;
    }

    return memory;
}

/**
 * @brief ヒープのメモリを開放します
 *
 * @param BS ブートサービス
 * @param memory Mallocで確保した開放するメモリ
 */
static VOID
Free(const EFI_BOOT_SERVICES *BS, const VOID *memory)
{
    uefi_call_wrapper(BS->FreePool, 1, memory);
}

/**
 * @brief メモリをコピーする
 *
 * @param[out] dst 入力先
 * @param[in] src 入力元
 * @param[in] length コピーするメモリの長さ
 */
static VOID
Memcpy(VOID *dst, const VOID *src, UINTN length)
{
    UINT8 *p1 = (UINT8 *)dst;
    const UINT8 *p2 = (UINT8 *)src;

    while (length > 0) {
        --length;
        *p1 = *p2;
        --p1;
        --p2;
    }
}

/**
 * @brief 2つのメモリを比較する
 *
 * @param[in] s1 比較対象1
 * @param[in] s2 比較対象2
 * @param[in] length 長さ
 * @retval 0 同一内容
 * @retval 0以外 異なる内容
 */
static INTN
Memcmp(const VOID *s1, const VOID *s2, UINTN length)
{
    const UINT8 *p1 = (UINT8 *)s1;
    const UINT8 *p2 = (UINT8 *)s2;
    INTN ret;

    while (length > 0) {
        --length;
        ret = *p1 - *p2;
        if (ret != 0) {
            break;
        }
        --p1;
        --p2;
    }

    return ret;
}

/**
 * @brief 2つのデバイスノードが一致しているか
 *
 * @param imgPath 1つ目のデバイスノード
 * @param devPath 2つ目のデバイスノード
 * @retval TRUE ノードが一致
 * @retval FALSE ノードが不一致
 */
static BOOLEAN
IsNodeMatch(const EFI_DEVICE_PATH *imgPath, const EFI_DEVICE_PATH *devPath)
{
    BOOLEAN match = FALSE;
    UINTN length = DevicePathNodeLength(imgPath);

    if (imgPath->Type == devPath->Type) {
        if (imgPath->SubType == devPath->SubType) {
            if (length == DevicePathNodeLength(devPath)) {
                if (Memcmp(imgPath, devPath, length) == 0) {
                    match = TRUE;
                }
            }
        }
    }

    return match;
}

/**
 * @brief
 *
 * @param[in] BS ブートサービス
 * @param[in] block ブロックIO
 * @param[in] fat32Data fat32の構造体
 * @param[in] cluster ディレクトリのある先頭クラスタ
 * @param[out] files 読んだファイル数
 * @param[out] datas 各ファイルのFAT情報
 */
static VOID
ReadDirSub(const EFI_BOOT_SERVICES *BS, const EFI_BLOCK_IO *block, const fat32 *fat32Data, const UINTN cluster, UINTN *files, fat32entry *datas)
{
    EFI_STATUS status;
    const UINTN entries = fat32Data->bytesPerSector * fat32Data->sectorsPerCluster / sizeof(fat32entry);
    const fat32entry fat32EntryData[entries];
    const UINTN rootLBA = fat32Data->fatSize32 * fat32Data->numFats + fat32Data->reserveSectors;
    const UINTN fatLBA = fat32Data->reserveSectors;
    UINTN cursor = 0;

    for (UINTN tmpCluster = cluster; tmpCluster < 0x0FFFFFF8;) {
        const UINTN relaticeLBA = (tmpCluster - fat32Data->rootEntryClusPos) * fat32Data->sectorsPerCluster;
        const UINTN relativeFatLBA = tmpCluster / 128;
        const UINT32 fatEntry[128];
        status = uefi_call_wrapper(block->ReadBlocks, 5, block, block->Media->MediaId, rootLBA + relaticeLBA, sizeof(fat32EntryData), (VOID *)fat32EntryData);
        if (status == EFI_SUCCESS) {
            for (UINTN i = 0; i < entries; ++i) {
                if (fat32EntryData[i].name[0] != 0x00) {
                    if (fat32EntryData[i].name[0] != 0xE5) {
                        if (fat32EntryData[i].name[0] != 0x05) {
                            if (datas != NULL) {
                                Memcpy(&datas[cursor], &fat32EntryData[i], sizeof(fat32entry));
                            }
                            ++cursor;
                        }
                    }
                }
            }
            status = uefi_call_wrapper(block->ReadBlocks, 5, block, block->Media->MediaId, fatLBA + relativeFatLBA, sizeof(fatEntry), (VOID *)fatEntry);
            if (status == EFI_SUCCESS) {
                Print(L"current -> %X\n", tmpCluster);
                tmpCluster = fatEntry[tmpCluster % 128];
                Print(L"next -> %X\n", tmpCluster);
            }
        }
    }
    *files = cursor;

    return datas;
}

/**
 * @brief 指定されたクラスタのファイル一覧を取得する
 *
 * @param[in] BS ブートサービス
 * @param[in] block ブロックIO
 * @param[in] fat32Data fat32の構造体
 * @param[in] cluster ディレクトリのある先頭クラスタ
 * @param[out] files 読んだファイル数
 * @return ファイルリストのポインタ
 */
static fat32entry*
ReadDir(const EFI_BOOT_SERVICES *BS, const EFI_BLOCK_IO *block, const fat32 *fat32Data, const UINTN cluster, UINTN *files)
{
    fat32entry* datas = NULL;
    *files = 0;
    ReadDirSub(BS, block, fat32Data, cluster, files, NULL);
    if (*files != 0) {
        datas = Malloc(BS, sizeof(fat32entry) * (*files));
        ReadDirSub(BS, block, fat32Data, cluster, files, datas);
    }

    return datas;
}

static VOID
LoadKernel(const EFI_BOOT_SERVICES *BS, const EFI_BLOCK_IO *block)
{
    EFI_STATUS status;

    Print(L"block size:%d\n", block->Media->BlockSize);

    fat32 fat32Data;
    status = uefi_call_wrapper(block->ReadBlocks, 5, block, block->Media->MediaId, 0, sizeof(fat32), (VOID *)&fat32Data);
    if (status == EFI_SUCCESS) {
        Print(L"rootEntryClusPos -> %u\n", fat32Data.rootEntryClusPos);
        UINTN files;
        fat32entry *fileEntry = ReadDir(BS, block, &fat32Data, fat32Data.rootEntryClusPos, &files);
        Print(L"files2 -> %X\n", files);
        for (INT16 i = 0; i < files; ++i) {
            for (CHAR8 j = 0; j < sizeof(fileEntry[i].name); ++j) {
                Print(L"%c", fileEntry[i].name[j]);
            }
            Print(L"\n");
        }
        Print(L"Fat32 Dump...\n");
        /*
        Print(L"bytesPerSector -> %u\n", fat32Data.bytesPerSector);
        Print(L"sectorsPerCluster -> %u\n", fat32Data->sectorsPerCluster);
        Print(L"sectorPerTruck -> %u\n", fat32Data->sectorPerTruck);
        Print(L"reserveSectors -> %u\n", fat32Data->reserveSectors);
        Print(L"numFats -> %u\n", fat32Data->numFats);
        Print(L"totalSector32 -> %u\n", fat32Data->totalSector32);
        Print(L"fatSize32 -> %u\n", fat32Data->fatSize32);
        Print(L"fsinfoEntrySector -> %u\n", fat32Data->fsinfoEntrySector);
        Print(L"backupBootSector -> %u\n", fat32Data->backupBootSector);
        Print(L"signature  -> %04X\n", fat32Data->signature);

        UINTN rootLBA = fat32Data->fatSize32 * fat32Data->numFats + fat32Data->reserveSectors;
        UINTN fatLBA = fat32Data->reserveSectors;
        fat32entry *fat32EntryData = (fat32entry *)Malloc(BS, sizeof(fat32entry) * 16);
        status = uefi_call_wrapper(block->ReadBlocks, 5, block, block->Media->MediaId, rootLBA, sizeof(fat32entry) * 16, (VOID *)fat32EntryData);
        if (status == EFI_SUCCESS) {
            Print(L"Fat32 Entry Dump...\n");
            for (INT8 i = 0; i < 16; ++i) {
                if (fat32EntryData[i].name[0] != 0x00) {
                    if (fat32EntryData[i].name[0] != 0xE5) {
                        UINTN cluster = fat32EntryData[i].clusterHi << 16 | fat32EntryData[i].clusterLo;
                        Print(L"name ->");
                        for (INT8 j = 0; j < sizeof(fat32EntryData[i].name); ++j) {
                            Print(L"%c", fat32EntryData[i].name[j]);
                        }
                        Print(L"\n");
                        Print(L"cluster -> %d\n", cluster);
                        Print(L"fileSize -> %d\n", fat32EntryData[i].fileSize);
                        if ((fat32EntryData[i].name[0] == 'K') &&
                            (fat32EntryData[i].name[1] == 'E') &&
                            (fat32EntryData[i].name[2] == 'R') &&
                            (fat32EntryData[i].name[3] == 'N') &&
                            (fat32EntryData[i].name[4] == 'E') &&
                            (fat32EntryData[i].name[5] == 'L')) {
                            UINTN allocate = 0;
                            UINTN size = fat32EntryData[i].fileSize;
                            const UINT16 ONE = 512 * fat32Data->sectorsPerCluster;

                            for (INT8 k = 0; cluster < 0x0FFFFFF8; ++k) {
                                UINT16 reader = size;
                                if (size > ONE) {
                                    size -= ONE;
                                    reader = ONE;
                                    allocate += ONE;
                                }
                                else {
                                    allocate += size;
                                }
                                UINT32 fat[128];
                                UINT8 clusterData[ONE];

                                uefi_call_wrapper(block->ReadBlocks, 5, block, block->Media->MediaId, fatLBA + cluster / 128, 512, (VOID *)fat);
                                status = uefi_call_wrapper(block->ReadBlocks, 5, block, block->Media->MediaId, rootLBA + (cluster - fat32Data->rootEntryClusPos) * fat32Data->sectorsPerCluster, ONE, (VOID *)clusterData);
                                for (INT16 m = 0; m < reader; ++m) {
                                    Print(L"%c", clusterData[m]);
                                }
                                cluster = fat[cluster];
                            }
                        }
                    }
                }
            }
        }
        */
    }
}

/**
 * @brief UEFI エントリー
 *
 * @param[in] image このプログラムのハンドル
 * @param[in] table システムテーブル
 * @return EFI_STATUS
 */
EFI_STATUS
efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *table)
{
    InitializeLib(image, table);

    EFI_BOOT_SERVICES *BS = table->BootServices;
    EFI_STATUS status;
    EFI_HANDLE *handles;
    EFI_LOADED_IMAGE *img;
    EFI_DEVICE_PATH *keepImgPath, *imgPath, *devPath;
    EFI_BLOCK_IO *block;
    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *text;
    CHAR16 *disp;
    UINTN size, nHanles;
    BOOLEAN goNext;

    EFI_GUID LoadedImageGUID = LOADED_IMAGE_PROTOCOL;
    EFI_GUID DevicePathGUID = DEVICE_PATH_PROTOCOL;
    EFI_GUID BlockIoProtocolGUID = BLOCK_IO_PROTOCOL;
    EFI_GUID DevicePathToTextGUID = EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;

    Print(L"TinyX Boot Loader\n");
    goNext = TRUE;
    status = uefi_call_wrapper(BS->HandleProtocol, 3, image, &LoadedImageGUID, (VOID **)&img);
    if (status != EFI_SUCCESS) {
        goNext = FALSE;
    }

    if (goNext == TRUE) {
        status = uefi_call_wrapper(BS->LocateProtocol, 3, &DevicePathToTextGUID, NULL, (VOID **)&text);
        if (status != EFI_SUCCESS) {
            goNext = FALSE;
        }
    }

    if (goNext == TRUE) {
        disp = (CHAR16 *)uefi_call_wrapper(text->ConvertDevicePathToText, 3, img->FilePath, TRUE, TRUE);
        Print(L"Loader path:%s\n", disp);
        Free(BS, disp);
        status = uefi_call_wrapper(BS->HandleProtocol, 3, img->DeviceHandle, &DevicePathGUID, (VOID **)&imgPath);
        keepImgPath = imgPath;
        if (status != EFI_SUCCESS) {
            goNext = FALSE;
        }
    }

    if (goNext == TRUE) {
        disp = (CHAR16 *)uefi_call_wrapper(text->ConvertDevicePathToText, 3, imgPath, TRUE, TRUE);
        Print(L"Load Device:%s\n", disp);
        Free(BS, disp);
        size = sizeof(EFI_HANDLE);
        handles = (EFI_HANDLE *)Malloc(BS, size);
        if (handles != NULL) {
            status = uefi_call_wrapper(BS->LocateHandle, 5, ByProtocol, &BlockIoProtocolGUID, NULL, &size, handles);
            switch (status) {
                case EFI_SUCCESS:
                    break;
                case EFI_BUFFER_TOO_SMALL:
                    Free(BS, (VOID *)handles);
                    handles = Malloc(BS, size);
                    if (handles == NULL) {
                        goNext = FALSE;
                    }
                    else {
                        status = uefi_call_wrapper(BS->LocateHandle, 5, ByProtocol, &BlockIoProtocolGUID, NULL, &size, handles);
                        if (status != EFI_SUCCESS) {
                            goNext = FALSE;
                        }
                    }
                    break;
                default:
                    goNext = FALSE;
            }
        }
        else {
            goNext = FALSE;
        }
    }

    if (goNext == TRUE) {
        nHanles = size / sizeof(EFI_HANDLE);
        Print(L"Probing %u block devices...\n", nHanles);
        for (UINTN i = 0; i < nHanles; ++i) {
            status = uefi_call_wrapper(BS->HandleProtocol, 3, handles[i], &DevicePathGUID, (VOID **)&devPath);
            if (status == EFI_SUCCESS) {
                disp = (CHAR16 *)uefi_call_wrapper(text->ConvertDevicePathToText, 3, devPath, TRUE, TRUE);
                Print(L"Probing %u -> %s\n", i + 1, disp);
                Free(BS, disp);
                status = uefi_call_wrapper(BS->HandleProtocol, 3, handles[i], &BlockIoProtocolGUID, (VOID **)&block);
                if (status == EFI_SUCCESS) {
                    if (block->Media->LogicalPartition == TRUE) {
                        BOOLEAN match = FALSE;
                        imgPath = keepImgPath;
                        while ((IsDevicePathEnd(imgPath) == FALSE) && (IsDevicePathEnd(devPath) == FALSE)) {
                            if (DevicePathType(imgPath) == MEDIA_DEVICE_PATH) {
                                if (DevicePathType(devPath) == MEDIA_DEVICE_PATH) {
                                    match = TRUE;
                                    break;
                                }
                            }
                            if (IsNodeMatch(imgPath, devPath) == FALSE) {
                                break;
                            }

                            imgPath = NextDevicePathNode(imgPath);
                            devPath = NextDevicePathNode(devPath);
                        }
                        if (match == TRUE) {
                            LoadKernel(BS, block);
                        }
                    }
                }
            }
        }
    }

    Print(L"Fail to Ban\n");
    for (;;) ;
    return EFI_SUCCESS;
}
