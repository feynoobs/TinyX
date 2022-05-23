#include <stdio.h>
#include <stdint.h>

#include "mbr.h"
#include "gpt.h"
#include "fat32.h"

int main(void)
{
    mbr m;
    gpt g;

    FILE *fr = fopen("/home/feynoobs/Desktop/hage.bin", "r");
    fread(&m, sizeof(mbr), 1, fr);
    fread(&g, sizeof(gpt), 1, fr);

    printf("mbr = %ld byte\n", sizeof(mbr));
    printf("gpt = %ld byte\n", sizeof(gpt));
    printf("gpt entry = %ld byte\n", sizeof(gptEntry));
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

    puts("dump GPT Entry...");
    gptEntry e[128];

    for (int i = 0; i < g.partitionArrayCnt; ++i) {
        fread(&e[i], sizeof(gptEntry), 1, fr);
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
    fseek(fr, e[0].firstLBA * 512,SEEK_SET);
    fat32 f;
    fread(&f, sizeof(fat32), 1, fr);
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
    printf("totalSector32: %u\n", f.totalSector32);
    printf("fatSize32: %u\n", f.fatSize32);
    printf("flags: %04X\n", f.flags);
    printf("media: %u\n", f.version);
    printf("rootEntryClusPos: %u\n", f.rootEntryClusPos);
    printf("fsinfoEntrySector: %u\n", f.fsinfoEntrySector);
    printf("backupBootSector: %u\n", f.backupBootSector);
    printf("drive: %02X\n", f.drive);
    printf("drive: %02X\n", f.bootSignature);
    printf("drive: %u\n", f.volumeId);
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

    fclose(fr);

    return 0;
}
