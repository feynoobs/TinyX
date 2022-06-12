#include <efi.h>
#include <efilib.h>

VOID *
Malloc(EFI_BOOT_SERVICES *BS, UINT64 length)
{
    VOID *memory;
    EFI_STATUS status;

    status = uefi_call_wrapper(BS->AllocatePool, 2, length, (VOID **)&memory);
    if (status != EFI_SUCCESS) {
        memory = NULL;
    }

    return memory;
}

VOID
Free(EFI_BOOT_SERVICES *BS, VOID *memory)
{
    uefi_call_wrapper(BS->FreePool, 1, memory);
}

EFI_STATUS
efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *table)
{
    InitializeLib(image, table);

    EFI_BOOT_SERVICES *BS = table->BootServices;
    EFI_RUNTIME_SERVICES *RS = table->RuntimeServices;
    EFI_STATUS status;
    EFI_HANDLE *handles;
    EFI_LOADED_IMAGE *img;
    EFI_DEVICE_PATH *imgPath;
    EFI_BLOCK_IO *blkio;
    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *text;
    CHAR16 *disp;
    UINTN size;
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
        status = uefi_call_wrapper(BS->HandleProtocol, 3, img->DeviceHandle, &DevicePathGUID, (VOID **)&imgPath);
        if (status != EFI_SUCCESS) {
            goNext = FALSE;
        }
    }

    if (goNext == TRUE) {
        disp = (CHAR16 *)uefi_call_wrapper(text->ConvertDevicePathToText, 3, imgPath, TRUE, TRUE);
        Print(L"Load Device:%s\n", disp);
        size = sizeof(EFI_HANDLE);
        handles = (EFI_HANDLE *)Malloc(BS, size);
        if (handles == NULL) {
            goNext = FALSE;
        }
    }

    if (goNext == TRUE) {
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

    for (;;) ;
    return EFI_SUCCESS;
}
