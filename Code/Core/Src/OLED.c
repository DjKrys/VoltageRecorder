#include "SSD1306.h"
#include "LIB_Config.h"
#include "OLED.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void OLED_Init(void)
{
	ssd1306_init();
}

void OLED_Clear_screen(void)
{
	ssd1306_clear_screen(0x00);
}
	
void OLED_Full_screen(void)
{
	ssd1306_clear_screen(0xFF);
}

void OLED_Power_on(void)
{
	for(int i = 0; i < 3; i ++)
	{
		OLED_Clear_screen();
		ssd1306_display_string(15, 24, "Please wait", 16, 1);
		ssd1306_refresh_gram();
		HAL_Delay(500);
		ssd1306_display_string(15, 24, "Please wait.", 16, 1);
		ssd1306_refresh_gram();
		HAL_Delay(500);
		ssd1306_display_string(15, 24, "Please wait..", 16, 1);
		ssd1306_refresh_gram();
		HAL_Delay(500);
		ssd1306_display_string(15, 24, "Please wait...", 16, 1);
		ssd1306_refresh_gram();
		HAL_Delay(1000);
	}
}

void OLED_Start(void)
{
	OLED_Clear_screen();
	ssd1306_display_string(2, 0, "Rejestrator napiecia", 12, 1);
	ssd1306_display_string(45, 16, "Praca", 16, 1);
	ssd1306_display_string(20, 32, "inzynierska", 16, 1);
	
	ssd1306_display_string(0, 52, "REC", 12, 0);
	ssd1306_display_string(21, 52, "FILTER", 12, 0);
	ssd1306_display_string(60, 52, "SAMPLE", 12, 0);
	ssd1306_display_string(98, 52, "ENTER", 12, 0);
	
	ssd1306_refresh_gram();
}

int OLED_intToStr(int x, char str[], int d, int bipolar) 
{ 
	int i = 0;
	int k = 0;
	
	if (bipolar == 1)
	{
		k = 1;
		i = 1;
	}		
	
	int tmp;

	do
	{ 
		str[i++] = (x % 10) + '0'; 
		x = x / 10; 
	} 
	while(x);

	while (i < d) 
		str[i++] = '0';


	int l = i - 1;
	while (k < l) 
	{ 
		tmp = str[k]; 
		str[k] = str[l]; 
		str[l] = tmp; 
		k++; 
		l--; 
	}

	str[i] = '\0'; 
	return i; 
} 

void OLED_Display_float(uint8_t x, uint8_t y, float data)
{
	int j = 0;
	int bipolar = 0;
	char *pomiar = (char*) malloc(2);
	int AFTERPOINT = 5;
	
	if (data >= 10 || data <= -10)
	{
		AFTERPOINT = AFTERPOINT - 1;
	}
	
	if(data < 0) 
	{
		*pomiar = '-';
		data = -data;
		bipolar = 1;
		AFTERPOINT = AFTERPOINT - 1;
	}
	
	int ipart = (int)data; 
	float fpart = data - (float)ipart; 

	j = OLED_intToStr(ipart, pomiar, 0, bipolar); 

	if (AFTERPOINT != 0) 
	{ 
		pomiar[j] = '.';
		fpart = fpart * pow(10, AFTERPOINT); 
		OLED_intToStr((int)fpart, pomiar + j + 1, AFTERPOINT,0); 
	} 

	ssd1306_display_string(x, y, pomiar, 12, 1);

	free(pomiar);
}

void OLED_Display_Int(uint8_t x, uint8_t y, float data)
{
	int j = 0;
	int bipolar = 0;
	char *pomiar = (char*) malloc(2);
	int AFTERPOINT = 0;
	
	int ipart = (int)data; 
	float fpart = data - (float)ipart; 

	j = OLED_intToStr(ipart, pomiar, 0, bipolar); 

	if (AFTERPOINT != 0) 
	{ 
		pomiar[j] = '.';
		fpart = fpart * pow(10, AFTERPOINT); 
		OLED_intToStr((int)fpart, pomiar + j + 1, AFTERPOINT,0); 
	} 

	ssd1306_display_string(x, y, pomiar, 12, 1);

	free(pomiar);
}

