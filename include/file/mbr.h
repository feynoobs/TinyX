/**
 * @file mbr.h
 * @author feynoobs (feynoobs@fssoft.co.jp)
 * @brief マスターブートレコードの定義
 * @version 0.1
 * @date 2023-01-03
 *
 * @copyright Copyleft (i) 199X all wrongs reversed
 *
 */

#ifndef __MBR__H__
#define __MBR__H__

#include <stdint.h>

/**
 * @brief 論理パーティション
 *
 */
typedef struct _partition
{
    uint8_t status;
    uint8_t chsStartHead;
    uint8_t chsStartSector;
    uint8_t chsStartCylinder;
    uint8_t partitionType;
    uint8_t chsEndHead;
    uint8_t chsEndSector;
    uint8_t chsEndCylinder;
    uint32_t startLBASector;
    uint32_t sectors;

} __attribute__((packed)) partition;

typedef struct _mbr
{
    uint8_t code[446];
    partition partition1;
    partition partition2;
    partition partition3;
    partition partition4;
    uint16_t signature;

} __attribute__((packed)) mbr;

#endif
