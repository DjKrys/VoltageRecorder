#include "SD_card.h"
#include "fatfs.h"
#include <math.h>
#include <stdlib.h>



FRESULT res;
FATFS SDFatFs;
FIL FileSDCard1, FileSDCard2, FileSDCard3, FileSDCard4, FileSDCard5, FileSDCard6, FileSDCard7, FileSDCard8;

UINT zapisanych_bajtow;

FATFS *pfs;
DWORD fre_clust;
uint32_t total_SD, free_SD;




int SD_intToStr(int x, char str[], int d, int bipolar) 
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

uint8_t Init_SD(void)
{
	res = f_mount(&SDFatFs, "", 1);
		
		if(res != FR_OK) 
			return 1;
		else 
			return 0;
}

uint8_t open_file_SD(int channel) 
{
	switch(channel)
	{
		case 1:
			res = f_open(&FileSDCard1, "Kanal-1.txt", FA_WRITE | FA_CREATE_ALWAYS);
			break;
		case 2:
			res = f_open(&FileSDCard2, "Kanal-2.txt", FA_WRITE | FA_CREATE_ALWAYS);
			break;
		case 3:
			res = f_open(&FileSDCard3, "Kanal-3.txt", FA_WRITE | FA_CREATE_ALWAYS);
			break;
		case 4:
			res = f_open(&FileSDCard4, "Kanal-4.txt", FA_WRITE | FA_CREATE_ALWAYS);
			break;
		case 5:
			res = f_open(&FileSDCard5, "Kanal-5.txt", FA_WRITE | FA_CREATE_ALWAYS);
			break;
		case 6:
			res = f_open(&FileSDCard6, "Kanal-6.txt", FA_WRITE | FA_CREATE_ALWAYS);
			break;
		case 7:
			res = f_open(&FileSDCard7, "Kanal-7.txt", FA_WRITE | FA_CREATE_ALWAYS);
			break;
		case 8:
			res = f_open(&FileSDCard8, "Kanal-8.txt", FA_WRITE | FA_CREATE_ALWAYS);
			break;
	}
	
	if(res != FR_OK) 
		return 1;
	else 
		return 0;
}

uint8_t open_SD(void)
{
	if(open_file_SD(1) || open_file_SD(2) || open_file_SD(3) || open_file_SD(4) || open_file_SD(5) || open_file_SD(6) || open_file_SD(7) || open_file_SD(8) != 0)
		return 1;
	else
		return 0;
}

