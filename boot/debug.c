#include <stdio.h>
#include "file/mbr.h"
#include "file/gpt.h"
#include "file/fat32.h"

static void mbPtDump(partition *p)
{
    puts("=========================================");
    printf("%20s %02X\n", "status", p->status);
    printf("%20s %02X\n", "chsStartHead", p->chsStartHead);
    printf("%20s %02X\n", "chsStartSector", p->chsStartSector);
    printf("%20s %02X\n", "chsStartCylinder", p->chsStartCylinder);
    printf("%20s %02X\n", "partitionType", p->partitionType);
    printf("%20s %02X\n", "chsEndHead", p->chsEndHead);
    printf("%20s %02X\n", "chsEndSector", p->chsEndSector);
    printf("%20s %08X\n", "startLBASector", p->startLBASector);
    printf("%20s %08X\n", "sectors", p->sectors);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    mbr mbr;
    fp = fopen(argv[1], "r");

    if (fp != NULL) {
        fread(&mbr, sizeof(mbr), 1, fp);
        mbPtDump(&mbr.partition1);
        mbPtDump(&mbr.partition2);
        mbPtDump(&mbr.partition3);
        mbPtDump(&mbr.partition4);
    }
}
