/*
 * drv_oled_ssd1306.h
 *
 *  Created on: Mar 15, 2020
 *      Author: kimi
 */

#ifndef INC_DRV_OLED_SSD1306_H_
#define INC_DRV_OLED_SSD1306_H_

/********** Include **********/

/********** Define **********/

/* 色定義 */
#define DOT_BLACK		0
#define DOT_WHITE		1

/* 画面の向き */
#define ROTATE_HORIZONTAL	0	/* 画面横置き */
#define ROTATE_VERTICAL		1	/* 画面縦置き */

/* 【コンフィグ】画面の向きを選択 */
#define DISPLAY_ROTATE	ROTATE_HORIZONTAL
// #define DISPLAY_ROTATE	ROTATE_VERTICAL


#if DISPLAY_ROTATE == ROTATE_HORIZONTAL

/* ドット数 */
#define OLED_HEIGHT		64
#define OLED_WIDTH		128

#define MEMORY_MAP(x, y)	((x) + (y) * OLED_WIDTH + 1)

#define MEMORY_SET_DOT(x, y)	(VideoMemory[MEMORY_MAP((x), (y) / 8)] |= 0x01 << ((y) % 8))
#define MEMORY_CLR_DOT(x, y)	(VideoMemory[MEMORY_MAP((x), (y) / 8)] &= ~(0x01 << ((y) % 8)) )

#else	/* ROTATE_VERTICAL */

/* ドット数 */
#define OLED_HEIGHT		128
#define OLED_WIDTH		64

#define MEMORY_MAP(x, y)	((x) + (y) * OLED_WIDTH / 8 + 1)

#define MEMORY_SET_DOT(x, y)	(VideoMemory[MEMORY_MAP((x) / 8, (y))] |= 0x01 << ((x) % 8))
#define MEMORY_CLR_DOT(x, y)	(VideoMemory[MEMORY_MAP((x) / 8, (y))] &= ~(0x01 << ((x) % 8)) )

#endif	/* DISPLAY_ROTATE */

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

extern uint8_t VideoMemory[OLED_WIDTH * OLED_HEIGHT / 8 + 1];

/********** Function Prototype **********/

void DrvOledInit(void);
void DrvOledSetConfig(I2C_HandleTypeDef* hi2c);
void DrvOledDisplay(void);
void DrvOledClearMemory(uint8_t color);

#endif /* INC_DRV_OLED_SSD1306_H_ */
