/*
 * drv_YMZ294.h
 *
 *  Created on: May 2, 2020
 *      Author: kimi
 */

#ifndef INC_DRV_YMZ294_H_
#define INC_DRV_YMZ294_H_

#define YMZ294_CH_A		0
#define YMZ294_CH_B		0
#define YMZ294_CH_C		0

void drvYMZ294SetFrequency(uint8_t channel, uint8_t freq_table_index);
void drvYMZ294Init(void);

#endif /* INC_DRV_YMZ294_H_ */
