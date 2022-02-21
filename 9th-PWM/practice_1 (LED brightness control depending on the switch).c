/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 11 */

#include "stm32f4xx_hal.h"

TIM_HandleTypeDef htim3;

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_OC_InitTypeDef sConfigOC;
	
	HAL_Init();
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 15;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 10000-1;
	HAL_TIM_PWM_Init(&htim3);
	
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 1000-1;
	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
	
	sConfigOC.Pulse = 10000-1;
	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);
	
	while (1)
	{
		if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET )
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
		if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET )
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
		if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET )
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
		if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET )
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
	}
}
