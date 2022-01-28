/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 7 */

#include "stm32f4xx_hal.h"
#include <stdio.h>

void GPIOx_Init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

UART_HandleTypeDef huart;

int fputc(int ch, FILE *f)
{
	/* write a character to the uart1 and Loop until the end of transmission */
	HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, 10); 
	return ch;
}

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

int main()
{
	HAL_Init(); GPIOx_Init(); UARTx_Init();
	printf("Test Terminal\r\n");
	uint8_t c = 'A';
	float f_val = 1.2;
	while (1)
	{
		printf("Send Character %c %f\r\n", c, f_val+=(float)0.1);
		if( ++c > 'Z' ) c = 'A';
		HAL_Delay(10);
	}
}
