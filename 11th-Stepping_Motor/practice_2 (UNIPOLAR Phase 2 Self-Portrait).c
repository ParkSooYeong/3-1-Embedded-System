/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 13 */

#include "stm32f4xx_hal.h"
#define STEP_A GPIO_PIN_8
#define STEP_B GPIO_PIN_9
#define STEP_AN GPIO_PIN_10
#define STEP_BN GPIO_PIN_11
#define STEP_PORT GPIOB

unsigned char STEP_M_STAT=0;
TIM_HandleTypeDef htim2;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if( htim->Instance == TIM2 )
	{
		STEP_M_STAT = (STEP_M_STAT + 1) % 4;
	}
}

void GPIOx_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/*Configure GPIO pins : PB8 PB9 PB10 PB11 */
	GPIO_InitStructure.Pin = STEP_A|STEP_B|STEP_AN|STEP_BN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL; //GPIO_PULLUP
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(STEP_PORT, &GPIO_InitStructure);
}

void TIM2_Init()
{
	__HAL_RCC_TIM2_CLK_ENABLE();
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 16-1; // 1MHz
	htim2.Init.Period = 10000-1; // 100Hz
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&htim2);
	
	HAL_NVIC_ClearPendingIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 7, 0); /* Set interrupt Priority*/
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_TIM_Base_Start_IT(&htim2);
}

int main()
{
	HAL_Init();
	GPIOx_Init();
	TIM2_Init();
	
	unsigned char c_stat;
	while(1)
	{
		if( c_stat != STEP_M_STAT )
		{
			HAL_GPIO_WritePin(STEP_PORT, 0xF<<8, GPIO_PIN_RESET);
			if( STEP_M_STAT == 0 ) HAL_GPIO_WritePin(STEP_PORT, STEP_A|STEP_B, GPIO_PIN_SET);
			else if( STEP_M_STAT == 1 ) HAL_GPIO_WritePin(STEP_PORT, STEP_B|STEP_AN, GPIO_PIN_SET);
			else if( STEP_M_STAT == 2 ) HAL_GPIO_WritePin(STEP_PORT, STEP_AN|STEP_BN, GPIO_PIN_SET);
			else if( STEP_M_STAT == 3 ) HAL_GPIO_WritePin(STEP_PORT, STEP_BN|STEP_A, GPIO_PIN_SET);
			c_stat = STEP_M_STAT;
		}
	}
}
