#ifndef __GPT__H__
#define __GPT__H__

#include <stdint.h>

typedef struct _gpt
{
    uint8_t efi[8];
    uint8_t revision[4];
    uint32_t headerSize;
    uint32_t headerCRC32;
    uint32_t reserve1;
    uint64_t currentLBA;
    uint64_t backupLBA;
    uint64_t firstUsableLBA;
    uint64_t lastUsableLBA;
    uint8_t GUID[16];
    uint64_t partitionArrayLBA;
    uint32_t partitionArrayCnt;
    uint32_t onePartitionArraySize;
    uint32_t partitionArrayCRC32;
    uint8_t reserve2[420];

} __attribute__((packed)) gpt;

#endif
