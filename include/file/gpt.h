/**
 * @file gpt.h
 * @author 真理の御魂最聖麻原彰晃尊師 (sonshi@aum.or.jp)
 * @brief GPTの定義
 * @version 0.1
 * @date 2023-01-03
 * @since 0.1
 * @see https://en.wikipedia.org/wiki/File_Allocation_Table
 * @see http://resove.cool.coocan.jp/fatKnowledge/fatStructure.html
 * @bug FAT32のわかりやすい日本語ドキュメントないかなあ…
 *
 * @copyright Copyleft 🄯 199X 真理の御魂最聖麻原彰晃尊師 All Wrongs Reversed.
 *
 */

#ifndef __GPT__H__
#define __GPT__H__

#include <stdint.h>

/**
 * @brief GPT
 *
 */
typedef struct _GPT
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

} __attribute__((packed)) GPT;

/**
 * @brief GPTのエントリー
 *
 */
typedef struct _GPTENTRY
{
    uint8_t partitionGUID[16];
    uint8_t uniqueGUID[16];
    uint64_t firstLBA;
    uint64_t lastLBA;
    uint64_t flags;
    uint8_t partitionName[72];

} __attribute__((packed)) GPTENTRY;

#endif