void OLED_Display_measurement(int battery, int RECORD_BOOL, float pomiar1, float pomiar2, float pomiar3, float pomiar4, float pomiar5, float pomiar6, float pomiar7, float pomiar8)
{
	OLED_Clear_screen();
	ssd1306_display_string(0, 16, "V1:", 12, 1);
	ssd1306_display_string(0, 28, "V2:", 12, 1);
	ssd1306_display_string(0, 40, "V3:", 12, 1);
	ssd1306_display_string(0, 52, "V4:", 12, 1);
	ssd1306_display_string(64, 16, "V5:", 12, 1);
	ssd1306_display_string(64, 28, "V6:", 12, 1);
	ssd1306_display_string(64, 40, "V7:", 12, 1);
	ssd1306_display_string(64, 52, "V8:", 12, 1);
	
	OLED_Display_float(17, 16, pomiar1);
	OLED_Display_float(17, 28, pomiar2);
	OLED_Display_float(17, 40, pomiar3);
	OLED_Display_float(17, 52, pomiar4);
	
	OLED_Display_float(81, 16, pomiar5);
	OLED_Display_float(81, 28, pomiar6);
	OLED_Display_float(81, 40, pomiar7);
	OLED_Display_float(81, 52, pomiar8);

	if (battery >= 100 )
		ssd1306_display_string(84, 0, "100", 12, 1);
	else if (battery < 10)
		OLED_Display_Int(96, 0, battery);
	else
		OLED_Display_Int(90, 0, battery);
	
	ssd1306_display_string(103, 0, "%", 12, 1);
	
	if(battery <= 7)
		ssd1306_draw_bitmap(112, 2, &c_chBatOver816[0], 16, 8);
	else if( battery <= 25)
		ssd1306_draw_bitmap(112, 2, &c_chBatQuarter816[0], 16, 8);
	else if (battery <= 50)
		ssd1306_draw_bitmap(112, 2, &c_chBatHalf816[0], 16, 8);
	else if (battery <= 75)
		ssd1306_draw_bitmap(112, 2, &c_chBatAlmostFull816[0], 16, 8);
	else
		ssd1306_draw_bitmap(112, 2, &c_chBatFull816[0], 16, 8);
	
	if(RECORD_BOOL == 1)
	{
		ssd1306_display_string(55, 0, "REC", 12, 0);
	}
	
	ssd1306_refresh_gram();
}

void OLED_Top(int state)
{
	OLED_Clear_screen();
	ssd1306_display_string(0, 0, "Select:", 12, 1);
	ssd1306_display_string(98, 52, "ENTER", 12, 0);
	
	ssd1306_display_string(0, 16, "Record START", 12, 1);
	ssd1306_display_string(0, 28, "Record STOP", 12, 1);
	ssd1306_display_string(0, 40, "DISPLAY MEASUREMENT", 12, 1);
		
	switch(state)
	{
		case 1:
			ssd1306_display_string(0, 16, "Record START", 12, 0);
			break;
		case 2:
			ssd1306_display_string(0, 28, "Record STOP", 12, 0);
			break;
		case 3:
			ssd1306_display_string(0, 40, "DISPLAY MEASUREMENT", 12, 0);
			break;
	}
	
	ssd1306_refresh_gram();
}

