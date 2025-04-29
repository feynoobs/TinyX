#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "mbr.h"
#include "gpt.h"
#include "fat32.h"


static void dumpTree(uint32_t dir, uint32_t fat, uint32_t cur, uint8_t indent)
{
    FAT32ENTRY sect[16];
    FILE *fr = fopen("/home/feynoobs/Desktop/fat32.img", "r");

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
                        if (child != 0) {
                            uint32_t next = 0;
                            fseek(fr, fat + cur * 4, SEEK_SET);
                            fread(&next, sizeof(next), 1, fr);
                            if (next < 0x0FFFFFF8) {
                                dumpTree(dir, fat, child, indent + 4);
                            }
                            // printf("%04x\n", child);exit(-1);
                            // dumpTree(dir, fat, child, indent + 4);
                            // dumpTree(dir, );
                        }
                    }
                }
            }
        }
    }
    fclose(fr);
}

int main(int argc, char *argv[])
{
    dumpTree(0xf70000, 0x104000, 2, 0); 
}

