/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 9 */

#include "stm32f4xx_hal.h"
#include <stdio.h>

int EINT_NO; //Interrupt Number

// UART
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

//GPIO
GPIO_InitTypeDef GPIO_InitStruct;
void GPIOx_Init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE(); // UART
	__HAL_RCC_GPIOB_CLK_ENABLE(); // LED
	__HAL_RCC_GPIOC_CLK_ENABLE(); // JOY STICK

	GPIO_InitTypeDef GPIO_InitStruct; 						//PA9 ------> USART1_TX
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10; //PA10 ------> USART1_RX 
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 			// UART
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); // LED
	
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); // JOY STICK
}

uint16_t mode = 0x0001; // Start LED : 0001

//Callback Function
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static uint32_t debounce = 0;
	static uint32_t now = 0;
	now = HAL_GetTick();
	if(now - debounce < 200) return;
	debounce = HAL_GetTick();
	
	if(GPIO_Pin == GPIO_PIN_5) //JOY STICK B
	{
		EINT_NO = 5;
		HAL_GPIO_WritePin(GPIOB,mode,GPIO_PIN_RESET); // ex : 0001 -> 0000
		mode = mode << 1; // left shift
		if(mode == 0x0010) // 1 0000
		{
			mode = 0x0001; // 0001
		}
		HAL_GPIO_WritePin(GPIOB,mode,GPIO_PIN_SET); // ex : 0000 -> 0010
	}
	else if(GPIO_Pin == GPIO_PIN_6) //JOY STICK C
	{
		EINT_NO = 6;
		HAL_GPIO_WritePin(GPIOB,mode,GPIO_PIN_RESET); // ex : 1000 -> 0000
		mode = mode >> 1; // right shift
		if(mode == 0x0000) // 0000 0000 0000 0000
		{
			mode = 0x0008; // 0000 0000 0000 1000
		}
		HAL_GPIO_WritePin(GPIOB,mode,GPIO_PIN_SET); // ex : 0000 -> 0100
	}
}

int main()
{
	HAL_Init();
	GPIOx_Init();
	UARTx_Init();
	
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	HAL_GPIO_WritePin(GPIOB,mode,GPIO_PIN_SET); // Start LED : 0001
	
	while(1)
	{
		printf("%d", mode); // Putty Output
	}
}
