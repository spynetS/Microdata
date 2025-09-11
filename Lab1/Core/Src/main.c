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
#include <stdint.h>

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int is_blue_button_pressed(){
	uint32_t reg_read = GPIOC->IDR;
	// we look at the first bit in the reg_variable
	return reg_read & (0x01 << 13);
}


uint8_t die_value = -1;
int is_rolling = 0;


void set_led_dice(int die_value){
  HAL_GPIO_WritePin(GPIOA,DI_A_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA,DI_B_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA,DI_C_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA,DI_D_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA,DI_E_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA,DI_F_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA,DI_G_Pin, GPIO_PIN_RESET);

  if(!is_rolling){
  switch(die_value){
    case 0:
      HAL_GPIO_WritePin(GPIOA,DI_D_Pin, GPIO_PIN_SET);
      break;
    case 1:
      HAL_GPIO_WritePin(GPIOA,DI_E_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_C_Pin, GPIO_PIN_SET);
      break;
    case 2:
      HAL_GPIO_WritePin(GPIOA,DI_D_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_E_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_C_Pin, GPIO_PIN_SET);
      break;
    case 3:
      HAL_GPIO_WritePin(GPIOA,DI_E_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_C_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_A_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_G_Pin, GPIO_PIN_SET);
      break;
    case 4:
      HAL_GPIO_WritePin(GPIOA,DI_D_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_E_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_C_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_A_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_G_Pin, GPIO_PIN_SET);

      break;
    case 5:
      HAL_GPIO_WritePin(GPIOA,DI_E_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_C_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_A_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_B_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_F_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA,DI_G_Pin, GPIO_PIN_SET);
      break;
  }
  }
}


const uint16_t sseg[10] = {0x0, 0x06, 0x09b, 0x08f,0x0c6,0x0cd,0x0dd,0x07,0x0df,0x0cf};
const uint16_t sseg_err = 0x1ac;

void put_on_sseg(uint8_t dec_nbr){
  GPIO_TypeDef* gpio = GPIOC;
  for(int i =0; i < 9; i ++){
	  GPIOC->BRR = 0x01 << i;
  }
  GPIOC->BSRR = sseg[dec_nbr+1];
}

int main(void)
{
   HAL_Init();
   SystemClock_Config();
   MX_GPIO_Init();

  int pressed = 0;
  while (1)
  {
  	put_on_sseg(die_value);
    pressed = is_blue_button_pressed();
    if(pressed){
    	is_rolling = 1;
    	die_value = (die_value + 1) % 6;
    }
    else{
    	is_rolling = 0;
    }
    set_led_dice(die_value);

    HAL_Delay(1);
  }
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SSEG_A_Pin|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DI_A_Pin|DI_B_Pin|SMPS_EN_Pin|SMPS_V1_Pin
                          |SMPS_SW_Pin|DI_C_Pin|DI_D_Pin|DI_E_Pin
                          |DI_F_Pin|DI_G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SSEG_A_Pin PC1 PC2 PC3
                           PC4 PC6 PC7 PC8
                           PC9 */
  GPIO_InitStruct.Pin = SSEG_A_Pin|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : DI_A_Pin DI_B_Pin SMPS_EN_Pin SMPS_V1_Pin
                           SMPS_SW_Pin DI_C_Pin DI_D_Pin DI_E_Pin
                           DI_F_Pin DI_G_Pin */
  GPIO_InitStruct.Pin = DI_A_Pin|DI_B_Pin|SMPS_EN_Pin|SMPS_V1_Pin
                          |SMPS_SW_Pin|DI_C_Pin|DI_D_Pin|DI_E_Pin
                          |DI_F_Pin|DI_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : USART_TX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(USART_TX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SMPS_PG_Pin */
  GPIO_InitStruct.Pin = SMPS_PG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SMPS_PG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD4_Pin */
  GPIO_InitStruct.Pin = LD4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
