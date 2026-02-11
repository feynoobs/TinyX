ARCH            = `arch`

OBJS            = boot.o
TARGET          = BOOTX64.EFI

EFIINC          = /usr/include/efi
EFIINCS         = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
LOCALINC        = -I../include
LIB             = /usr/lib
EFILIB          = /usr/lib
EFI_CRT_OBJS    = $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS         = $(EFILIB)/elf_$(ARCH)_efi.lds

CFLAGS          = $(LOCALINC) $(EFIINCS) -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -Wall
ifeq ($(ARCH),x86_64)
CFLAGS          += -DEFI_FUNCTION_WRAPPER
endif

LDFLAGS         = -nostdlib -znocombreloc -T $(EFI_LDS) -shared -Bsymbolic -L $(EFILIB) -L $(LIB) $(EFI_CRT_OBJS)

all: $(TARGET)

BOOTX64.so: $(OBJS)
	ld $(LDFLAGS) $(OBJS) -o $@ -lefi -lgnuefi

%.EFI: %.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .rodata -j .dynsym  -j .rel -j .rela -j .reloc --target=efi-app-$(ARCH) $^ $@

clean:
	rm -f *.o *.so *.EFI
