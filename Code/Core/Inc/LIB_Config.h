/**
  ******************************************************************************
  * @file    LIB_Config.h
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief     This file provides configurations for low layer hardware libraries.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USE_LIB_CONFIG_H_
#define _USE_LIB_CONFIG_H_
//Macro Definition

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "MacroAndConst.h"
#include "SSD1306.h"
#include "Fonts.h"

extern SPI_HandleTypeDef hspi3;

#define SH1106

#define INTERFACE_4WIRE_SPI     //4-wire SPI 


#define SSD1306_CS_PIN          GPIO_PIN_6
#define SSD1306_RES_PIN         GPIO_PIN_8
#define SSD1306_DC_PIN          GPIO_PIN_7

#define SSD1306_CS_GPIO         GPIOB
#define SSD1306_RES_GPIO        GPIOB
#define SSD1306_DC_GPIO         GPIOB 

#define __SSD1306_CS_SET()      HAL_GPIO_WritePin(SSD1306_CS_GPIO, SSD1306_CS_PIN, GPIO_PIN_SET)
#define __SSD1306_CS_CLR()      HAL_GPIO_WritePin(SSD1306_CS_GPIO, SSD1306_CS_PIN, GPIO_PIN_RESET)

#define __SSD1306_RES_SET()     HAL_GPIO_WritePin(SSD1306_RES_GPIO, SSD1306_RES_PIN, GPIO_PIN_SET)
#define __SSD1306_RES_CLR()     HAL_GPIO_WritePin(SSD1306_RES_GPIO, SSD1306_RES_PIN, GPIO_PIN_RESET)

#define __SSD1306_DC_SET()      HAL_GPIO_WritePin(SSD1306_DC_GPIO, SSD1306_DC_PIN, GPIO_PIN_SET)
#define __SSD1306_DC_CLR()      HAL_GPIO_WritePin(SSD1306_DC_GPIO, SSD1306_DC_PIN, GPIO_PIN_RESET)

#define SSD1331_SCLK_PIN        GPIO_PIN_3
#define SSD1331_SDIN_PIN        GPIO_PIN_5
      
#define SSD1331_SCLK_GPIO       GPIOB
#define SSD1331_SDIN_GPIO       GPIOB

#define __SSD1306_CLK_SET()     GPIO_WriteBit(SSD1306_CLK_GPIO, SSD1306_CLK_PIN, Bit_SET)
#define __SSD1306_CLK_CLR()     GPIO_WriteBit(SSD1306_CLK_GPIO, SSD1306_CLK_PIN, Bit_RESET)

#define __SSD1306_WRITE_BYTE(DATA)  while(HAL_SPI_Transmit(&hspi3,DATA,1,0x10) != HAL_OK)


#endif

/*-------------------------------END OF FILE-------------------------------*/

