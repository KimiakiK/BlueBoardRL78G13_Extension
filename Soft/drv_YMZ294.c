/*
 * drv_YMZ294.c
 *
 *  Created on: May 2, 2020
 *      Author: kimi
 */

/********** Include **********/

#include "main.h"
#include "drv_YMZ294.h"
#include "drv_YMZ294_rodata.h"

/********** Define **********/

#define MAIN_PERIOD	(10)	/* メイン周期[ms] */

enum job_state_enum {
	JOB_STATE_STOP = 0,
	JOB_STATE_PLAYING,
	JOB_STATE_END
};

/********** Type **********/

typedef struct job_type {
	uint8_t type;
	uint8_t state;
	note_t* music;
	uint16_t note_num;
	uint16_t note_index;
	uint16_t note_timer;
	uint8_t volume;
} job_t;

/********** Constant **********/

/********** Variable **********/

static job_t job[YMZ294_CH_NUM];

/********** Function Prototype **********/

static void initJob(uint8_t channel);
static void writeRegister(uint8_t addr, uint8_t data);
static void outputDataPin(uint8_t data);

/********** Function **********/

/*=== 初期化関数 ===*/
void DrvYMZ294Init(void)
{
	uint8_t channel;

	/* ジョブ初期化 */
	for (channel=0; channel<YMZ294_CH_NUM; channel++) {
		initJob(channel);
	}

	// YMZ294のレジスタを全て初期化
	// ミキサーOFF
	writeRegister(YMZ294_ADDR_MIXER, 0x3F);

	// 音量0
	writeRegister(YMZ294_ADDR_VOLUME + YMZ294_CH_A, YMZ294_VOLUME_MUTE);
	writeRegister(YMZ294_ADDR_VOLUME + YMZ294_CH_B, YMZ294_VOLUME_MUTE);
	writeRegister(YMZ294_ADDR_VOLUME + YMZ294_CH_C, YMZ294_VOLUME_MUTE);

	// 楽音周波数初期化
	writeRegister(YMZ294_ADDR_FREQ + YMZ294_ADDR_L + YMZ294_CH_A, 0x00);
	writeRegister(YMZ294_ADDR_FREQ + YMZ294_ADDR_H + YMZ294_CH_A, 0x00);
	writeRegister(YMZ294_ADDR_FREQ + YMZ294_ADDR_L + YMZ294_CH_B, 0x00);
	writeRegister(YMZ294_ADDR_FREQ + YMZ294_ADDR_H + YMZ294_CH_B, 0x00);
	writeRegister(YMZ294_ADDR_FREQ + YMZ294_ADDR_L + YMZ294_CH_C, 0x00);
	writeRegister(YMZ294_ADDR_FREQ + YMZ294_ADDR_H + YMZ294_CH_C, 0x00);

	// ノイズ音周波数初期化
	writeRegister(YMZ294_ADDR_NOISE, 0x00);

	// エンベロープ周波数初期化
	writeRegister(YMZ294_ADDR_ENVE_FREQ + YMZ294_ADDR_L, 0x00);
	writeRegister(YMZ294_ADDR_ENVE_FREQ + YMZ294_ADDR_H, 0x00);

	// エンベロープ形状初期化
	writeRegister(YMZ294_ADDR_ENVE_TYPE, 0x00);


	// ミキサー チャネルA,B,C出力許可
	writeRegister(YMZ294_ADDR_MIXER, 0x38);
}

/*=== 周期関数 ===*/
void DrvYMZ294Main(void)
{
	uint8_t channel;

	for (channel=0; channel<YMZ294_CH_NUM; channel++) {
		switch (job[channel].state) {
		case JOB_STATE_STOP:
			/* 処理なし */
			break;
		case JOB_STATE_PLAYING:
			job[channel].note_timer += MAIN_PERIOD;
			if (job[channel].note_timer == MAIN_PERIOD) {
				/* 音開始 */
				if ((job[channel].music + job[channel].note_index)->pitch != NOTE_REST) {
					DrvYMZ294SetFrequency(channel, (job[channel].music + job[channel].note_index)->pitch);
					job[channel].volume = YMZ294_VOLUME_MAX;
					DrvYMZ294SetVolume(channel, job[channel].volume);
				} else {
					job[channel].volume = YMZ294_VOLUME_MUTE;
					DrvYMZ294SetVolume(channel, job[channel].volume);
				}
			} else if (job[channel].note_timer > (job[channel].music + job[channel].note_index)->duration) {
				/* 音終了 */
				job[channel].note_timer = 0;
				job[channel].note_index++;
				if (job[channel].note_index >= job[channel].note_num) {
					job[channel].state = JOB_STATE_END;
				}
			} else {
				/* 音出力中 */
				if ((job[channel].music + job[channel].note_index)->pitch != NOTE_REST) {
					uint8_t progress;
					uint8_t volume;
					progress = job[channel].note_timer / ((job[channel].music + job[channel].note_index)->duration / 10);
					if (progress <= 5) {
						volume = YMZ294_VOLUME_MAX;
					} else if (progress <= 6) {
						volume = YMZ294_VOLUME_MAX / 2;
					} else if (progress <= 8) {
						volume = YMZ294_VOLUME_MAX / 4;
					} else {
						volume = YMZ294_VOLUME_MAX / 8;
					}
					if (job[channel].volume != volume) {
						job[channel].volume = volume;
						DrvYMZ294SetVolume(channel, job[channel].volume);
					}
				}
			}
			break;
		case JOB_STATE_END:
			if (job[channel].type == JOB_TYPE_ONCE) {
				initJob(channel);
				job[channel].volume = YMZ294_VOLUME_MUTE;
				DrvYMZ294SetVolume(channel, job[channel].volume);
			} else {
				job[channel].state = JOB_STATE_PLAYING;
				job[channel].note_index = 0;
				job[channel].note_timer = 0;
			}
			break;
		default:
			initJob(channel);
			break;
		}
	}
}

