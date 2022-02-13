/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 10 */

#include "stm32f4xx_hal.h"
#include <stdio.h>

int t_cnt = 0, m_cnt=0;
TIM_HandleTypeDef hTimBase;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if( htim->Instance == TIM7 )
	{
		if((++t_cnt) >= 999)
		{
			t_cnt = 0;
			m_cnt ++ ;
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
		}
	}
}

void TIMx_Init()
{
	__HAL_RCC_TIM7_CLK_ENABLE();
	
	hTimBase.Instance = TIM7;
	hTimBase.Init.Prescaler = 16-1; //(168Mhz/2)/84 = 1MHz(1us)
	hTimBase.Init.Period = 999; //1000-1;
	hTimBase.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&hTimBase);
	
	HAL_NVIC_ClearPendingIRQ(TIM7_IRQn);
	HAL_NVIC_SetPriority(TIM7_IRQn, 7, 0); /* Set interrupt Priority*/
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	HAL_TIM_Base_Start_IT(&hTimBase);
}

void GPIOx_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
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
	HAL_Init();
	UARTx_Init();
	GPIOx_Init();
	TIMx_Init();
	
	while(1)
	{
		printf("Timer in SEC : %4d\r", m_cnt);
		HAL_Delay(1000);
	}
}
