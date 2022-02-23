/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 11 */

#include "stm32f4xx_hal.h"

TIM_HandleTypeDef htim;

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_OC_InitTypeDef sConfigOC;
	
	HAL_Init();
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	htim.Instance = TIM2;
	htim.Init.Prescaler = 15-1;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 16700-1;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_PWM_Init(&htim);
	
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_2);
	
	HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_2);
	
	while (1)
	{
		for(int i=0; i<16700; i+=100)
		{
			// htim.Instance->CCR1 = i;
			__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_2, i);
			HAL_Delay(10);
		}
	}
}
