#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mbr.h"
#include "gpt.h"
#include "fat32.h"

void dumpTreeV2(uint64_t origin)
{

}

void hexDump(uint64_t target)
{
    uint16_t buffer[1024] = {0};
    FILE *fr = fopen("/home/feynoobs/Desktop/fat32.img", "r");
    fseek(fr, target, SEEK_SET);

    for (uint16_t i = 0; i < sizeof(buffer); ++i) {
        if ((i % 16) == 0) {
            putchar('\n');
        }
        uint8_t c;
        fread(&c, 1, 1, fr);
        printf("%02X ", c);
    }

    fclose(fr);
}

void dumpTree(uint64_t origin, FILE *fr, uint32_t *fat, int indent, int pos)
{
    FAT32ENTRY sect[16];
    int next = pos;
    for (int cnt = 0;; ++cnt) {
        if (cnt % 16 == 0) {
            if (next < 0x0FFFFFF8) {
                fseek(fr, origin + next * 512, SEEK_SET);
                fread(sect, sizeof(sect), 1, fr);
                next = fat[next];
            }
            else {
                break;
            }
        }
        if (sect[cnt % 16].attr != 0) {
            for (int i = 0; i < indent; ++i) {
                putchar(' ');
            }
            for (int i = 0; i < 11; ++i) {
                putchar(sect[cnt % 16].name[i]);
            }
            putchar('\n');
            if (sect[cnt % 16].attr & 0x20) {
                for (int i = 0; i < indent; ++i) {
                    putchar(' ');
                }
                int st = sect[cnt % 16].clusterHi << 16 | sect[cnt % 16].clusterLo;
                int size = sect[cnt % 16].fileSize;
                for (;;) {
                    uint8_t buf[512];
                    fseek(fr, origin + (st-2) * 512, SEEK_SET);
                    fread(buf, sizeof(buf), 1, fr);
                    for (int i = 0; i < 512; ++i) {
                        putchar(buf[i]);
                    }
                    st = fat[st];
                    if (st >= 0x0FFFFFF8) {
                        break;
                    }
                }
            }
            if (sect[cnt % 16].attr & 0x10) {
                if (sect[cnt % 16].name[0] != 0x2E) {
                    int cur = sect[cnt % 16].clusterHi << 16 | sect[cnt % 16].clusterLo;
                    dumpTree(origin, fr, fat, indent + 4, cur - 2);
                }
            }
        }
    }
}

