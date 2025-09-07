#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "mbr.h"
#include "gpt.h"
#include "fat32.h"

typedef struct _FAT32INFO
{
    uint32_t bytesPerSector;
    uint32_t sectorsPerCluster;
    uint32_t bytesPerCluster;
    uint32_t fatStart;    // FAT先頭オフセット(byte)
    uint32_t dataStart;   // データ領域オフセット(byte)
    uint32_t fatSize;     // FAT1個あたりのサイズ(byte)
} FAT32INFO;


static void dumpFile(FILE *fr, FAT32INFO *info, uint32_t startCluster, uint32_t size, uint8_t indent)
{
    uint8_t pool[info->bytesPerCluster];
    uint32_t cur = startCluster;
    uint32_t remain = size;

    while (cur < 0x0FFFFFF8 && remain > 0) {
        fseek(fr, info->dataStart + (cur - 2) * info->bytesPerCluster, SEEK_SET);
        fread(buf, info->bytesPerCluster, 1, fr);

    }

}

static void dumpTree(FILE *fr, FAT32INFO *info, uint32_t startCluster, uint8_t indent)
{
    FAT32ENTRY sect[info->bytesPerCluster / sizeof(FAT32ENTRY)];

    while (startCluster < 0x0FFFFFF8) {
        fseek(fr, info->dataStart + (startCluster - 2) * info->bytesPerCluster, SEEK_SET);
        fread(sect, sizeof(sect), 1, fr);

        int entries = info->bytesPerCluster / sizeof(FAT32ENTRY);
        for (int i = 0; i < entries; ++i) {
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
                                dumpTree(fr, info, child, indent + 4);
                            }
                            // ファイル
                            else if (attr & 0x20) {
                                dumpFile(fr, info, child, sect[i].fileSize, indent + 4);
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
    for (int i = 0; i < g.partitionArrayCnt; ++i) {
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

    FAT32INFO info;
    info.bytesPerSector   = f.bytesPerSector;
    info.sectorsPerCluster= f.sectorsPerCluster;
    info.bytesPerCluster  = f.bytesPerSector * f.sectorsPerCluster;
    info.fatStart         = e[0].firstLBA * 512 + f.reserveSectors * f.bytesPerSector;
    info.fatSize          = f.fatSize32 * f.bytesPerSector;
    info.dataStart        = info.fatStart + f.numFats * info.fatSize;
    dumpTree(fr, &f, 0x104000, 2, 0); 
    fclose(fr);

    return 0;
}
