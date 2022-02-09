/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 10 */

#include "stm32f4xx_hal.h"
#include <stdio.h>

int msec_cnt = 0, sec_cnt=0;
TIM_HandleTypeDef hTimBase;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM7)
	{
		if((++msec_cnt) >= 999)
		{
			msec_cnt = 0;
			sec_cnt++;
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0)
	{
		if(hTimBase.Instance->CR1 & 0x01)
		{
			HAL_TIM_Base_Stop(&hTimBase); // TIMx stop
		}
		else
		{
			HAL_TIM_Base_Start(&hTimBase); // TIMx restart
		}
	}
	if(GPIO_Pin == GPIO_PIN_1)
	{
		HAL_TIM_Base_Stop(&hTimBase); // stop
		__HAL_TIM_SetCounter(&hTimBase, 0); // reset
		sec_cnt = 0; // PuTTY 0
	}
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
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_TIM7_CLK_ENABLE();
	
	HAL_Init();
	UARTx_Init();
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // GPIO_MODE_IT_RISING;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	hTimBase.Instance = TIM7;
	hTimBase.Init.Prescaler = 15; // 16-1; // (16MHz --> 1MHz(1us))
	hTimBase.Init.Period = 999; // 1000-1;
	hTimBase.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&hTimBase);
	
	HAL_NVIC_SetPriority(TIM7_IRQn, 7, 0); /* Set interrupt Priority */
	HAL_NVIC_SetPriority(EXTI0_IRQn, 8, 0); /* Set interrupt Priority */
	HAL_NVIC_SetPriority(EXTI1_IRQn, 9, 0); /* Set interrupt Priority */
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	HAL_TIM_Base_Start_IT(&hTimBase);
	
	while(1)
	{
		if((hTimBase.Instance -> CR1 & 0x01))
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		}
		
		printf("StopWatch : %4d(sec)\r", sec_cnt);
		HAL_Delay(1000);
	}
}
