/**
 * @file fat32.h
 * @author feynoobs (feynoobs@fssoft.co.jp)
 * @brief Fat32の定義
 * @version 0.1
 * @date 2023-01-03
 * @since 0.1
 * @see https://en.wikipedia.org/wiki/File_Allocation_Table
 * @see http://resove.cool.coocan.jp/fatKnowledge/fatStructure.html
 * @bug FAT32のわかりやすい日本語ドキュメントないかなあ…
 *
 * @copyright Copyleft 🄯 199X feynoobs All Wrongs Reversed.
 *
 */

#ifndef __FAT32__H__
#define __FAT32__H__

#include <stdint.h>

/**
 * @brief FAT32ブートセクタ
 *
 */
typedef struct _fat32
{
    uint8_t jumCode[3];         /**< ジャンプコード @note 使われる? */
    uint8_t oemName[8];         /**< OMW名 */
    uint16_t bytesPerSector;    /**< 1セクタあたりのバイト数 @note 多分今どきのPCはすべて512B  */
    uint8_t sectorsPerCluster;  /**< 1クラスタあたりのセクタ数 */
    uint16_t reserveSectors;    /**< このテーブルを含む予約セクターの数 */
    uint8_t numFats;            /**< FATテーブルの個数 */
    uint16_t rootEntryDirs;
    uint16_t totalSector16;
    uint8_t media;
    uint16_t fatSize16;
    uint16_t sectorPerTruck;
    uint16_t numHeads;
    uint32_t hidden;
    uint32_t totalSector32;
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

typedef struct _fat32fsinfo
{
    uint32_t signature1;
    uint8_t reserved1[480];
    uint32_t signature2;
    uint32_t freeCluster;
    uint32_t lastAllocateCluster;
    uint8_t reserve2[12];
    uint32_t signature3;
} __attribute__((packed)) fat32fsinfo;

typedef struct _fat32entry
{
    uint8_t name[11];
    uint8_t attr;
    uint8_t opt;
    uint8_t subtime;
    uint16_t createdTime;
    uint16_t createdDate;
    uint16_t accessDate;
    uint16_t clusterHi;
    uint16_t writedTime;
    uint16_t writedDate;
    uint16_t clusterLo;
    uint32_t fileSize;

} __attribute__((packed)) fat32entry;

#endif
