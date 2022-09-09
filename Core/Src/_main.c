/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#define __DEBUG__ 4

#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "ringbuf/ringbuf.h"
#include "console/console.h"
#include "blink/blink.h"


void SystemClock_Config(void);

/******************************************************
 * LED driver
 *****************************************************/
static blink_t blink;

void blink_write(uint8_t level)
{
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, level);
}

/******************************************************
 * Ring buffer driver
 *****************************************************/

#define RINGBUF_LEN 128
RINGBUF ringbuf;
uint8_t ringbuf_buffer[RINGBUF_LEN];

void ringbuf_intterupt_active(bool active)
{
	if(active)
	{
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
	else
	{
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
}

/******************************************************
 * Console driver
 *****************************************************/

int console_get_char_callback()
{
	if(RINGBUF_Available(&ringbuf))
	{
		uint8_t c;
		RINGBUF_Get(&ringbuf, &c);
		return c;
	}
	else
	{
		return -1;
	}
}

void console_put_callback(char *data, uint16_t len)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)data, len, 1000);
}

/******************************************************
 * Uart callbacks
 *****************************************************/

static uint8_t uart2_read_byte;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart2)
	{
		RINGBUF_Put(&ringbuf, uart2_read_byte);
		HAL_UART_Receive_DMA(huart, &uart2_read_byte, 1);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart2)
	{
		HAL_UART_Receive_DMA(huart, &uart2_read_byte, 1);
	}
}

/******************************************************
 * Console callbacks
 *****************************************************/
void console_message_callback(char *message)
{
	if(__check_cmd("hello"))
	{
		debug("hi there");
	}
	else if(__check_cmd("blink "))
	{
		int interval;
		if(sscanf(__param_pos("blink "), "%d", &interval) == 1)
		{
			if(interval==0)
			{
				debug("Invalid parameter");
			}
			else
			{
				debug("Start blink LED interval: %d", interval);
				blink_init(&blink, blink_write, HAL_GetTick, interval);
			}
		}
		else
		{
			error("Please input parameter");
		}
	}
	else
		error("Unknown command");
}

int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  RINGBUF_Init(&ringbuf,
		  	  ringbuf_buffer,
			  RINGBUF_LEN,
			  ringbuf_intterupt_active);
  console_init(console_message_callback,
		  	  console_get_char_callback,
			  console_put_callback,
			  true);

  HAL_UART_Receive_DMA(&huart2, &uart2_read_byte, 1);
  blink_init(&blink, blink_write, HAL_GetTick, 1000);
  while (1)
  {
	  console_process();
	  blink_process(&blink);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
