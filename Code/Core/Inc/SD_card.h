#ifndef __SD_CARD_H
#define __SD_CARD_H

#include "stm32f1xx_hal.h"

#define AFTERPOINT 7

uint8_t Init_SD(void);
uint8_t open_file_SD(int channel);
uint8_t open_SD(void);
uint8_t write_to_file_SD(int channel, float data, float czas);
uint8_t write_SD(float data1, float data2, float data3, float data4, float data5, float data6, float data7, float data8, float czas);
uint8_t close_file_SD(int channel);
uint8_t close_SD(void);
uint8_t check_free_SD(void);
uint8_t write_info(void);


#endif
