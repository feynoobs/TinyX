#ifndef __FAT32__H__
#define __FAT32__H__

#include <stdint.h>

/**
 * @brief
 *
 */
typedef struct _fat32
{
    uint32_t bootJump;
    uint8_t oem[8];
    uint16_t bytePerSec;
    uint8_t secPerClus;
    uint16_t reserve1;
    uint8_t numFats;
    uint16_t rootEntryDirs;
    uint16_t totalSec16;
    uint8_t media;
    uint16_t fatSize16;
    uint16_t srcPerTruck;
    uint16_t numHeads;
    uint32_t hidden;
    uint32_t totalSec32;
    uint32_t fatSize32;
    uint16_t flags;
    uint16_t version;
    uint32_t rootEntryClusPos;
    uint16_t fsinfoEntrySector;
    uint16_t backupBootSector;
    uint8_t reserve2[12];
    uint8_t drive;
    uint8_t reserve3;
    uint8_t bootSignature;
    uint32_t volumeId;
    uint8_t volumeLabel[11];
    uint8_t fileSystemType[8];
    uint8_t bootCode[420];
    uint16_t signature;
} __attribute__ ((packed)) fat32;

#endif
