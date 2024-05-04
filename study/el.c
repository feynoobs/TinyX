#include <stdio.h>
#include <elf.h>

int main(int argc, char *argv[])
{
    FILE *fp = fopen("a.out", "r");
    Elf64_Ehdr aaa;
    fread(&aaa, sizeof(aaa), 1, fp);
    printf("e_ident:");
    printf("%d\n", sizeof(aaa));
    for (int i = 0; i < sizeof(aaa.e_ident); ++i) {
        printf("%02x", aaa.e_ident[i]);
    }
    putchar('\n');

    printf("e_type:     %04X\n", aaa.e_type);
    printf("e_machine:  %04X\n", aaa.e_machine);
    printf("e_version:  %08X\n", aaa.e_version);
    printf("e_entry:    %08X\n", aaa.e_entry);
    printf("e_phoff:    %08X\n", aaa.e_phoff);
    printf("e_shoff:    %08X\n", aaa.e_shoff);
    printf("e_flags:    %08X\n", aaa.e_flags);
    printf("e_ehsize:   %04X\n", aaa.e_ehsize);
    printf("e_phentsize:%04X\n", aaa.e_phentsize);
    printf("e_phnum:    %04X\n", aaa.e_phnum);
    printf("e_phentsize:%04X\n", aaa.e_shentsize);
    printf("e_shnum:    %04X\n", aaa.e_shnum);
    printf("e_shstrndx: %04X\n", aaa.e_shstrndx);

    Elf64_Shdr shdr;
    fseek(fp, aaa.e_phoff, SEEK_SET);


    return 0;
}