uint8_t write_to_file_SD (int channel, float data, float czas)
{
	int bipolar = 0;
	char *pomiar = (char*) malloc(2);
	char *time = (char*) malloc(3);
	
	if(data < 0) 
	{
		*pomiar = '-';
		data = -data;
		bipolar = 1;
	}

	int ipomiar = (int)data; 
	float fpomiar = data - (float)ipomiar; 

	int i = SD_intToStr(ipomiar, pomiar, 0, bipolar); 

	if (AFTERPOINT != 0) 
	{ 
		pomiar[i] = '.';
		fpomiar = fpomiar * pow(10, AFTERPOINT); 
		SD_intToStr((int)fpomiar, pomiar + i + 1, AFTERPOINT, 0); 
	} 
	
	int itime = (int)czas; 
	float ftime = czas - (float)itime; 

	int j = SD_intToStr(itime, time, 0, 0); 

	if (AFTERPOINT != 0) 
	{ 
		time[j] = '.';
		ftime = ftime * pow(10, AFTERPOINT); 
		SD_intToStr((int)ftime, time + j + 1, AFTERPOINT, 0); 
	}

	switch(channel)
	{
		case 1:
			res = f_write(&FileSDCard1, pomiar , 9, &zapisanych_bajtow);
			res = f_write(&FileSDCard1, "                        " , 18, &zapisanych_bajtow);
			res = f_write(&FileSDCard1, time , 7, &zapisanych_bajtow);
			res = f_write(&FileSDCard1, "\r\n" , 2, &zapisanych_bajtow);
			break;
		case 2:
			res = f_write(&FileSDCard2, pomiar , 9, &zapisanych_bajtow);
			res = f_write(&FileSDCard2, "                        " , 18, &zapisanych_bajtow);
			res = f_write(&FileSDCard2, time , 7, &zapisanych_bajtow);
			res = f_write(&FileSDCard2, "\r\n" , 2, &zapisanych_bajtow);
			break;
		case 3:
			res = f_write(&FileSDCard3, pomiar , 9, &zapisanych_bajtow);
			res = f_write(&FileSDCard3, "                        " , 18, &zapisanych_bajtow);
			res = f_write(&FileSDCard3, time , 7, &zapisanych_bajtow);
			res = f_write(&FileSDCard3, "\r\n" , 2, &zapisanych_bajtow);
			break;
		case 4:
			res = f_write(&FileSDCard4, pomiar , 9, &zapisanych_bajtow);
			res = f_write(&FileSDCard4, "                        " , 18, &zapisanych_bajtow);
			res = f_write(&FileSDCard4, time , 7, &zapisanych_bajtow);
			res = f_write(&FileSDCard4, "\r\n" , 2, &zapisanych_bajtow);
			break;
		case 5:
			res = f_write(&FileSDCard5, pomiar , 9, &zapisanych_bajtow);
			res = f_write(&FileSDCard5, "                        " , 18, &zapisanych_bajtow);
			res = f_write(&FileSDCard5, time , 7, &zapisanych_bajtow);
			res = f_write(&FileSDCard5, "\r\n" , 2, &zapisanych_bajtow);
			break;
		case 6:
			res = f_write(&FileSDCard6, pomiar , 9, &zapisanych_bajtow);
			res = f_write(&FileSDCard6, "                        " , 18, &zapisanych_bajtow);
			res = f_write(&FileSDCard6, time , 7, &zapisanych_bajtow);
			res = f_write(&FileSDCard6, "\r\n" , 2, &zapisanych_bajtow);
			break;
		case 7:
			res = f_write(&FileSDCard7, pomiar , 9, &zapisanych_bajtow);
			res = f_write(&FileSDCard7, "                        " , 18, &zapisanych_bajtow);
			res = f_write(&FileSDCard7, time , 7, &zapisanych_bajtow);
			res = f_write(&FileSDCard7, "\r\n" , 2, &zapisanych_bajtow);
			break;
		case 8:
			res = f_write(&FileSDCard8, pomiar , 9, &zapisanych_bajtow);
			res = f_write(&FileSDCard8, "                        " , 18, &zapisanych_bajtow);
			res = f_write(&FileSDCard8, time , 7, &zapisanych_bajtow);
			res = f_write(&FileSDCard8, "\r\n" , 2, &zapisanych_bajtow);
			break;
	}
	
	if(res != FR_OK) 
		return 1;
		
	free(pomiar);
	free(time);
	return 0;
}

uint8_t write_SD(float data1, float data2, float data3, float data4, float data5, float data6, float data7, float data8, float czas)
{
	if((write_to_file_SD(1,data1, czas) || write_to_file_SD(2,data2, czas) || write_to_file_SD(3,data3, czas) || write_to_file_SD(4,data4, czas) || write_to_file_SD(5,data5, czas) || write_to_file_SD(6,data6, czas) || write_to_file_SD(7,data7, czas) || write_to_file_SD(8,data8, czas)) != 0)
		return 1;
	else 
		return 0;
}

uint8_t close_file_SD(int channel) 
{
	switch(channel)
	{
		case 1:
			res = f_close(&FileSDCard1);
			break;
		case 2:
			res = f_close(&FileSDCard2);
			break;
		case 3:
			res = f_close(&FileSDCard3);
			break;
		case 4:
			res = f_close(&FileSDCard4);
			break;
		case 5:
			res = f_close(&FileSDCard5);
			break;
		case 6:
			res = f_close(&FileSDCard6);
			break;
		case 7:
			res = f_close(&FileSDCard7);
			break;
		case 8:
			res = f_close(&FileSDCard8);
			break;
	}
	if(res != FR_OK) 
		return 1;
	else 
		return 0;
}

