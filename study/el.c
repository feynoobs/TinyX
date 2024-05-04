#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

int main(int argc, char *argv[])
{
    FILE *fp = fopen("a.out", "r");
    Elf64_Ehdr aaa;
    fread(&aaa, sizeof(aaa), 1, fp);
    printf("e_ident:");
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
    printf("e_shentsize:%04X\n", aaa.e_shentsize);
    printf("e_shnum:    %04X\n", aaa.e_shnum);
    printf("e_shstrndx: %04X\n", aaa.e_shstrndx);
    putchar('\n');
    unsigned char *buf = malloc(8192);
    fseek(fp, 0x36ec-0x40, SEEK_SET);
    fread(buf, 8192, 1, fp);
    for (int i = 0; i < 0x11A; ++i) {
        printf("%d:%s\n", i, &buf[i]);
    }
    // exit(0);
    // fclose(fp);

    // fp = fopen("a.out", "r");
    // Elf64_Shdr nm;
    // printf("%X\n", aaa.e_shoff + aaa.e_shentsize * (aaa.e_shstrndx - 2));
    // printf("aaa\n");
    // fseek(fp, aaa.e_shoff + aaa.e_shentsize * (aaa.e_shstrndx - 2), SEEK_SET);
    // fread(&nm, sizeof(nm), 1,fp);
//     for (int i = 0; i < 256; ++i) {
//         printf("%x ", (char*)&nm + i);exit(1);
//         // printf("%d", nm.sh_type);
//     }

// exit(9);

    Elf64_Shdr shdr;
    fseek(fp, aaa.e_shoff, SEEK_SET);
    for (int i = 0; i < aaa.e_shnum; ++i) {
        fread(&shdr, sizeof(shdr), 1, fp);
        printf("Loop:%d\n", i);
        // printf("sh_name:%s\n", (char*)(aaa.e_shoff + aaa.e_phentsize * aaa.e_shstrndx + shdr.sh_name)) ;
        // printf("sh_name:%s\n", (char*)(aaa.e_shoff - shdr.sh_name - aaa.e_shstrndx)) ;
        // printf("sh_name:%X\n", (char*)(aaa.e_shoff - shdr.sh_name - aaa.e_shstrndx)) ;
        // printf("sh_name:%s\n", (char*)(0x36a3 + shdr.sh_name));
        printf("sh_name:%d\n", shdr.sh_name);
        printf("sh_name:%s\n", &buf[shdr.sh_name]);
        // printf("sh_name:%X\n", 0x36a3+shdr.sh_name);
        // printf("sh_name:%X\n", aaa.e_shstrndx);
        if (aaa.e_shstrndx == i) {
            puts("hage");
        }

        printf("sh_flags:%X\n", shdr.sh_flags);
        printf("sh_addr:%X\n", shdr.sh_addr);
        printf("sh_offset:%X\n", shdr.sh_offset);
        printf("sh_size:%X\n", shdr.sh_size);

        putchar('\n');
    }

//     fseek(fp, aaa.e_shstrndx, SEEK_SET);
//     for (int i = 0; i < 60; ++i) {
//         putchar(fgetc(fp));
//     }
// exit(0);
/*
    Elf64_Phdr phdr;
    printf("***%d\n", sizeof(phdr));
    fseek(fp, aaa.e_phoff, SEEK_SET);
    for (int i = 0; i < aaa.e_phnum; ++i) {
        fread(&phdr, sizeof(phdr), 1, fp);
        printf("Loop:%d\n", i);
        printf("p_type:%llX\n", phdr.p_type);
        printf("p_flags:%llX\n", phdr.p_flags);
        printf("p_offset:%llX\n", phdr.p_offset);
        printf("p_vaddr:%llX\n", phdr.p_vaddr);
        printf("p_paddr:%llX\n", phdr.p_paddr);
        printf("p_paddr:%llX\n", phdr.p_paddr);
        printf("p_memsz:%llX\n", phdr.p_memsz);
        printf("p_align:%llX\n", phdr.p_align);
        putchar('\n');
    }
*/
    // fclose(fp);

    return 0;
}
