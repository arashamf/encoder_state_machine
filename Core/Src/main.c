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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "OLED.h"
#include "typedef.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_CNT_TIMEOUT   15
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t val_old=0, val_new =0; 
char lcd_buf[25];
int32_t enc_count = 0; //счётчик щелчков энкодера
uint8_t flag_dir = 0, flag_rev = 0; //флаг направления поворота
uint8_t state_machine = SM_WAIT; //стадия конечного автомата
uint8_t flag_end_delay = 0; //флаг окончания таймаута
uint32_t ms_delay = 10; //продолжительность таймаута
uint8_t cnt_timeout = 0; //количество таймаутов ожидания изменения показаний энкодера
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
  MX_USART1_UART_Init();
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
    for (uint32_t count = 0; count < 500000; count++)
    { 
      {
        val_new = read_gray_code_from_encoder(); //чтение данных энкодера

        switch (state_machine) //проверка номера стадии конечного автомата
        {
          case SM_WAIT: //стадия ожидания поворота энкодера
            if (flag_end_delay == 0) //если таймаут истёк
            {
              if (val_new != val_old) //если показания энкодера изменились
	            { 
                if (val_new == PULSE_STATE_FIRST_REV) 
                {
                  state_machine = SM_FIRST; //переход на следующую стадию автомата
                  flag_rev = 1; //флаг реверсного поворота энкодера
                    #ifdef __USE_DBG
		                  sprintf (DBG_buffer,  "1WAIT,flag_rev=%u\r\n", val_new);
		                  DBG_PutString(DBG_buffer);
	                  #endif
                }
                else
                {
                  if (val_new == PULSE_STATE_FIRST_DIR)
                  {
                    state_machine = SM_FIRST; //переход на следующую стадию автомата
                    flag_dir = 1; //флаг прямого поворота энкодера
                    #ifdef __USE_DBG
		                  sprintf (DBG_buffer,  "1WAIT,flag_dir=%u\r\n", val_new);
		                  DBG_PutString(DBG_buffer);
	                  #endif
                  }
                  else 
                  {
                    #ifdef __USE_DBG
                    	sprintf (DBG_buffer,  "1RESET,new=%u,old=%u\r\n",  val_new, val_old);
		                  DBG_PutString(DBG_buffer);
	                  #endif
                    flag_rev = flag_dir = 0; //сброс флагов поворота энкодера
                    state_machine = SM_RESET;  //переход в начальную стадию конечного автомата
                  }
                }
                cnt_timeout = 0; //сброс счётчика таймаутов
                val_old = val_new; //сохранение текущего показания энкодера
                delay_ms (ms_delay); //начало таймаута
              }
            }
            break;

         case SM_FIRST: //стадия средней части импульса энкодера
            if (flag_end_delay == 0) //если таймаут истёк
            {
              if (val_new == val_old)
	            { 
                cnt_timeout++;
                if (cnt_timeout>MAX_CNT_TIMEOUT)
                {
                  cnt_timeout = 0;  //сброс счётчика таймаутов
                  flag_rev = flag_dir = 0; //сброс флагов поворота энкодера
                  state_machine = SM_RESET;
                }
                else
                { 
                  delay_ms (ms_delay); //ожидание изменений показаний энкодера 
                }
              }       
              else
              { 
                #ifdef __USE_DBG
                  sprintf (DBG_buffer,  "2FIRST,val_new=%u\r\n",  val_new);
		              DBG_PutString(DBG_buffer);
	              #endif
                cnt_timeout = 0; //сброс счётчика таймаутов             
                val_old = val_new; //сохранение текущего показания энкодера
                if (val_new == PULSE_STATE_SECOND)
                {
                  state_machine = SM_NULL; //переход на следующую стадию автомата
                }
                else
                {
                  flag_rev = flag_dir = 0;
                  state_machine = SM_RESET;
                }
                delay_ms (ms_delay);
              }
            }
            break;        

          case SM_NULL: //стадия последней части импульса энкодера
            if (flag_end_delay == 0) //если таймаут истёк
            {
              if (val_new == val_old) //если показания энкодера не изменилось
	            { 
                if (cnt_timeout > MAX_CNT_TIMEOUT) //если количество таймаутов ожидания изменения показаний энкодера превышено
                {
                  cnt_timeout = 0;  //сброс счётчика таймаутов
                  flag_rev = flag_dir = 0; //сброс флагов поворота энкодера
                  state_machine = SM_RESET; //переход в начальную стадию конечного автомата
                }
                else
                { 
                  delay_ms (ms_delay); //начало таймаута ожидания изменений показаний энкодера 
                }
              }       
              else
              { 
                #ifdef __USE_DBG
                  sprintf (DBG_buffer,  "3NULL,val_new=%u\r\n",  val_new);
		              DBG_PutString(DBG_buffer);
	              #endif

                if (val_new ==  PULSE_STATE_LAST_REV)
                {
                  if (flag_rev == 1) //если установлен флаг реверсного поворота энкодера
                  { 
                    state_machine = SM_LAST;  //переход на следующую стадию автомата
                    delay_ms (ms_delay); //начало таймаута
                  }              
                }
                else
                {
                  if (val_new == PULSE_STATE_LAST_DIR) 
                  {
                    if (flag_dir == 1) //если установлен флаг прямого поворота энкодера
                    {  
                      state_machine = SM_LAST;  //переход на следующую стадию автомата
                      delay_ms (ms_delay); //начало таймаута
                    }
                  }
                  else
                 {
                    flag_rev = flag_dir = 0; //сброс флагов поворота энкодера
                    state_machine = SM_RESET;  //переход в начальную стадию конечного автомата
                  }
                }
                val_old = val_new; //сохранение текущего показания энкодера
                cnt_timeout = 0; //сброс счётчика таймаутов
              }
            }
            break;

          case  SM_LAST: //проверка стадии импульса энкодера по умолчанию
            if (flag_end_delay == 0) //если таймаут истёк
            {
              if (val_new == val_old) //если показания энкодера не изменилось
	            { 
                if (cnt_timeout > MAX_CNT_TIMEOUT) //если количество таймаутов ожидания изменения показаний энкодера превышено
                {
                  cnt_timeout = 0;  //сброс счётчика таймаутов
                  flag_rev = flag_dir = 0; 
                  state_machine = SM_RESET; //переход к начальной стадии конечного автомата
                }
                else
                { 
                  delay_ms (ms_delay); //таймаут ожидания изменений показаний энкодера 
                }
              }       
              else
              {
                #ifdef __USE_DBG
                  sprintf (DBG_buffer,  " 4LAST,val_new=%u\r\n",  val_new);
		              DBG_PutString(DBG_buffer);
	              #endif
                if (flag_rev == 1) //если установлен флаг реверсного поворота энкодера
                {
                  enc_count--;  
                  OLED_Clear(NONE_INVERTED);
                  snprintf(lcd_buf, 25,"-cnt=%ld, %x, %x, %x", enc_count, val_new, val_old, val_old | val_new);
                  OLED_DrawStr(lcd_buf, 15, 25);
                  OLED_UpdateScreen();
                }
                else
                {
                  if (flag_dir == 1) //если установлен флаг прямого поворота энкодера
                  {
                    enc_count++;  
                    OLED_Clear(NONE_INVERTED);
                    snprintf(lcd_buf, 25,"+cnt=%ld, %x, %x, %x", enc_count, val_new, val_old, val_old | val_new);
                    OLED_DrawStr(lcd_buf, 15, 25);
                    OLED_UpdateScreen(); 
                  } 
                } 
                flag_rev = flag_dir = 0; //сброс флагов поворота энкодера
                val_old = val_new; //сохранение текущего показания энкодера
                cnt_timeout = 0; //сброс счётчика таймаутов
                state_machine = SM_WAIT; //переход к начальной стадии конечного автомата
              }            
            } 
            break;  

          case  SM_RESET: //проверка стадии импульса энкодера по умолчанию
            if (flag_end_delay == 0) //если таймаут истёк
            {
              if (val_new != val_old) //если показания энкодера изменились
              {
                if (val_new == PULSE_STATE_DEFAULT) //если энкодер в состоянии по умолчанию (поворот закончен)
	              { 
                  state_machine = SM_WAIT; //переход к начальной стадии конечного автомата
                  #ifdef __USE_DBG
                    sprintf (DBG_buffer,  "END_RESET,new=%u,old=%u\r\n",  val_new, val_old);
		                DBG_PutString(DBG_buffer);
	                #endif
                }
                else
                {
                  #ifdef __USE_DBG
                    sprintf (DBG_buffer,  "CONT_RESET,new=%u,old=%u\r\n",  val_new, val_old);
		                DBG_PutString(DBG_buffer);
	                #endif
                }
                val_old = val_new; //сохранение текущего показания энкодера
              }               
              delay_ms (ms_delay); //начало таймаута
            }
            break;

          default:
            break;         
        }
      }   
    }
    TOOGLE_LED_RED(); 
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
    flag_end_delay = 0;
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
