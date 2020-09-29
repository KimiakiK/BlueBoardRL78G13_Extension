/*
 * drv_YMZ294_rodata.h
 *
 *  Created on: May 5, 2020
 *      Author: kimi
 */

#ifndef INC_DRV_YMZ294_RODATA_H_
#define INC_DRV_YMZ294_RODATA_H_

/********** Include **********/

/********** Define **********/

/* 休符 */
#define NOTE_REST	(0xFF)

#define YMZ294_FREQ_TABLE_MIN	23	/* 22以下の周波数は出力不可 */
#define YMZ294_FREQ_TABLE_MAX	128

/********** Type **********/

/********** Constant **********/

extern const uint8_t YMZ294_Freq_Table_H[YMZ294_FREQ_TABLE_MAX];
extern const uint8_t YMZ294_Freq_Table_L[YMZ294_FREQ_TABLE_MAX];
extern const music_t music_kaerunouta;

/********** Variable **********/

/********** Function Prototype **********/

#endif /* INC_DRV_YMZ294_RODATA_H_ */