/*=== ジョブ設定関数 ===*/
void DrvYMZ294SetJob(uint8_t channel, music_t* music, uint8_t job_type)
{
	if (channel < YMZ294_CH_NUM) {
		initJob(channel);
		job[channel].type = job_type;
		job[channel].music = music->note_address;
		job[channel].note_num = music->note_num;
		job[channel].state = JOB_STATE_PLAYING;
	}
}

/*=== 音量設定関数 ===*/
void DrvYMZ294SetVolume(uint8_t channel, uint8_t volume)
{
	if ((channel < YMZ294_CH_NUM) && (volume <= YMZ294_VOLUME_MAX)){
		writeRegister(YMZ294_ADDR_VOLUME + channel, volume);
	}
}

/*=== 周波数設定関数 ===*/
void DrvYMZ294SetFrequency(uint8_t channel, uint8_t freq_table_index)
{
	if ((channel < YMZ294_CH_NUM) && (freq_table_index >= YMZ294_FREQ_TABLE_MIN) && (freq_table_index < YMZ294_FREQ_TABLE_MAX)){
		writeRegister(YMZ294_ADDR_FREQ + YMZ294_ADDR_L + channel * 2, YMZ294_Freq_Table_L[freq_table_index]);
		writeRegister(YMZ294_ADDR_FREQ + YMZ294_ADDR_H + channel * 2, YMZ294_Freq_Table_H[freq_table_index]);
	}
}

/*=== ジョブ初期化関数 ===*/
static void initJob(uint8_t channel)
{
	job[channel].type = JOB_TYPE_ONCE;
	job[channel].state = JOB_STATE_STOP;
	job[channel].music = (void *)0;
	job[channel].note_num = 0;
	job[channel].note_index = 0;
	job[channel].note_timer = 0;
	job[channel].volume = YMZ294_VOLUME_MUTE;
}

static void writeRegister(uint8_t addr, uint8_t data)
{
	// アドレス書き込み
	HAL_GPIO_WritePin(YMZ294_A0_GPIO_Port, YMZ294_A0_Pin, GPIO_PIN_RESET);
	// 書き込み開始
	HAL_GPIO_WritePin(YMZ294_CS_GPIO_Port, YMZ294_CS_Pin, GPIO_PIN_RESET);
	// アドレス値設定
	outputDataPin(addr);
	// 書き込み終了
	HAL_GPIO_WritePin(YMZ294_CS_GPIO_Port, YMZ294_CS_Pin, GPIO_PIN_SET);

	// データ書き込み
	HAL_GPIO_WritePin(YMZ294_A0_GPIO_Port, YMZ294_A0_Pin, GPIO_PIN_SET);
	// 書き込み開始
	HAL_GPIO_WritePin(YMZ294_CS_GPIO_Port, YMZ294_CS_Pin, GPIO_PIN_RESET);
	// データ値設定
	outputDataPin(data);
	// 書き込み終了
	HAL_GPIO_WritePin(YMZ294_CS_GPIO_Port, YMZ294_CS_Pin, GPIO_PIN_SET);
}

static void outputDataPin(uint8_t data)
{
	if ((data & 0x01) == 0x01) {
		HAL_GPIO_WritePin(YMZ294_D0_GPIO_Port, YMZ294_D0_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(YMZ294_D0_GPIO_Port, YMZ294_D0_Pin, GPIO_PIN_RESET);
	}
	if ((data & 0x02) == 0x02) {
		HAL_GPIO_WritePin(YMZ294_D1_GPIO_Port, YMZ294_D1_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(YMZ294_D1_GPIO_Port, YMZ294_D1_Pin, GPIO_PIN_RESET);
	}
	if ((data & 0x04) == 0x04) {
		HAL_GPIO_WritePin(YMZ294_D2_GPIO_Port, YMZ294_D2_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(YMZ294_D2_GPIO_Port, YMZ294_D2_Pin, GPIO_PIN_RESET);
	}
	if ((data & 0x08) == 0x08) {
		HAL_GPIO_WritePin(YMZ294_D3_GPIO_Port, YMZ294_D3_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(YMZ294_D3_GPIO_Port, YMZ294_D3_Pin, GPIO_PIN_RESET);
	}
	if ((data & 0x10) == 0x10) {
		HAL_GPIO_WritePin(YMZ294_D4_GPIO_Port, YMZ294_D4_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(YMZ294_D4_GPIO_Port, YMZ294_D4_Pin, GPIO_PIN_RESET);
	}
	if ((data & 0x20) == 0x20) {
		HAL_GPIO_WritePin(YMZ294_D5_GPIO_Port, YMZ294_D5_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(YMZ294_D5_GPIO_Port, YMZ294_D5_Pin, GPIO_PIN_RESET);
	}
	if ((data & 0x40) == 0x40) {
		HAL_GPIO_WritePin(YMZ294_D6_GPIO_Port, YMZ294_D6_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(YMZ294_D6_GPIO_Port, YMZ294_D6_Pin, GPIO_PIN_RESET);
	}
	if ((data & 0x80) == 0x80) {
		HAL_GPIO_WritePin(YMZ294_D7_GPIO_Port, YMZ294_D7_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(YMZ294_D7_GPIO_Port, YMZ294_D7_Pin, GPIO_PIN_RESET);
	}
}
