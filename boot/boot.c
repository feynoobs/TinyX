#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *table)
{
    InitializeLib(image, table);

    EFI_BOOT_SERVICES *BS = table->BootServices;
    EFI_RUNTIME_SERVICES *RS = table->RuntimeServices;
    EFI_STATUS status;
    EFI_HANDLE handle;
    EFI_LOADED_IMAGE *img;
    EFI_DEVICE_PATH *imgpath;
    EFI_BLOCK_IO *blkio;

    EFI_GUID LoadedImageGUID = LOADED_IMAGE_PROTOCOL;
    EFI_GUID DevicePathGUID = DEVICE_PATH_PROTOCOL;
    EFI_GUID BlockIoProtocolGUID = BLOCK_IO_PROTOCOL;

    Print(L"TinyX Boot Loader\n");
    status = uefi_call_wrapper(BS->HandleProtocol, 3, image, &LoadedImageGUID, (VOID **)&img);
    if (status == EFI_SUCCESS) {
        status = uefi_call_wrapper(img->DeviceHandle, 2, &DevicePathGUID, (VOID **)&imgpath);
        if (status == EFI_SUCCESS) {
            status = uefi_call_wrapper(BS->LocateHandle, 3, ByProtocol, &BlockIoProtocolGUID, (VOID **)&blkio);
        }
    }

    return EFI_SUCCESS;
}
