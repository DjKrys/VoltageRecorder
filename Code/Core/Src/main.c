/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

#include "SD_card.h"
#include "OLED.h"
#include "AD7193.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define DIVIDER_4x 4.0
#define DIVIDER_16x 16.18

int stateB1 = 0;
int stateB2 = 0;
int stateB3 = 0;
int stateB4 = 0;
int button = 0;
int32_t next = 0;
float time = 0.0;

int Channel1, Channel2, Channel3, Channel4, Channel5, Channel6, Channel7, Channel8 = 0;
float divider1, divider2, divider3, divider4, divider5, divider6, divider7, divider8 = 0.0;
float Pomiar1, Pomiar2, Pomiar3, Pomiar4, Pomiar5, Pomiar6, Pomiar7, Pomiar8 = 0.0;

int RECORD_BOOL = 0; // 1 - Nagrywanie, 0 - Brak nagrywania
int SD_Error = 1;
int Filter = 2;			// 1 - Filtr z tlumieniem 50Hz, 2 - Filtr bez tlumienia
uint32_t PomiarADC;
float Vsense;
int ProcentBattery;

const float SupplyVoltage = 3.36; // [Volts]
const float ADCResolution = 4096.0;
const float ADCBatteryRange = 0.5;
const float ADCBatteryMin = 2.6;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	
	////////////OLED////////////
	OLED_Init();
	
	OLED_Full_screen();
	HAL_Delay(1000);
	OLED_Clear_screen();
	OLED_Power_on();
	HAL_Delay(1000);
	OLED_Start();
	HAL_Delay(3000);

///////////Przyciski
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	//////////VBAT////////
	HAL_ADC_Start_DMA(&hadc1, &PomiarADC, 1);

