/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 9 */

#include "stm32f4xx_hal.h"
#include <stdio.h>

int COUNT = 0; // ????? ? ? ??????? ??? ?? ????.

GPIO_InitTypeDef GPIO_InitStruct;
void GPIOx_Init() 
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//PA9 ------> USART1_TX
	//PA10 ------> USART1_RX 
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Pin = GPIO_PIN_0; //??A? 0?? ?? ???? 0? ??
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; //?? ????0? ?????? ??
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) 
{
	if(GPIO_Pin == GPIO_PIN_0)
	{
		COUNT++;
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
}

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, 10); 
	return ch;
}

int main()
{
	HAL_Init();
	GPIOx_Init();
	UARTx_Init();
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0); //???? enable ? Priority ??.
	HAL_NVIC_EnableIRQ(EXTI0_IRQn); //?? ????0 ???? ??(????? ???? ??)
	COUNT = 0;
	while(1)
	{
		printf("Count is %d\r\n", COUNT);
		HAL_Delay(100);
	}
}
