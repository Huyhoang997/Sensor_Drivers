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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
  /* USER CODE BEGIN 2 */
	
	/* ================== CLOCK_CONFIG ================== */
	FLASH->ACR |= FLASH_ACR_LATENCY_2;
	RCC->CR &= ~(1 << 24U);					//PLLON : pll_off
	RCC->CR |= (1 << 0U); 					//HSION
	while (!(RCC->CR & (1 << 1U))) 	//Wait HSIRDY_flag
	
	RCC->CFGR &= ~(7 << 24U);				//MCO: clear
	RCC->CFGR |= (1 << 26U);				//MCO: choose sys_clock
	RCC->CFGR |= (15 << 18U);				//PLLMUL: pll x 16
	RCC->CFGR &= ~(1 << 16U);				//PLLSRC: HSI/2
	RCC->CFGR &= ~(3 << 0U); 				//SW: sys clock switch
	RCC->CFGR |= (1 << 1U);					//SW: clock switch
	
	RCC->CR |= (1 << 24U);					//PLLON: pll_on
	while (!(RCC->CR & (1 << 25U))) //wait PLLRDY_flag
	
	/* ============ CLOCK_ENABLE_PERIPHERAL ============= */
	RCC->APB1ENR |= (1 << 0U);			//TIM2EN
	RCC->APB2ENR |= (1 << 3U);			//GPIOBEN
	RCC->APB2ENR |= (1 << 2U);			//GPIOBEN
	
	/* ================ TIMER_CONFIG ==================== */
	uint16_t servo_angle[3] = {1003, 1500, 2000}; 
	TIM2->PSC = 0x3F;							//PSC: 63
	TIM2->ARR = 0x4E20;							//ARR: 20000
		//TIM2->CCR1 = servo_angle[2];						//CCR1 = 2000

	
	/* ================= PWM_CONFIG ===================== */
	TIM2->CCMR1 |= (1 << 3U);				//ENABLE_preload
	TIM2->CCMR1 &= ~(7 << 4U);			//OC1M: clear_mode
	TIM2->CCMR1 |= (6 << 4U);				//OC1M: pwm_mode_1
	TIM2->CCER 	|= (1 << 0U);				//CC1E: C/C1 output_enable	
	//TIM2->BDTR |= (1 << 15U);				//CONFIG_MOE_MODE (TIM1 & TIM8)!!!!
	TIM2->CR1 |= (1 << 0U);					//CEN: enable_cnt
	
	/* ================= GPIO_CONFIG ==================== */
	GPIOA->CRL &= ~(15 << 0U);			//CRH: clear mode_gpio
	GPIOA->CRL |= (11 << 0U);				//CRH: enable_mode_pwm
	
	GPIOB->CRL &= ~(15 << 12U);			//CRL: clear_mode_gpio
	GPIOB->CRL |= (3 << 12U);				//CRL: enable_mode_gpio
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		for (int i = 0 ; i < 3; i++) {
			TIM2->CCR1 = servo_angle[i];
			HAL_Delay(1000);
		}
		GPIOB->ODR |= (1 << 3U);
		if(TIM2->CNT == TIM2->ARR) {
			GPIOB->ODR &= ~(1 << 3U);
			HAL_Delay(500);
		}
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
#ifdef USE_FULL_ASSERT
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
