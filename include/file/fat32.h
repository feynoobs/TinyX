/**
 * @file fat32.h
 * @author 真理の御魂最聖麻原彰晃尊師 (sonshi@aum.or.jp)
 * @brief Fat32の定義
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

#ifndef __FAT32__H__
#define __FAT32__H__

#include <stdint.h>

#define BOC     0x0FFFFFF7      // 不良クラスタ
#define EOC     0x0FFFFFF8      // クラスタの終端

/**
 * @brief FAT32ブートセクタ
 *
 */
typedef struct _FAT32
{
    uint8_t jumCode[3];         /**< ジャンプコード @note 使われる? */
    uint8_t oemName[8];         /**< OMW名 */
    uint16_t bytesPerSector;    /**< 1セクタあたりのバイト数 @note 多分今どきのPCはすべて512B  */
    uint8_t sectorsPerCluster;  /**< 1クラスタあたりのセクタ数 */
    uint16_t reserveSectors;    /**< このテーブルを含む予約セクターの数 */
    uint8_t numFats;            /**< FATテーブルの個数 */
    uint16_t rootEntryDirs;     /**< @note FAT32では使われないらしい */
    uint16_t totalSector16;     /**< @note FAT32では使われないらしい */
    uint8_t media;              /**< 0xF8 */
    uint16_t fatSize16;         /**< @note FAT32では使われないらしい */
    uint16_t sectorPerTruck;    /**< @note 使われないらしい */
    uint16_t numHeads;          /**< @note 使われないらしい */
    uint32_t hidden;            /**< 0 */
    uint32_t totalSector32;     /**< ボリュームの総セクタ数 */
    uint32_t fatSize32;         /**< FATテーブル1個のサイズ */
    uint16_t flags;             /**< @note あまり基にしなくていい？ */
    uint16_t version;           /**< バージョン情報 */
    uint32_t rootEntryClusPos;  /**< rootディレクトリのあるクラスタ */
    uint16_t fsinfoEntrySector; /**< fsinfo構造体のある場所 */
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
} __attribute__ ((packed)) FAT32;

typedef struct _FAT32FSINFO
{
    uint32_t signature1;
    uint8_t reserved1[480];
    uint32_t signature2;
    uint32_t freeCluster;
    uint32_t lastAllocateCluster;
    uint8_t reserve2[12];
    uint32_t signature3;
} __attribute__((packed)) FAT32FSINFO;

typedef struct _FAT32ENTRY
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
} __attribute__((packed)) FAT32ENTRY;

typedef struct _FAT32LFNENTRY
{
    uint8_t order;          /** 1から始まるLFNの順序 */
    uint16_t name1[5];      /** UTF16のファイル名5文字 */
    uint8_t attr;           /** 属性。 0x0fのはず */
    uint8_t type;           /** 参考サイトによると0みたい。 @see https://zenn.dev/hidenori3/articles/3ce349c02e79fa */
    uint8_t checkSum;       /** チェックサム @see https://zenn.dev/hidenori3/articles/3ce349c02e79fa */
    uint16_t name2[6];      /** UTF16のファイル名6〜11文字 */
    uint16_t clus;          /** 参考サイトによると0みたい。 @see https://zenn.dev/hidenori3/articles/3ce349c02e79fa */
    uint16_t name3[2];      /** UTF16のファイル名12〜13文字 */
} __attribute__((packed)) FAT32LFNENTRY;

/**
 * @brief LFN
 *
 */
typedef struct _LFMENTRY
{
    uint8_t order;
    uint8_t name1[10];
    uint8_t attr;
    uint8_t type;
    uint8_t checksum;
    uint8_t name2[12];
    uint16_t reserve2;
    uint8_t name3[4];

} __attribute__((packed)) LFMENTRY;

#endif
