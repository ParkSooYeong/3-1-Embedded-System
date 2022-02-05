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
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_0; // delete GPIO_PIN_1
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	htim.Instance = TIM2;
	htim.Init.Prescaler = 16 - 1;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 10000-1;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_PWM_Init(&htim);
	
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 5000-1; //50% = 5,000/10,000
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_2);
	
	HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_2);
	
	GPIO_PinState in_key, past_key;
	int cv = 0;
	
	while (1)
	{
		in_key = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
		if((past_key == GPIO_PIN_RESET) && (in_key != GPIO_PIN_RESET))
		{
			if(cv == 0)
			{
				__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_2, 1-1); // 0%
				cv = 1;
			}
			else if(cv == 1)
			{
				__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_2, 2500-1); // 25%
				cv = 2;
			}
			else if(cv == 2)
			{
				__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_2, 5000-1); // 50%
				cv = 3;
			}
			else if(cv == 3)
			{
				__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_2, 7500-1); // 75%
				cv = 4;
			}
			else if(cv == 4)
			{
				__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_2, 10000-1); // 100%
				cv = 0;
			}
		}
		past_key = in_key;
	}
}
