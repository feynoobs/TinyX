#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "mbr.h"
#include "gpt.h"
#include "fat32.h"

static void dumpTree(FILE *fr, uint32_t dir, uint32_t fat, uint32_t cur, uint8_t indent)
{
    FAT32ENTRY sect[16];

    while (cur < 0x0FFFFFF8) {
        fseek(fr, dir + (cur - 2) * 512, SEEK_SET);
        fread(sect, sizeof(sect), 1, fr);

        for (int i = 0; i < 16; ++i) {
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
                        if (attr & 0x10) {
                            uint32_t child = (sect[i].clusterHi << 16) | sect[i].clusterLo;
                            if (child != 0 && child < 0x0FFFFFF8) {
                                if (name0 != '.') {
                                    dumpTree(fr, dir, fat, child, indent + 4);
                                }
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
    dumpTree(fr, 0xf70000, 0x104000, 2, 0); 
    fclose(fr);

    return 0;
}
