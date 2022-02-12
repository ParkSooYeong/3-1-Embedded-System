/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 10 */

#include "stm32f4xx_hal.h"
#include <stdio.h>

void GPIOx_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

TIM_HandleTypeDef htim2;

void TIMx_Init()
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0; //15;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 9999; // 0 ~ 9999;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim2);
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2; //INTERNAL;
	sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_INVERTED;
	sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
	sClockSourceConfig.ClockFilter = 15; // 0:chattering
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
	
	HAL_TIM_Base_Start(&htim2);
}

UART_HandleTypeDef huart;

void UARTx_Init()
{
	__HAL_RCC_USART1_CLK_ENABLE();
	
	huart.Instance = USART1;
	huart.Init.BaudRate = 115200;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&huart);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10; // PA9 ------> USART1_TX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // PA10 ------> USART1_RX 
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, 10); 
	return ch;
}

int main()
{
	int val;
	
	HAL_Init();
	UARTx_Init();
	GPIOx_Init();
	TIMx_Init();
	
	while(1)
	{
		val = __HAL_TIM_GetCounter(&htim2); //val = TIM2->CNT;
		printf("COUNTER VAL : %d\r\n", val);
		HAL_Delay(200);
	}
}