void OLED_Sampling_frequency1(int state)
{
	OLED_Clear_screen();
	ssd1306_display_string(0, 0, "Sample time:", 12, 1);
	ssd1306_display_string(98, 52, "ENTER", 12, 0);
	
	ssd1306_display_string(0, 16, "60ms", 12, 1);
	ssd1306_display_string(0, 28, "100ms", 12, 1);
	ssd1306_display_string(0, 40, "250ms", 12, 1);
	ssd1306_display_string(0, 52, "500ms", 12, 1);
	ssd1306_display_string(64, 16, "1s", 12, 1);
	ssd1306_display_string(64, 28, "2s", 12, 1);
	ssd1306_display_string(64, 40, "5s", 12, 1);
	ssd1306_display_string(64, 52, "10s", 12, 1);
		
	switch(state)
	{
		case 1:
			ssd1306_display_string(0, 16, "60ms", 12, 0);
			break;
		case 2:
			ssd1306_display_string(0, 28, "100ms", 12, 0);
			break;
		case 3:
			ssd1306_display_string(0, 40, "250ms", 12, 0);
			break;
		case 4:
			ssd1306_display_string(0, 52, "500ms", 12, 0);
			break;
		case 5:
			ssd1306_display_string(64, 16, "1s", 12, 0);
			break;
		case 6:
			ssd1306_display_string(64, 28, "2s", 12, 0);
			break;
		case 7:
			ssd1306_display_string(64, 40, "5s", 12, 0);
			break;
		case 8:
			ssd1306_display_string(64, 52, "10s", 12, 0);
			break;
	}
	
	ssd1306_refresh_gram();
}

void OLED_Sampling_frequency2(int state)
{
	OLED_Clear_screen();
	ssd1306_display_string(0, 0, "Sample time:", 12, 1);
	ssd1306_display_string(98, 52, "ENTER", 12, 0);
	
	ssd1306_display_string(0, 16, "15ms", 12, 1);
	ssd1306_display_string(0, 28, "50ms", 12, 1);
	ssd1306_display_string(0, 40, "150ms", 12, 1);
	ssd1306_display_string(0, 52, "250ms", 12, 1);
	ssd1306_display_string(64, 16, "500ms", 12, 1);
	ssd1306_display_string(64, 28, "1s", 12, 1);
	ssd1306_display_string(64, 40, "2s", 12, 1);
	ssd1306_display_string(64, 52, "5s", 12, 1);
		
	switch(state)
	{
		case 1:
			ssd1306_display_string(0, 16, "15ms", 12, 0);
			break;
		case 2:
			ssd1306_display_string(0, 28, "50ms", 12, 0);
			break;
		case 3:
			ssd1306_display_string(0, 40, "150ms", 12, 0);
			break;
		case 4:
			ssd1306_display_string(0, 52, "250ms", 12, 0);
			break;
		case 5:
			ssd1306_display_string(64, 16, "500ms", 12, 0);
			break;
		case 6:
			ssd1306_display_string(64, 28, "1s", 12, 0);
			break;
		case 7:
			ssd1306_display_string(64, 40, "2s", 12, 0);
			break;
		case 8:
			ssd1306_display_string(64, 52, "5s", 12, 0);
			break;
	}
	
	ssd1306_refresh_gram();
}

void OLED_Select_Filter(int state)
{
	OLED_Clear_screen();
	ssd1306_display_string(0, 0, "Select filter:", 12, 1);
	ssd1306_display_string(98, 52, "ENTER", 12, 0);
	
	ssd1306_display_string(0, 16, "Sinc3 z tlumieniem 50Hz", 12, 1);
	ssd1306_display_string(0, 40, "Sinc3 bez tlumienia 50Hz", 12, 1);
		
	switch(state)
	{
		case 1:
			ssd1306_display_string(0, 16, "Sinc3 z tlumieniem 50Hz", 12, 0);
			break;
		case 2:
			ssd1306_display_string(0, 40, "Sinc3 bez tlumienia 50Hz", 12, 0);
			break;
	}
	
	ssd1306_refresh_gram();
}

