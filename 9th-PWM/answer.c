#include "stm32f4xx_hal.h"

TIM_HandleTypeDef htim;

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_OC_InitTypeDef sConfigOC;

	HAL_Init();

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_TIM10_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	htim.Instance = TIM10;
	htim.Init.Prescaler = 16 - 1;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 10000-1;
//	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;								//default value
//	htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;		//default value
	HAL_TIM_PWM_Init(&htim);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 5000-1;											//50% = 5,000/10,000
//	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;											//default value
//	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;											//default value
	HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_1);

	HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1);

	GPIO_PinState	in_key, past_key;
	int	cv;
	while (1) {
		in_key = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
		if((past_key == GPIO_PIN_RESET) && (in_key != GPIO_PIN_RESET)) {
			cv = htim.Instance->CCR1;
			if( cv >= (10000-1) ) htim.Instance->CCR1 = 0;
			else htim.Instance->CCR1 = ((cv==0)?-1:cv) + 2500;
		}  
		past_key = in_key;   
	}
}