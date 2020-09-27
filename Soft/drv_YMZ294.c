/*
 * drv_YMZ294.c
 *
 *  Created on: May 2, 2020
 *      Author: kimi
 */


#include "main.h"
#include "drv_YMZ294.h"
#include "drv_YMZ294_rodata.h"

static void writeRegister(uint8_t addr, uint8_t data);
static void outputDataPin(uint8_t data);

void drvYMZ294SetFrequency(uint8_t channel, uint8_t freq_table_index)
{
	/* 周波数設定 */
	writeRegister(0x00 + channel * 2, YMZ294_Freq_Table_L[freq_table_index]);
	writeRegister(0x01 + channel * 2, YMZ294_Freq_Table_H[freq_table_index]);
}

void drvYMZ294Init(void)
{
	// YMZ294のレジスタを全て初期化

	// ミキサーOFF
	writeRegister(0x07, 0x3F);

	// 音量0
	writeRegister(0x08, 0x00);
	writeRegister(0x09, 0x00);
	writeRegister(0x0A, 0x00);

	// 楽音周波数初期化
	writeRegister(0x00, 0x00);
	writeRegister(0x01, 0x00);
	writeRegister(0x02, 0x00);
	writeRegister(0x03, 0x00);
	writeRegister(0x04, 0x00);
	writeRegister(0x05, 0x00);

	// ノイズ音周波数初期化
	writeRegister(0x06, 0x00);

	// エンベロープ周波数初期化
	writeRegister(0x0B, 0x00);
	writeRegister(0x0C, 0x00);

	// エンベロープ形状初期化
	writeRegister(0x0D, 0x00);

	writeRegister(0x07, 0b00111000);
	writeRegister(0x00, 0b00011100);
	writeRegister(0x01, 0b00000011);
	writeRegister(0x08, 0b00001111);

//	writeRegister(0x08, 0b00011111);
//	writeRegister(0x07, 0b00111110);
//	writeRegister(0x0D, 0b00001110);
//	writeRegister(0x0B, 0x10);
//	writeRegister(0x0C, 0x00);
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