uint8_t close_SD(void)
{
	if(close_file_SD(1) || close_file_SD(2) || close_file_SD(3) || close_file_SD(4) || close_file_SD(5) || close_file_SD(6) || close_file_SD(7) || close_file_SD(8) != 0)
		return 1;
	else
		return 0;
}

uint8_t check_free_SD(void)
{
	uint32_t total_SD, free_SD;
	
	if(f_getfree("", &fre_clust, &pfs) != FR_OK)
		return 1;

  total_SD = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
  free_SD = (uint32_t)(fre_clust * pfs->csize * 0.5);
	
	char *total = (char*) malloc(4);
	SD_intToStr(total_SD, total, 0, 0); 

	free(total);
	
	char *fre = (char*) malloc(5);
	SD_intToStr(free_SD, fre, 0, 0);

	free(fre);
	
	return 0;
}

uint8_t write_info(void)
{
	res = f_write(&FileSDCard5, "Napiecie [V]: ", 16, &zapisanych_bajtow);
	res = f_write(&FileSDCard5, "        " , 8, &zapisanych_bajtow);
	res = f_write(&FileSDCard5, "Czas [s]: ", 11, &zapisanych_bajtow);
	res = f_write(&FileSDCard5, "\r\n" , 2, &zapisanych_bajtow);			
	
	res = f_write(&FileSDCard4, "Napiecie [V]: ", 16, &zapisanych_bajtow);
	res = f_write(&FileSDCard4, "        " , 8, &zapisanych_bajtow);
	res = f_write(&FileSDCard4, "Czas [s]: ", 11, &zapisanych_bajtow);
	res = f_write(&FileSDCard4, "\r\n" , 2, &zapisanych_bajtow);			
	
	res = f_write(&FileSDCard3, "Napiecie [V]: ", 16, &zapisanych_bajtow);
	res = f_write(&FileSDCard3, "        " , 8, &zapisanych_bajtow);
	res = f_write(&FileSDCard3, "Czas [s]: ", 11, &zapisanych_bajtow);
	res = f_write(&FileSDCard3, "\r\n" , 2, &zapisanych_bajtow);			
	
	res = f_write(&FileSDCard2, "Napiecie [V]: ", 16, &zapisanych_bajtow);
	res = f_write(&FileSDCard2, "        " , 8, &zapisanych_bajtow);
	res = f_write(&FileSDCard2, "Czas [s]: ", 11, &zapisanych_bajtow);
	res = f_write(&FileSDCard2, "\r\n" , 2, &zapisanych_bajtow);			
	
	res = f_write(&FileSDCard1, "Napiecie [V]: ", 16, &zapisanych_bajtow);
	res = f_write(&FileSDCard1, "        " , 8, &zapisanych_bajtow);
	res = f_write(&FileSDCard1, "Czas [s]: ", 11, &zapisanych_bajtow);
	res = f_write(&FileSDCard1, "\r\n" , 2, &zapisanych_bajtow);			
	
	res = f_write(&FileSDCard6, "Napiecie [V]: ", 16, &zapisanych_bajtow);
	res = f_write(&FileSDCard6, "        " , 8, &zapisanych_bajtow);
	res = f_write(&FileSDCard6, "Czas [s]: ", 11, &zapisanych_bajtow);
	res = f_write(&FileSDCard6, "\r\n" , 2, &zapisanych_bajtow);			
	
	res = f_write(&FileSDCard7, "Napiecie [V]: ", 16, &zapisanych_bajtow);
	res = f_write(&FileSDCard7, "        " , 8, &zapisanych_bajtow);
	res = f_write(&FileSDCard7, "Czas [s]: ", 11, &zapisanych_bajtow);
	res = f_write(&FileSDCard7, "\r\n" , 2, &zapisanych_bajtow);			
	
	res = f_write(&FileSDCard8, "Napiecie [V]: ", 16, &zapisanych_bajtow);
	res = f_write(&FileSDCard8, "        " , 8, &zapisanych_bajtow);
	res = f_write(&FileSDCard8, "Czas [s]: ", 11, &zapisanych_bajtow);
	res = f_write(&FileSDCard8, "\r\n" , 2, &zapisanych_bajtow);
	
	return 0;
}
