/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
//Private typedef-----------------------------------------------------------------------//
#define LED_RED(x) ((x)? (LL_GPIO_SetOutputPin (LED_GPIO_Port, LED_Pin)) : (LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin)))
#define TOOGLE_LED_RED() (LED_RED (!(LL_GPIO_IsOutputPinSet(LED_GPIO_Port, LED_Pin))))

#define LCD_RST(x) ((x)? (LL_GPIO_SetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin)) : (LL_GPIO_ResetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin)));  
#define LCD_DC(x) ((x)? (LL_GPIO_SetOutputPin(LCD_DC_GPIO_Port, LCD_DC_Pin)) : (LL_GPIO_ResetOutputPin(LCD_DC_GPIO_Port, LCD_DC_Pin)));  
#define LCD_CS(x) ((x)? (LL_GPIO_SetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin)) : (LL_GPIO_ResetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin)));  
 
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
uint8_t read_gray_code_from_encoder(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

