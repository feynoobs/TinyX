#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "mbr.h"
#include "gpt.h"
#include "fat32.h"

static void dumpFile(FILE *fr, uint32_t dir, uint32_t fat, uint32_t cur, uint8_t indent, uint32_t size)
{
    uint8_t pool[32*16*16];

    fseek(fr, dir + (cur - 2) * 512 * 16, SEEK_SET);
    fread(pool, sizeof(pool), 1, fr);

    printf("size: %u\n", size);
    for (int i = 0; i < indent; ++i) {
        putchar(' ');
    }
    for (int i = 0; i < 16; ++i) {
        putchar(pool[i]);
    }
    putchar('\n');
}

static void dumpTree(FILE *fr, uint32_t dir, uint32_t fat, uint32_t cur, uint8_t indent)
{
    FAT32ENTRY sect[16*16];

    while (cur < 0x0FFFFFF8) {
        fseek(fr, dir + (cur - 2) * 512 * 16, SEEK_SET);
        fread(sect, sizeof(sect), 1, fr);

        for (int i = 0; i < 16*16; ++i) {
            uint8_t name0 = sect[i].name[0];
            uint8_t attr = sect[i].attr;
            if (name0 != 0x00) {
                if (name0 != 0xe5) {
                    if ((attr & 0x0f) != 0x0f) {
                        for (int j = 0; j < indent; ++j) {
                            putchar(' ');
                        }
                        for (int j = 0; j < 11; ++j) {
                            putchar(sect[i].name[j]);
                        }
                        putchar('\n');
                        if (name0 != '.') {
                            // ディレクトリ
                            uint32_t child = (sect[i].clusterHi << 16) | sect[i].clusterLo;
                            if (attr & 0x10) {
                                dumpTree(fr, dir, fat, child, indent + 4);
                            }
                            // ファイル
                            else if (attr & 0x20) {
                                dumpFile(fr, dir, fat, child, indent + 4, sect[i].fileSize);
                            }
                        }
                    }
                }
            }
        }
        fseek(fr, fat + cur * 4, SEEK_SET);
        fread(&cur, sizeof(cur), 1, fr);
    }
}

int main(int argc, char *argv[])
{
    MBR m;
    GPT g;
    FILE *fr = fopen("/home/feynoobs/Desktop/fat32.img", "rb");
    // FILE *fr = fopen("/dev/sda", "rb");

    fread(&m, sizeof(MBR), 1, fr);
    fread(&g, sizeof(GPT), 1, fr);

    GPTENTRY e[128];
    int i;
    for (i = 0; i < g.partitionArrayCnt; ++i) {
        fread(&e[i], sizeof(GPTENTRY), 1, fr);
        if (e[i].firstLBA == 0) {
            break;
        }
    }
    // printf("%d\n", i);
    fseek(fr, e[0].firstLBA * 512, SEEK_SET);
    // exit(-1);
    FAT32BPB f;
    fread(&f, sizeof(FAT32BPB), 1, fr);
    
//     printf("bytesPerSector: %u\n", f.bytesPerSector);
//     printf("sectorsPerCluster: %u\n", f.sectorsPerCluster);
// exit(-1);

    dumpTree(fr, 0xf70000, 0x104000, 2, 0); 
    fclose(fr);

    return 0;
}
