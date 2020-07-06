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
#include "dma.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Discrets.h"
#include "Fifo.h"
#include "Wrap_Flash.h"
#include "Internal_Flash.h"
#include "Commands.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define UART_FIFO_TX_SIZE                                1024
#define UART_FIFO_RX_SIZE                                1024
#define TASK_BEGIN_CRITICAL_SECTION()					 __disable_irq()
#define TASK_END_CRITICAL_SECTION()					 	 __enable_irq()
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
Fifo_t UartTx, UartRx;
uint8_t UartTxBuffer[UART_FIFO_TX_SIZE];
uint8_t UartRxBuffer[UART_FIFO_RX_SIZE];
uint8_t UartRxSymbol;
extern volatile bool IsStartMeasurement;
volatile uint32_t CounterSemaphore[DISCRETS_OUTPUTS_PINS];
volatile uint32_t CounterStateSemaphore[DISCRETS_OUTPUTS_PINS];
volatile uint8_t DiscretLineState[DISCRETS_OUTPUTS_PINS];
extern volatile FlashMapData_t GlobalStorage;
//volatile uint64_t UserQuantityStateLine[DISCRETS_OUTPUTS_PINS];
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
  MX_IWDG_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  InitFifo();
  FlashInit();
  FlashStorageInit();
  StartParser();
  CommandStartMeasurement();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_IWDG_Refresh(&hiwdg);
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(htim);

	/* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
	 */
	if(htim->Instance == TIM2)
	{
		for (uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
			DiscretsHandler(NumberLine);
	}
	else if(htim->Instance == TIM3)
	{
		ResetParser();
		HAL_UART_Receive_IT(&huart2, &UartRxSymbol, 1);
	}
	else if(htim->Instance == TIM4)
	{
		CommandCheckImpulseChannel(NUMBER_DISCRETS_LINE_ALL);
	}
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
	 */

	if(huart->Instance == USART2)
	{
		ParseSymbols(UartRxSymbol);
		HAL_UART_Receive_IT(&huart2, &UartRxSymbol, 1);
	}
}

/*!
 * @brief Start parser timers for work
 */
void StartParser(void)
{
	HAL_UART_Receive_IT(&huart2, &UartRxSymbol, 1);
	HAL_TIM_Base_Start_IT(&htim3);
}

/*!
 * Initial Virtual COM Port buffers
 */
void InitFifo(void)
{
	FifoInit(&UartTx, UartTxBuffer, UART_FIFO_TX_SIZE);
	FifoInit(&UartRx, UartRxBuffer, UART_FIFO_RX_SIZE);
}

/*!
 * Send to Uart with DMA
 */
void UartSend(void)
{
	uint8_t TxMessage[UART_FIFO_TX_SIZE];
	uint16_t Size = UartTx.Count;

	FifoGetBuf(&UartTx, TxMessage, Size);

	HAL_UART_Transmit_DMA(&huart2, TxMessage, Size);
}

/*!
 * Discret's Handler
 */
void DiscretsHandler(uint8_t NumberLine)
{
	if(!GlobalStorage.DiscretLine[NumberLine].IsDiscretLineEnable)
		CounterSemaphore[NumberLine] = 0;
	else
		CounterSemaphore[NumberLine]++;

	if(!GlobalStorage.DiscretLine[NumberLine].IsDiscretLineEnable)
		return;
	else if(CounterStateSemaphore[NumberLine] != GlobalStorage.DiscretLine[NumberLine].QuantityStateLine)
	{
		if((CounterSemaphore[NumberLine] == GlobalStorage.DiscretLine[NumberLine].DelayStep1) &&
			DiscretLineState[NumberLine])
		{
			CounterSemaphore[NumberLine] = 0;
			DiscretLineState[NumberLine] = 0;
			CounterStateSemaphore[NumberLine]++;
			DiscretsToggleLines(NumberLine);
		}
		else if((CounterSemaphore[NumberLine] == GlobalStorage.DiscretLine[NumberLine].DelayStep0) &&
				!DiscretLineState[NumberLine])
		{
			CounterSemaphore[NumberLine] = 0;
			DiscretLineState[NumberLine] = 1;
			DiscretsToggleLines(NumberLine);
		}
	}
	else
		CounterSemaphore[NumberLine] = 0;
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
