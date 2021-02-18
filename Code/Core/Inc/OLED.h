#include "stm32f1xx_hal.h"
#ifndef __OLED_H
#define __OLED__H

void OLED_Init(void);
void OLED_Clear_screen(void);
void OLED_Full_screen(void);
void OLED_Power_on(void);
void OLED_Start(void);
void OLED_Display_measurement(int battery, int RECORD_BOOL, float pomiar1, float pomiar2, float pomiar3, float pomiar4, float pomiar5, float pomiar6, float pomiar7, float pomiar8);
void OLED_Sampling_frequency1(int state);
void OLED_Sampling_frequency2(int state);
void OLED_Succesfull(void);
void OLED_Activate(void);
void OLED_Deactivate(void);
void OLED_Select_Channel(int state);
void OLED_Select_Filter(int state);
void OLED_ON_OFF_Channel(int state);
void OLED_Top(int state);
void OLED_Error(void);

#endif
