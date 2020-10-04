/*
 * drv_oled_ssd1306.c
 *
 *  Created on: Mar 15, 2020
 *      Author: kimi
 */

/********** Include **********/

#include "common_type.h"
#include "drv_oled_ssd1306.h"

/********** Define **********/

#define SSD1306_I2C_ADDRESS		0x3C
#define DEV_ADDRESS				(SSD1306_I2C_ADDRESS << 1)

#define I2C_TIMEOUT				100 /* 100ms */

#define CONTROL_BYTE_COMMAND	0x00
#define CONTROL_BYTE_DATA		0x40

/********** Type **********/

/********** Constant **********/

const static uint8_t txDataSSD1306Initilize[] = {
		CONTROL_BYTE_COMMAND,
		0xAE,			/* Set Display ON/OFF:  Display OFF */
		0xA8, 0x3F,		/* Set MUX Ratio:  Default(63) */
		0xD3, 0x00,		/* Set Display Offset:  Default(0) */
		0x40,			/* Set Display Start Line:  Default(0) */
#if DISPLAY_ROTATE == ROTATE_HORIZONTAL
		0xA1,			/* Set Segment Re-map:  column address 127 is mapped to SEG0 */
		0xC8,			/* Set COM Output Scan Direction:  remapped mode. Scan from COM[N-1] to COM0 */
#else	/* ROTATE_VERTICAL */
		0xA0,			/* Set Segment Re-map:  column address 0 is mapped to SEG0 */
		0xC8,			/* Set COM Output Scan Direction:  remapped mode. Scan from COM[N-1] to COM0 */
//		0xC0,			/* Set COM Output Scan Direction:  normal mode (RESET) Scan from COM0 to COM[N –1] */
#endif	/* DISPLAY_ROTATE */
		0xDA, 0x12,		/* Set COM Pins Hardware Configuration:  Alternative COM pin configuration, Disable COM Left/Right remap */
		0x81, 0x7F,		/* Set Contrast Control:  Default(127) */
		0xA4,			/* Entire Display ON:  Resume to RAM content display */
		0xA6,			/* Set Normal/Inverse Display:  Normal */
		0xD5, 0x80,		/* Set Display Clock Divide Ratio/Oscillator Frequency:  Default */
		0x8D, 0x14,		/* Charge Pump Setting:  Enable Charge Pump */
#if DISPLAY_ROTATE == ROTATE_HORIZONTAL
		0x20, 0x00,			/* Set Memory Addressing Mode:  Horizontal Addressing Mode */
#else	/* ROTATE_VERTICAL */
		0x20, 0x01,			/* Set Memory Addressing Mode:  Vertical Addressing Mode */
#endif	/* DISPLAY_ROTATE */
		0x21, 0x00, 0x7F,	/* Set Column Address:  Column start address(0), Column end address(127)  */
		0x22, 0x00, 0x07	/* Set Page Address:  Page start Address(0), Page end Address(7) */
};

const static uint8_t txDataSSD1306DisplayOn[] = {
		CONTROL_BYTE_COMMAND,
		0xAF			/* Set Display ON/OFF:  Display ON */
};

/********** Variable **********/

uint8_t VideoMemory[OLED_WIDTH * OLED_HEIGHT / 8 + 1];	/* byte 0はコントロールバイト */

static I2C_HandleTypeDef* i2cHandle;

/********** Function Prototype **********/

static void drvOledAsyncTransmit(uint8_t* data, uint16_t size);
static void drvOledSyncTransmit(uint8_t* data, uint16_t size);

/********** Function **********/

/*=== 初期化関数 ===*/
void DrvOledInit(void)
{
	/* ビデオメモリ初期化 */
	DrvOledClearMemory(DOT_BLACK);

	/* 画面を初期化(リセット直前のデータ送信を完了させるために1画面分のデータを送信) */
	drvOledSyncTransmit((uint8_t*)VideoMemory, sizeof(VideoMemory));

	/* OLED SSD1306初期化 */
	drvOledSyncTransmit((uint8_t*)txDataSSD1306Initilize, sizeof(txDataSSD1306Initilize));

	/* 画面を初期化 */
	drvOledSyncTransmit((uint8_t*)VideoMemory, sizeof(VideoMemory));

	/* Display ON */
	drvOledSyncTransmit((uint8_t*)txDataSSD1306DisplayOn, sizeof(txDataSSD1306DisplayOn));
}

void DrvOledSetConfig(I2C_HandleTypeDef* hi2c)
{
	i2cHandle = hi2c;
}

/*=== 描画関数 ===*/
void DrvOledDisplay(void)
{
	HAL_I2C_StateTypeDef i2cState;

	i2cState = HAL_I2C_GetState(i2cHandle);

	if (i2cState == HAL_I2C_STATE_READY) {
		/* コントロールバイト設定 */
		VideoMemory[0] = CONTROL_BYTE_DATA;

		/* 描画実行(非同期通信) */
		drvOledAsyncTransmit((uint8_t*)&VideoMemory, sizeof(VideoMemory));
	}
}

/*=== 表示クリア関数 ===*/
void DrvOledClearMemory(uint8_t color)
{
	uint8_t c;
	uint16_t index;

	/* コントロールバイト設定 */
	VideoMemory[0] = CONTROL_BYTE_DATA;

	if (color == DOT_BLACK) {
		c = 0x00;
	} else {
		c = 0xFF;
	}

	for (index=0; index<(OLED_WIDTH * OLED_HEIGHT / 8); index++) {
		VideoMemory[index + 1] = c;
	}
}

/*=== 非同期送信関数 ===*/
static void drvOledAsyncTransmit(uint8_t* data, uint16_t size)
{
	HAL_I2C_Master_Transmit_DMA(i2cHandle, DEV_ADDRESS, data, size);
}

/*=== 同期送信関数 ===*/
static void drvOledSyncTransmit(uint8_t* data, uint16_t size)
{
	HAL_I2C_Master_Transmit(i2cHandle, DEV_ADDRESS, data, size, I2C_TIMEOUT);
}