////////////ADC////////////
	AppendStatusValuetoData();
	SetPGAGain(1);
	SetAveraging(96);
	SetFilterSinc3();
	SetPsuedoDifferentialInputs();
	Calibrate();
	ReadRegisterMap();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		 /* USER CODE END WHILE */
		
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 1)
			divider1 = DIVIDER_16x;
		else
			divider1 = DIVIDER_4x;
		
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 1)
			divider2 = DIVIDER_16x;
		else
			divider2 = DIVIDER_4x;
		
		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == 1)
			divider3 = DIVIDER_16x;
		else
			divider3 = DIVIDER_4x;
		
		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) == 1)
			divider4 = DIVIDER_16x;
		else
			divider4 = DIVIDER_4x;
		
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 1)
			divider5 = DIVIDER_16x;
		else
			divider5 = DIVIDER_4x;
		
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 1)
			divider6 = DIVIDER_16x;
		else
			divider6 = DIVIDER_4x;
		
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_14) == 1)
			divider7 = DIVIDER_16x;
		else
			divider7 = DIVIDER_4x;
		
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 1)
			divider8 = DIVIDER_16x;
		else
			divider8 = DIVIDER_4x;
		
		if(Channel1 == 1)
			Pomiar1 = DataToVoltage(ReadADCChannel(0) >> 8);
		if(Channel2 == 1)
			Pomiar2 = DataToVoltage(ReadADCChannel(1) >> 8);
		if(Channel3 == 1)
			Pomiar3 = DataToVoltage(ReadADCChannel(2) >> 8);
		if(Channel4 == 1)
			Pomiar4 = DataToVoltage(ReadADCChannel(3) >> 8);
		if(Channel5 == 1)
			Pomiar5 = DataToVoltage(ReadADCChannel(4) >> 8);
		if(Channel6 == 1)
			Pomiar6 = DataToVoltage(ReadADCChannel(5) >> 8);	
		if(Channel7 == 1)		
			Pomiar7 = DataToVoltage(ReadADCChannel(6) >> 8);
		if(Channel8 == 1)
			Pomiar8 = DataToVoltage(ReadADCChannel(7) >> 8);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM6)
	{
		Vsense = (SupplyVoltage*PomiarADC)/(ADCResolution);
		ProcentBattery = (int)(((Vsense-ADCBatteryMin)/ADCBatteryRange)*100);
		if(ProcentBattery < 3)
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

		OLED_Display_measurement(ProcentBattery, RECORD_BOOL, Pomiar1*divider1, Pomiar2*divider2, Pomiar3*divider3, Pomiar4*divider4, Pomiar5*divider5, Pomiar6*divider6, Pomiar7*divider7, Pomiar8*divider8);
	}
	
	if(htim->Instance == TIM7)
	{
		next++;
		write_SD(Pomiar1*divider1, Pomiar2*divider2, Pomiar3*divider3, Pomiar4*divider4, Pomiar5*divider5, Pomiar6*divider6, Pomiar7*divider7, Pomiar8*divider8, time*next);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//////B1////////////
	if(GPIO_Pin == GPIO_PIN_5)
	{
		button = 1;
		HAL_TIM_Base_Stop_IT(&htim6);
		stateB1++;
		
		if(stateB1 > 3)
			stateB1 = 1;
		
		OLED_Top(stateB1);
	}
	///////////B2//////////
	if(GPIO_Pin == GPIO_PIN_0)
	{
		button = 2;
		stateB2++;
		
		if(stateB2 > 2)
			stateB2 = 1;
				
    OLED_Select_Filter(stateB2);
	}
		
	///////////////B3//////////
	if(GPIO_Pin == GPIO_PIN_1)
	{
		button = 3;
		stateB3++;
		
		if(stateB3 > 8)
			stateB3 = 1;
				
		if(Filter == 1)
			OLED_Sampling_frequency1(stateB3);
		else if(Filter == 2)
			OLED_Sampling_frequency2(stateB3);
	}

/////////B4////////
	if(GPIO_Pin == GPIO_PIN_11)
	{
		button = 4;
		HAL_TIM_Base_Stop_IT(&htim6);
		
		stateB4++;
		
		if(stateB4 > 10)
			stateB4 = 1;
		
		OLED_ON_OFF_Channel(stateB4);

	}
	
/////////ENTER/////////
	if(GPIO_Pin == GPIO_PIN_2)
	{
		switch(button)
		{
			case 1:
				switch(stateB1)
				{
					case 1:
							if(Init_SD() != 1)
							{
								open_SD();
								write_info();
								time = ((float)(PERIOD_Tim + 1.0))/1000.0;
								RECORD_BOOL = 1;
								HAL_TIM_Base_Start_IT(&htim6);
								HAL_TIM_Base_Start_IT(&htim7);
								SD_Error = 0;
							}
							else
							{
								SD_Error = 1;
								OLED_Error();
							}
						break;
					case 2:
							if(close_SD() || SD_Error != 1)
							{
								RECORD_BOOL = 0;
								HAL_TIM_Base_Stop_IT(&htim6);
								HAL_TIM_Base_Stop_IT(&htim7);
								next = 0;
								OLED_Succesfull();
							}
							else
								OLED_Error();
						break;
					case 3:
							HAL_TIM_Base_Start_IT(&htim6);
						break;
				}
				break;
				
			case 2:
				switch(stateB2)
				{
					case 1:
							SetAveraging(96);
							Filter = 1;
							OLED_Succesfull();
						break;
					case 2:
							SetAveraging(24);
							Filter = 2;
							OLED_Succesfull();
						break;
				}
				break;
				
			case 3:
				if(Filter == 1)
				{
					switch(stateB3)
					{
						case 1:
								PERIOD_Tim = 59;
								OLED_Succesfull();
							break;
						case 2:
								PERIOD_Tim = 99;
								OLED_Succesfull();
							break;
						case 3:
								PERIOD_Tim = 249;
								OLED_Succesfull();
							break;
						case 4:
								PERIOD_Tim = 499;
								OLED_Succesfull();
							break;
						case 5:
								PERIOD_Tim = 999;
								OLED_Succesfull();
							break;
						case 6:
								PERIOD_Tim = 1999;
								OLED_Succesfull();
							break;
						case 7:
								PERIOD_Tim = 4999;
								OLED_Succesfull();
							break;
						case 8:
								PERIOD_Tim = 9999;
								OLED_Succesfull();
							break;
					}
					MX_TIM7_Init();
				}
				if (Filter == 2)
				{
					switch(stateB3)
					{
						case 1:
								PERIOD_Tim = 14;
								OLED_Succesfull();
							break;
						case 2:
								PERIOD_Tim = 49;
								OLED_Succesfull();
							break;
						case 3:
								PERIOD_Tim = 149;
								OLED_Succesfull();
							break;
						case 4:
								PERIOD_Tim = 249;
								OLED_Succesfull();
							break;
						case 5:
								PERIOD_Tim = 499;
								OLED_Succesfull();
							break;
						case 6:
								PERIOD_Tim = 999;
								OLED_Succesfull();
							break;
						case 7:
								PERIOD_Tim = 1999;
								OLED_Succesfull();
							break;
						case 8:
								PERIOD_Tim = 4999;
								OLED_Succesfull();
							break;
					}
					MX_TIM7_Init();
				}
				break;
			
			case 4:	
				switch(stateB4)
				{
					case 1:
							HAL_TIM_Base_Stop_IT(&htim6);
							OLED_Clear_screen();
						break;
					case 2:
							if(Channel1 == 0)
							{
								Channel1 = 1;
								OLED_Activate();
							}
							else
							{
								Channel1 = 0;
								OLED_Deactivate();
							}
						break;
					case 3:
							if(Channel2 == 0)
							{
								Channel2 = 1;
								OLED_Activate();
							}
							else
							{
								Channel2 = 0;
								OLED_Deactivate();
							}
						break;
					case 4:
							if(Channel3 == 0)
							{
								Channel3 = 1;
								OLED_Activate();
							}
							else
							{
								Channel3 = 0;
								OLED_Deactivate();
							}
						break;
					case 5:
							if(Channel4 == 0)
							{
								Channel4 = 1;
								OLED_Activate();
							}
							else
							{
								Channel4 = 0;
								OLED_Deactivate();
							}
						break;
					case 6:
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
						break;
					case 7:
							if(Channel5 == 0)
							{
								Channel5 = 1;
								OLED_Activate();
							}
							else
							{
								Channel5 = 0;
								OLED_Deactivate();
							}
						break;
					case 8:
							if(Channel6 == 0)
							{
								Channel6 = 1;
								OLED_Activate();
							}
							else
							{
								Channel6 = 0;
								OLED_Deactivate();
							}
						break;
					case 9:
							if(Channel7 == 0)
							{
								Channel7 = 1;
								OLED_Activate();
							}
							else
							{
								Channel7 = 0;
								OLED_Deactivate();
							}
						break;
					case 10:
							if(Channel8 == 0)
							{
								Channel8 = 1;
								OLED_Activate();
							}
							else
							{
								Channel8 = 0;
								OLED_Deactivate();
							}
						break;
				}
				break;
		}
	}
}

void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
}

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
