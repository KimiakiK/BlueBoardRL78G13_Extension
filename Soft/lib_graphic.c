/*
 * lib_graphic.c
 *
 *  Created on: Oct 1, 2020
 *      Author: kimi
 */

/********** Include **********/

#include "common_type.h"
#include "drv_oled_ssd1306.h"
#include "lib_graphic.h"

/********** Define **********/

/********** Type **********/

/********** Constant **********/

const uint8_t img_sample[] = {
		0b00000111, 0b11100000,
		0b00001111, 0b11110000,
		0b00011111, 0b11111000,
		0b00111111, 0b11111100,
		0b01111111, 0b11111110,
		0b11111011, 0b11011111,
		0b11111011, 0b11011111,
		0b11111011, 0b11011111,
		0b11111111, 0b11111111,
		0b11111111, 0b11111111,
		0b11111111, 0b11111111,
		0b01111001, 0b10011110,
		0b00111110, 0b01111100,
		0b00011111, 0b11111000,
		0b00001111, 0b11110000,
		0b00000111, 0b11100000,
};
const sprite_t sprite_sample = {
		img_sample,
		4,
		4,
		16,
		16
};

/********** Variable **********/

/********** Function Prototype **********/

/********** Function **********/

/*=== 描画クリア関数 ===*/
void DrawClear(uint8_t color)
{
	if (color == COLOR_BLACK) {
		DrvOledClearMemory(DOT_BLACK);
	} else {
		DrvOledClearMemory(DOT_WHITE);
	}
}

/*=== 四角形描画関数 ===*/
void DrawRect(uint8_t pos_x, uint8_t pos_y, uint8_t w, uint8_t h, uint8_t color)
{
	uint8_t x, y;

	for (x=pos_x; x<(pos_x + w); x++) {
		for (y=pos_y; y<(pos_y + h); y++) {
			if (color == COLOR_BLACK) {
				MEMORY_CLR_DOT(x, y);
			} else {
				MEMORY_SET_DOT(x, y);
			}
		}
	}
}

/*=== スプライト描画関数 ===*/
void DrawSprite(const sprite_t* sprite)
{
	uint8_t x, y;

	for (x=0; x<sprite->w; x++) {
		for (y=0; y<sprite->h; y++) {
			if ((sprite->img[x / 8 + y * ((sprite->w + 7) / 8)] & (0x80 >> (x % 8))) == DOT_BLACK) {
				MEMORY_CLR_DOT(x + sprite->x, y + sprite->y);
			} else {
				MEMORY_SET_DOT(x + sprite->x, y + sprite->y);
			}
		}
	}
}

/*=== スプライト描画(絶対位置指定)関数 ===*/
void DrawSpritePosition(const sprite_t* sprite, int16_t pos_x, int16_t pos_y)
{
	uint8_t x, y;
	int16_t target_x, target_y;

	for (x=0; x<sprite->w; x++) {
		for (y=0; y<sprite->h; y++) {
			target_x = x + pos_x;
			target_y = y + pos_y;
			if ((target_x >= 0) && (target_x < OLED_HEIGHT) && (target_y >= 0) && (target_y < OLED_WIDTH)) {
				if ((sprite->img[x / 8 + y * ((sprite->w + 7) / 8)] & (0x80 >> (x % 8))) == DOT_BLACK) {
					MEMORY_CLR_DOT(target_x, target_y);
				} else {
					MEMORY_SET_DOT(target_x, target_y);
				}
			}
		}
	}
}

/*=== スプライト描画(相対位置指定)関数 ===*/
void DrawSpriteOffset(const sprite_t* sprite, int16_t offset_x, int16_t offset_y)
{
	uint8_t x, y;
	int16_t target_x, target_y;

		for (x=0; x<sprite->w; x++) {
			for (y=0; y<sprite->h; y++) {
				target_x = x + sprite->x + offset_x;
				target_y = y + sprite->y + offset_y;
				if ((target_x >= 0) && (target_x < OLED_HEIGHT) && (target_y >= 0) && (target_y < OLED_WIDTH)) {
					if ((sprite->img[x / 8 + y * ((sprite->w + 7) / 8)] & (0x80 >> (x % 8))) == DOT_BLACK) {
						MEMORY_CLR_DOT(target_x, target_y);
					} else {
						MEMORY_SET_DOT(target_x, target_y);
					}
				}
			}
		}
}

