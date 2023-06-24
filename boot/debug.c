#include <stdio.h>
#include "file/mbr.h"
#include "file/gpt.h"
#include "file/fat32.h"

static void hex(uint64_t var, uint8_t *buf)
{
    for (int i = 0; i < 8; ++i) {
        sprintf(&buf[i], "%c", var >> (16 - (i+1) * 2));
    }
}

static void mbPtDump(PARTITION *p)
{
    puts("=========================================");
    printf("%-20s %02X\n", "status", p->status);
    printf("%-20s %02X\n", "chsStartHead", p->chsStartHead);
    printf("%-20s %02X\n", "chsStartSector", p->chsStartSector);
    printf("%-20s %02X\n", "chsStartCylinder", p->chsStartCylinder);
    printf("%-20s %02X\n", "partitionType", p->partitionType);
    printf("%-20s %02X\n", "chsEndHead", p->chsEndHead);
    printf("%-20s %02X\n", "chsEndSector", p->chsEndSector);
    printf("%-20s %08X\n", "startLBASector", p->startLBASector);
    printf("%-20s %08X\n", "sectors", p->sectors);
}

static void gptDump(GPT *g)
{
    uint8_t buf[8];

    puts("=================GPT=====================");
    printf("%-20s ", "efi");
    for (uint8_t i = 0; i < 8; ++i) {
        printf("%02X", g->efi[i]);
    }
    putchar('\n');
    printf("%-20s ", "revision");
    for (uint8_t i = 0; i < 4; ++i) {
        printf("%02X", g->revision[i]);
    }
    putchar('\n');
    printf("%-20s %08X\n", "headerSize", g->headerSize);
    printf("%-20s %08X\n", "headerCRC32", g->headerCRC32);
    printf("%-20s %08X\n", "reserve1", g->reserve1);
    hex(g->currentLBA, buf);

}

int main(int argc, char *argv[])
{
    FILE *fp;

    fp = fopen(argv[1], "r");

    if (fp != NULL) {
        MBR mbr;
        fread(&mbr, sizeof(mbr), 1, fp);
        mbPtDump(&mbr.partition1);
        mbPtDump(&mbr.partition2);
        mbPtDump(&mbr.partition3);
        mbPtDump(&mbr.partition4);


    }
}
