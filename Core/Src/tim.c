/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "main.h"

#define   TIM_US_DELAY                TIM4

#define   TIM_MS_DELAY                TIM3
#define   TIM_MS_DELAY_APB_BIT        LL_APB1_GRP1_PERIPH_TIM3
#define  	TIM_MS_DELAY_IRQn           TIM3_IRQn
#define		TIM_MS_DELAY_IRQHandler			TIM3_IRQHandler
/* USER CODE END 0 */

/* TIM4 init function */
void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  TIM_InitStruct.Prescaler = 71;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1000;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM4, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM4);
  LL_TIM_SetClockSource(TIM4, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM4, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM4);
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/* USER CODE BEGIN 1 */
//---------------------------------------------------------------------------------------//
void TIM_MS_DELAY_Init(void)
{
  LL_APB1_GRP1_EnableClock(TIM_MS_DELAY_APB_BIT );

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  TIM_InitStruct.Prescaler = 7200-1;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1000;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM_MS_DELAY, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM_MS_DELAY);
  LL_TIM_SetClockSource(TIM_MS_DELAY, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM_MS_DELAY, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM_MS_DELAY);

  LL_TIM_ClearFlag_UPDATE(TIM_MS_DELAY);
	LL_TIM_EnableIT_UPDATE(TIM_MS_DELAY);
	NVIC_SetPriority(TIM_MS_DELAY_IRQn, 4);
  NVIC_EnableIRQ(TIM_MS_DELAY_IRQn);
}

//---------------------------------------------------------------------------------------//
void delay_ms (uint16_t delay)
{
  LL_TIM_SetAutoReload(TIM_MS_DELAY, 2*delay); 
	LL_TIM_SetCounter(TIM_MS_DELAY, 0); 
	LL_TIM_ClearFlag_UPDATE(TIM_MS_DELAY);
	LL_TIM_EnableCounter(TIM_MS_DELAY); 
  flag_end_delay = 1;
}

//---------------------------------------------------------------------------------------//
void TIM_MS_DELAY_IRQHandler(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM_MS_DELAY) == SET)
	{	
		LL_TIM_ClearFlag_UPDATE (TIM_MS_DELAY); 
		LL_TIM_DisableCounter(TIM_MS_DELAY); 
    TIM_DELAY_MS_Callback ();
	}
}

//---------------------------------------------------------------------------------------//
void delay_us(uint16_t delay)
{
  LL_TIM_SetAutoReload(TIM_US_DELAY, delay); 
	LL_TIM_ClearFlag_UPDATE(TIM_US_DELAY);
	LL_TIM_SetCounter(TIM_US_DELAY, 0); 
	LL_TIM_EnableCounter(TIM_US_DELAY); 
	while (LL_TIM_IsActiveFlag_UPDATE(TIM_US_DELAY) == 0) {} 
	LL_TIM_DisableCounter(TIM_US_DELAY);
}
/* USER CODE END 1 */