void OLED_Select_Channel(int state)
{
	OLED_Clear_screen();
	ssd1306_display_string(0, 0, "Select channel:", 12, 1);
	ssd1306_display_string(98, 52, "ENTER", 12, 0);
	
	ssd1306_display_string(0, 16, "Channel 1", 12, 1);
	ssd1306_display_string(0, 28, "Channel 2", 12, 1);
	ssd1306_display_string(0, 40, "Channel 3", 12, 1);
	ssd1306_display_string(0, 52, "Channel 4", 12, 1);
	ssd1306_display_string(64, 16, "Channel 5", 12, 1);
	ssd1306_display_string(64, 28, "Channel 6", 12, 1);
	ssd1306_display_string(64, 40, "Channel 7", 12, 1);
	ssd1306_display_string(64, 52, "Channel 8", 12, 1);
		
	switch(state)
	{
		case 1:
			ssd1306_display_string(0, 16, "Channel 1", 12, 0);
			break;
		case 2:
			ssd1306_display_string(0, 28, "Channel 2", 12, 0);
			break;
		case 3:
			ssd1306_display_string(0, 40, "Channel 3", 12, 0);
			break;
		case 4:
			ssd1306_display_string(0, 52, "Channel 4", 12, 0);
			break;
		case 5:
			ssd1306_display_string(64, 16, "Channel 5", 12, 0);
			break;
		case 6:
			ssd1306_display_string(64, 28, "Channel 6", 12, 0);
			break;
		case 7:
			ssd1306_display_string(64, 40, "Channel 7", 12, 0);
			break;
		case 8:
			ssd1306_display_string(64, 52, "Channel 8", 12, 0);
			break;
	}
	
	ssd1306_refresh_gram();
}

void OLED_ON_OFF_Channel(int state)
{
	OLED_Clear_screen();
	ssd1306_display_string(0, 0, "OFF OLED", 12, 1);
	ssd1306_display_string(64, 0, "OFF device", 12, 1);
	
	ssd1306_display_string(0, 16, "Channel1", 12, 1);
	ssd1306_display_string(0, 28, "Channel2", 12, 1);
	ssd1306_display_string(0, 40, "Channel3", 12, 1);
	ssd1306_display_string(0, 52, "Channel4", 12, 1);
	ssd1306_display_string(64, 16, "Channel5", 12, 1);
	ssd1306_display_string(64, 28, "Channel6", 12, 1);
	ssd1306_display_string(64, 40, "Channel7", 12, 1);
	ssd1306_display_string(64, 52, "Channel8", 12, 1);
	
	switch(state)
	{
		case 1:
			ssd1306_display_string(0, 0, "OFF OLED", 12, 0);
			break;
		case 2:
			ssd1306_display_string(0, 16, "Channel1", 12, 0);
			break;
		case 3:
			ssd1306_display_string(0, 28, "Channel2", 12, 0);
			break;
		case 4:
			ssd1306_display_string(0, 40, "Channel3", 12, 0);
			break;
		case 5:
			ssd1306_display_string(0, 52, "Channel4", 12, 0);
			break;
		case 6:
			ssd1306_display_string(64, 0, "OFF device", 12, 0);
			break;
		case 7:
			ssd1306_display_string(64, 16, "Channel5", 12, 0);
			break;
		case 8:
			ssd1306_display_string(64, 28, "Channel6", 12, 0);
			break;						
		case 9:
			ssd1306_display_string(64, 40, "Channel7", 12, 0);
			break;
		case 10:
			ssd1306_display_string(64, 52, "Channel8", 12, 0);
			break;
	}
	
	ssd1306_refresh_gram();
}

void OLED_Succesfull(void)
{
	OLED_Clear_screen();
	ssd1306_display_string(20, 24, "SUCCESFULL", 16, 0);
	ssd1306_refresh_gram();
}

void OLED_Error(void)
{
	OLED_Clear_screen();
	ssd1306_display_string(46, 26, "ERROR", 16, 0);
	ssd1306_refresh_gram();
}

void OLED_Activate(void)
{
	OLED_Clear_screen();
	ssd1306_display_string(24, 26, "Channel ON", 16, 0);
	ssd1306_refresh_gram();
}

void OLED_Deactivate(void)
{
	OLED_Clear_screen();
	ssd1306_display_string(20, 26, "Channel OFF", 16, 0);
	ssd1306_refresh_gram();
}
