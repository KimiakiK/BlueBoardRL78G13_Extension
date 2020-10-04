/*
 * lib_graphic.h
 *
 *  Created on: Oct 1, 2020
 *      Author: kimi
 */

#ifndef INC_LIB_GRAPHIC_H_
#define INC_LIB_GRAPHIC_H_

/********** Include **********/

/********** Define **********/

/* 描画色 */
#define COLOR_BLACK		(0)
#define COLOR_WHITE		(1)

/********** Type **********/

typedef struct spriteType {
	const uint8_t* img;
	uint8_t x;
	uint8_t y;
	uint8_t w;
	uint8_t h;
} sprite_t;

/********** Constant **********/

/********** Variable **********/

extern const sprite_t sprite_sample;

/********** Function Prototype **********/

/*=== 描画クリア関数 ===*/
void DrawClear(uint8_t color);

/*=== 四角形描画関数 ===*/
void DrawRect(uint8_t pos_x, uint8_t pos_y, uint8_t w, uint8_t h, uint8_t color);

/*=== スプライト描画関数 ===*/
void DrawSprite(const sprite_t* sprite);

/*=== スプライト描画(絶対位置指定)関数 ===*/
void DrawSpritePosition(const sprite_t* sprite, int16_t pos_x, int16_t pos_y);

/*=== スプライト描画(相対位置指定)関数 ===*/
void DrawSpriteOffset(const sprite_t* sprite, int16_t offset_x, int16_t offset_y);

#endif /* INC_LIB_GRAPHIC_H_ */
