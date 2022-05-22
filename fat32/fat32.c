#include <stdio.h>
#include "fat32.h"

void dumpFat32Header(void)
{
    FILE *fr;
    fat32 data;

    fr = fopen("hage.bin", "r");
    fread(&data, sizeof(data), 1, fr);

    fclose(fr);
}

int main(void)
{
    printf("%ld", sizeof(fat32));
    return 0;
}
