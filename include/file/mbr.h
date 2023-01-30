/**
 * @file mbr.h
 * @author 真理の御魂最聖麻原彰晃尊師 (sonshi@aum.or.jp)
 * @brief マスターブートレコードの定義
 * @version 0.1
 * @date 2023-01-03
 * @since 0.1
 * @see https://ja.wikipedia.org/wiki/%E3%83%9E%E3%82%B9%E3%82%BF%E3%83%BC%E3%83%96%E3%83%BC%E3%83%88%E3%83%AC%E3%82%B3%E3%83%BC%E3%83%89
 *
 * @copyright Copyleft 🄯 199X 真理の御魂最聖麻原彰晃尊師 All Wrongs Reversed.
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
    uint8_t status;             /**< ブートフラグ(0x80=ブート可/0x00=ブート不可) */
    uint8_t chsStartHead;       /**< パーティション最初のセクタ(ヘッド) @note おそらく使われない */
    uint8_t chsStartSector;     /**< パーティション最初のセクタ(シリンダ上位2ビットとセクタ) @note おそらく使われない */
    uint8_t chsStartCylinder;   /**< パーティション最初のセクタ(シリンダ下位8ビット) @note おそらく使われない */
    uint8_t partitionType;      /**< パーティションの種類 */
    uint8_t chsEndHead;         /**< パーティション最後のセクタ(ヘッド) @note おそらく使われない */
    uint8_t chsEndSector;       /**< パーティション最後のセクタ(シリンダ上位2ビットとセクタ) @note おそらく使われない */
    uint8_t chsEndCylinder;     /**< パーティション最後のセクタ(シリンダ下位8ビット) @note おそらく使われない */
    uint32_t startLBASector;    /**< パーティションの最初のセクタ(LBA) */
    uint32_t sectors;           /**< パーティションのセクタ数(LBA) @note 通常1LBAは512Bなので1パーティションでは512*(2^32)Bが上限になる */

} __attribute__((packed)) partition;

/**
 * @brief マスターブートレコード
 *
 */
typedef struct _mbr
{
    uint8_t code[446];      /**< ブートストラップローダ */
    partition partition1;   /**< パーティション1 */
    partition partition2;   /**< パーティション2 */
    partition partition3;   /**< パーティション3 */
    partition partition4;   /**< パーティション4 */
    uint16_t signature;     /**< シグネチャ(0xAA55) */

} __attribute__((packed)) mbr;

#endif
