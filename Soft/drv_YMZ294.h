/*
 * drv_YMZ294.h
 *
 *  Created on: May 2, 2020
 *      Author: kimi
 */

#ifndef INC_DRV_YMZ294_H_
#define INC_DRV_YMZ294_H_

/********** Include **********/

/********** Define **********/

/* YMZ294 レジスタアドレス一覧 */
#define YMZ294_ADDR_FREQ	(0x00)
#define YMZ294_ADDR_NOISE	(0x06)
#define YMZ294_ADDR_MIXER	(0x07)
#define YMZ294_ADDR_VOLUME	(0x08)
#define YMZ294_ADDR_ENVE_FREQ	(0x0B)
#define YMZ294_ADDR_ENVE_TYPE	(0x0D)
#define YMZ294_ADDR_L	(0)
#define YMZ294_ADDR_H	(1)

/* YMZ294 音量定数 */
#define YMZ294_VOLUME_MUTE	(0x00)
#define YMZ294_VOLUME_MAX	(0x0F)

/* YMZ294 チャネル定数 */
enum ymz294_channel_enum {
	YMZ294_CH_A = 0,
	YMZ294_CH_B,
	YMZ294_CH_C,
	YMZ294_CH_NUM
};

/* ジョブ種類 */
enum job_type_enum {
	JOB_TYPE_ONCE = 0,
	JOB_TYPE_LOOP
};

/********** Type **********/

typedef struct note_type {
	uint8_t pitch; /* 音符の高さ */
	uint16_t duration; /* 音符の長さ */
} note_t;

typedef struct music_type {
	const note_t* note_address;
	uint16_t note_num;
} music_t;

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void DrvYMZ294Init(void);
void DrvYMZ294Main(void);
void DrvYMZ294SetJob(uint8_t channel, music_t* music, uint8_t job_type);
void DrvYMZ294SetVolume(uint8_t channel, uint8_t volume);
void DrvYMZ294SetFrequency(uint8_t channel, uint8_t freq_table_index);

#endif /* INC_DRV_YMZ294_H_ */