int main(void)
{
    MBR m;
    GPT g;

    FILE *fr = fopen("/home/feynoobs/Desktop/fat32.img", "r");
    fread(&m, sizeof(MBR), 1, fr);
    fread(&g, sizeof(GPT), 1, fr);

    printf("mbr = %ld byte\n", sizeof(MBR));
    printf("gpt = %ld byte\n", sizeof(GPT));
    printf("gpt entry = %ld byte\n", sizeof(GPTENTRY));
    putchar('\n');

    puts("dump MBR...");
    printf("partition1[status]: %02X\n", m.partition1.status);
    printf("partition1[chsStartHead]: %02X\n", m.partition1.chsStartHead);
    printf("partition1[chsStartSector]: %02X\n", m.partition1.chsStartSector);
    printf("partition1[chsStartCylinder]: %02X\n", m.partition1.chsStartCylinder);
    printf("partition1[partitionType]: %02X\n", m.partition1.partitionType);
    printf("partition1[chsEndHead]: %02X\n", m.partition1.chsEndHead);
    printf("partition1[chsEndSector]: %02X\n", m.partition1.chsEndSector);
    printf("partition1[chsEndCylinder]: %02X\n", m.partition1.chsEndCylinder);
    printf("partition1[startLBASector]: %u\n", m.partition1.startLBASector);
    printf("partition1[sectors]: %u\n", m.partition1.sectors);
    printf("partition1[total]: %lf Giga Byte\n", ((((m.partition1.sectors * 512.0) / 1024.0) / 1024.0) / 1024.0));
    putchar('\n');
    printf("partition2[status]: %02X\n", m.partition2.status);
    printf("partition2[chsStartHead]: %02X\n", m.partition2.chsStartHead);
    printf("partition2[chsStartSector]: %02X\n", m.partition2.chsStartSector);
    printf("partition2[chsStartCylinder]: %02X\n", m.partition2.chsStartCylinder);
    printf("partition2[partitionType]: %02X\n", m.partition2.partitionType);
    printf("partition2[chsEndHead]: %02X\n", m.partition2.chsEndHead);
    printf("partition2[chsEndSector]: %02X\n", m.partition2.chsEndSector);
    printf("partition2[chsEndCylinder]: %02X\n", m.partition2.chsEndCylinder);
    printf("partition2[startLBASector]: %u\n", m.partition2.startLBASector);
    printf("partition2[sectors]: %u\n", m.partition2.sectors);
    printf("partition2[total]: %lf Giga Byte\n", ((((m.partition2.sectors * 512.0) / 1024.0) / 1024.0) / 1024.0));
    putchar('\n');
    printf("partition3[status]: %02X\n", m.partition3.status);
    printf("partition3[chsStartHead]: %02X\n", m.partition3.chsStartHead);
    printf("partition3[chsStartSector]: %02X\n", m.partition3.chsStartSector);
    printf("partition3[chsStartCylinder]: %02X\n", m.partition3.chsStartCylinder);
    printf("partition3[partitionType]: %02X\n", m.partition3.partitionType);
    printf("partition3[chsEndHead]: %02X\n", m.partition3.chsEndHead);
    printf("partition3[chsEndSector]: %02X\n", m.partition3.chsEndSector);
    printf("partition3[chsEndCylinder]: %02X\n", m.partition3.chsEndCylinder);
    printf("partition3[startLBASector]: %u\n", m.partition3.startLBASector);
    printf("partition3[sectors]: %u\n", m.partition3.sectors);
    printf("partition3[total]: %u Gyga Byte\n", m.partition3.sectors * 512 / 1024 / 1024);
    printf("partition3[total]: %lf Giga Byte\n", ((((m.partition3.sectors * 512.0) / 1024.0) / 1024.0) / 1024.0));
    putchar('\n');
    printf("partition4[status]: %02X\n", m.partition4.status);
    printf("partition4[chsStartHead]: %02X\n", m.partition4.chsStartHead);
    printf("partition4[chsStartSector]: %02X\n", m.partition4.chsStartSector);
    printf("partition4[chsStartCylinder]: %02X\n", m.partition4.chsStartCylinder);
    printf("partition4[partitionType]: %02X\n", m.partition4.partitionType);
    printf("partition4[chsEndHead]: %02X\n", m.partition4.chsEndHead);
    printf("partition4[chsEndSector]: %02X\n", m.partition4.chsEndSector);
    printf("partition4[chsEndCylinder]: %02X\n", m.partition4.chsEndCylinder);
    printf("partition4[startLBASector]: %u\n", m.partition4.startLBASector);
    printf("partition4[sectors]: %u\n", m.partition4.sectors);
    printf("partition4[total]: %u Gyga Byte\n", m.partition4.sectors * 512 / 1024 / 1024);
    printf("partition4[total]: %lf Giga Byte\n", ((((m.partition4.sectors * 512.0) / 1024.0) / 1024.0) / 1024.0));
    putchar('\n');

    puts("dump GPT...");
    printf("efi: ");
    for (int i = 0; i < sizeof(g.efi); ++i) {
        printf("%c", g.efi[i]);
    }
    putchar('\n');
    printf("version: ");
    for (int i = 0; i < sizeof(g.revision); ++i) {
        printf("%02X", g.revision[i]);
    }
    putchar('\n');
    printf("headerSize: %u\n", g.headerSize);
    printf("headerCRC32: %u\n", g.headerCRC32);
    printf("reserve1: %u\n", g.reserve1);
    printf("currentLBA: %lu\n", g.currentLBA);
    printf("backupLBA: %lu\n", g.backupLBA);
    printf("firstUsableLBA: %lu\n", g.firstUsableLBA);
    printf("GUID: ");
    for (int i = 0; i < sizeof(g.GUID); ++i) {
        printf("%02X", g.GUID[i]);
    }
    putchar('\n');
    printf("partitionArrayLBA: %lu\n", g.partitionArrayLBA);
    printf("partitionArrayCnt: %u\n", g.partitionArrayCnt);
    printf("onePartitionArraySize: %u\n", g.onePartitionArraySize);
    printf("partitionArrayCRC32: %u\n", g.partitionArrayCRC32);

    putchar('\n');
    puts("dump GPT Entry...");
    GPTENTRY e[128];

    for (int i = 0; i < g.partitionArrayCnt; ++i) {
        fread(&e[i], sizeof(GPTENTRY), 1, fr);
        if (e[i].firstLBA == 0) {
            break;
        }

        printf("ENTRY %3d\n", i);
        printf("partitionGUID: ");
        for (int j = 0; j < sizeof(e[i].partitionGUID); ++j) {
            printf("%02X", e[i].partitionGUID[j]);
        }
        putchar('\n');
        printf("uniqueGUID: ");
        for (int j = 0; j < sizeof(e[i].uniqueGUID); ++j) {
            printf("%02X", e[i].uniqueGUID[j]);
        }
        putchar('\n');
        printf("firstLBA: %lu\n", e[i].firstLBA);
        printf("lastLBA: %lu\n", e[i].lastLBA);
        printf("flags: %016lX\n", e[i].flags);
        printf("partitionName: ");
        for (int j = 0; j < sizeof(e[i].partitionName); ++j) {
            printf("%c", e[i].partitionName[j]);
        }
        putchar('\n');
    }
    fseek(fr, e[0].firstLBA * 512, SEEK_SET);
    putchar('\n');
    printf("fat start: %08lx", e[0].firstLBA * 512);
    putchar('\n');
    FAT32 f;
    fread(&f, sizeof(FAT32), 1, fr);
    putchar('\n');
    puts("dump Fat32...");
    printf("jumCode: ");
    for (int i = 0; i < sizeof(f.jumCode); ++i) {
        printf("%02X ", f.jumCode[i]);
    }
    putchar('\n');
    printf("oemName: ");
    for (int i = 0; i < sizeof(f.oemName); ++i) {
        printf("%c", f.oemName[i]);
    }
    putchar('\n');
    printf("bytesPerSector: %u\n", f.bytesPerSector);
    printf("sectorsPerCluster: %u\n", f.sectorsPerCluster);
    printf("reserveSectors: %u\n", f.reserveSectors);
    printf("numFats: %u\n", f.numFats);
    printf("rootEntryDirs: %u\n", f.rootEntryDirs);
    printf("totalSector16: %u\n", f.totalSector16);
    printf("media: %02X\n", f.media);
    printf("fatSize16: %u\n", f.fatSize16);
    printf("sectorPerTruck: %u\n", f.sectorPerTruck);
    printf("numHeads: %u\n", f.numHeads);
    printf("hidden: %u\n", f.hidden);
    printf("fatSize32: %08X\n", f.fatSize32);
    printf("fatSize32: %u\n", f.fatSize32);
    printf("flags: %04X\n", f.flags);
    printf("media: %u\n", f.version);
    printf("rootEntryClusPos: %u\n", f.rootEntryClusPos);
    printf("fsinfoEntrySector: %u\n", f.fsinfoEntrySector);
    printf("backupBootSector: %u\n", f.backupBootSector);
    printf("drive: %02X\n", f.drive);
    printf("bootSignature: %02X\n", f.bootSignature);
    printf("volumeId: %u\n", f.volumeId);
    printf("volumeLabel: ");
    for (int i = 0; i < sizeof(f.volumeLabel); ++i) {
        printf("%c", f.volumeLabel[i]);
    }
    putchar('\n');
    printf("fileSystemType: ");
    for (int i = 0; i < sizeof(f.fileSystemType); ++i) {
        printf("%c", f.fileSystemType[i]);
    }
    putchar('\n');
    printf("signature: %04X\n", f.signature);
    putchar('\n');

    // printf("Root Dir = %08x\n", f.reserveSectors  * 512 + f.fatSize32 * f.numFats);
    // printf("Root Dir = %u\n", f.reserveSectors  * 512 + f.fatSize32 * f.numFats);
    printf("Root Dir = %08x\n", (f.reserveSectors + f.fatSize32 * f.numFats) * 512);
    printf("Root Dir = %u\n", (f.reserveSectors + f.fatSize32 * f.numFats) * 512);
    putchar('\n');

    FAT32FSINFO fi;
    fread(&fi, sizeof(FAT32FSINFO), 1, fr);
    puts("dump FSINFO...");
    printf("signature1: %04X\n", fi.signature1);
    printf("signature2: %04X\n", fi.signature2);
    printf("freeCluster: %u\n", fi.freeCluster);
    printf("lastAllocateCluster: %u\n", fi.lastAllocateCluster);
    printf("signature3: %04X\n", fi.signature3);
// printf("%08X\n", ftell(fr));

    putchar('\n');

// hexDump(2048*512);

    /*
    uint32_t *fat = (uint32_t *)malloc(f.fatSize32 * f.bytesPerSector);
    fseek(fr, (f.reserveSectors -2) * f.bytesPerSector, SEEK_CUR);
    printf("%08lX\n", ftell(fr));
    fread(fat, f.fatSize32 * f.bytesPerSector, 1, fr);
    putchar('\n');
    puts("dump ROOT...");
    fseek(fr, f.fatSize32 * f.bytesPerSector, SEEK_CUR);
*/
    // hexDump();
    // dumpTree(ftell(fr), fr, fat, 0, 0);
    // fseek(fr, (f.reserveSectors - 2 + f.fatSize32 * f.numFats) * f.bytesPerSector, SEEK_CUR);
    /*
    for (int i = 0; i < 51 ; ++i) {
        fat32entry fe;
        fread(&fe, sizeof(fe), 1, fr);
        if (fe.attr != 0) {
            printf("%d\n", i * sizeof(fe) / 512);
            printf("name: ");
            for (int j = 0; j < sizeof(fe.name); ++j) {
                putchar(fe.name[j]);
                // printf("%02X", fe.name[j]);
            }
            putchar('\n');
            printf("attr: %02x\n", fe.attr);
            printf("opt: %02x\n", fe.opt);
            printf("subtime: %u\n", fe.subtime);
            printf("createdTime: %u\n", fe.createdTime);
            printf("createdDate: %u\n", fe.createdDate);
            printf("accessDate: %u\n", fe.accessDate);
            printf("clusterHi: %04X\n", fe.clusterHi);
            printf("writedTime: %u\n", fe.writedTime);
            printf("writedDate: %u\n", fe.writedDate);
            printf("clusterLo: %04X\n", fe.clusterLo);
            printf("fileSize: %u\n", fe.fileSize);
        }
    }
    */

   int dataArea = 0x100000 + (f.reserveSectors + f.fatSize32 * f.numFats) * 512;
   /*
   fseek(fr, 0x104000 + 0x500 * 4, SEEK_SET);
   for (int i = 0; i < 0x200; ++i) {
        uint32_t var;
        fread(&var, sizeof(var), 1, fr);
        printf("%04d -> %08x\n", i, var);
   }
   */
   // 先頭4バイトx2は無効のでオフセット8
   // から始める
   fseek(fr, 0x104008, SEEK_SET);
   uint32_t var;
   for (;;) {
        fread(&var, sizeof(var), 1, fr);
        printf("%08x\n", var);
        printf(">>>%08x\n", 0x104000 + var * 4);
        if (var >= EOC) {
            break;
        }
       fseek(fr, 0x104000 + var * 4, SEEK_SET);
   }

    fclose(fr);

    return 0;
}
