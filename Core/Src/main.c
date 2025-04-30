/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "typedef.h"
#include <stdio.h>
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
uint8_t val_old=0, val_new =0; 
char lcd_buf[25];
int32_t enc_count = 0;
uint8_t flag_dir = 0, flag_rev = 0;
uint8_t state_machine = SM_WAIT;
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
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
  */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	OLED_init();
	delay_us (10000); 
			
	FontSet(Segoe_UI_Eng_12);
  snprintf(lcd_buf, 20,"enc_count=%ld", enc_count);
  OLED_DrawStr(lcd_buf, 5, 5);	//
	OLED_UpdateScreen();

  TIM_MS_DELAY_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    for (uint16_t count = 0; count < 2000; count++)
    { 
      delay_us (500); //delay 5 ms
      val_new = read_gray_code_from_encoder();
      switch (state_machine)
        case SM_WAIT:
          if (val_new != val_old)
	        { 
            state_machine = SM_FIRST;
            if (val_new == 0x01)
            {
              flag_dir = 1;
            }
            else
            {
              if (val_new == 0x02)
              {
                flag_rev = 1;
              }
              else
              {
                state_machine = SM_WAIT;
              }
            }
            val_old = val_new;
            delay_ms (20);
          }
          break;

        case SM_FIRST: 
        {
          if (val_new != val_old)
	        { 
            delay_ms (20);
          }
          else
          {
            delay_ms (10);
          }
          break;        
        }
        case 0x02: 
        {
          flag_rev = 1;
          break;
        }
        case 0x0D: 
        {
          if (flag_dir == 1)
          {
            enc_count--;
            OLED_Clear(NONE_INVERTED);
            snprintf(lcd_buf, 25,"-cnt=%ld, %x, %x, %x", enc_count, val_new, val_old, val_old | val_new);
            OLED_DrawStr(lcd_buf, 15, 25);
            OLED_UpdateScreen();
            flag_dir = 0; 
          }

          if (flag_rev == 1)
          {
            enc_count++;
            OLED_Clear(NONE_INVERTED);
            snprintf(lcd_buf, 25,"+cnt=%ld, %x, %x, %x", enc_count, val_new, val_old, val_old | val_new);
            OLED_DrawStr(lcd_buf, 15, 25);
            OLED_UpdateScreen(); 
            flag_rev = 0;    
            break; 
          }   
        }

        default:
          break;
          
      }
      val_old = val_new<<2; 
      if (count == 1999)
      { TOOGLE_LED_RED(); }
    }

    /* USER CODE END WHILE */

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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(72000000);
  LL_SetSystemCoreClock(72000000);
}

/* USER CODE BEGIN 4 */
 void TIM_DELAY_MS_Callback (void)
 {
    state_machine++
    if (state_machine > SM_SECOND)
      state_machine = SM_WAIT;
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
