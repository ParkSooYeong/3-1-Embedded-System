/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 9 */

#include "stm32f4xx_hal.h"
#include <stdio.h>

int COUNT[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int EINT_NO;

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

GPIO_InitTypeDef GPIO_InitStruct;
void GPIOx_Init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct; //PA9 ------> USART1_TX
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10; //PA10 ------> USART1_RX 
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_4)
	{
		EINT_NO = 4;
		COUNT[4]++;
	}
	else if(GPIO_Pin == GPIO_PIN_5)
	{
		EINT_NO = 5;
		COUNT[5]++;
	}
	else if(GPIO_Pin == GPIO_PIN_6)
	{
		EINT_NO = 6;
		COUNT[6]++;
	}
	else if(GPIO_Pin == GPIO_PIN_7)
	{
		EINT_NO = 7;
		COUNT[7]++;
	}
}

int main()
{
	HAL_Init();
	GPIOx_Init();
	UARTx_Init();
	
	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	while(1)
	{
		printf("%2d : COUNT %02d\r\n", EINT_NO, COUNT[EINT_NO] );
	}
}
