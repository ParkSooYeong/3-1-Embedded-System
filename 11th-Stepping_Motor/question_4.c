/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 13 */

#include "stm32f4xx_hal.h"

int main()
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	HAL_Init();
	__HAL_RCC_GPIOA_CLK_ENABLE(); // PUSH BUTTON SW
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	// PUSH BUTTON SW
	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*Configure GPIO pins : PB8 PB9 PB10 PB11 */
	GPIO_InitStructure.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL; //GPIO_PULLUP
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	uint16_t input_data, past_key; //
	
	while(1)
	{
		input_data = ((GPIOA->IDR) & 0xFF);
		if((past_key == 0) && (input_data != 0))
		{
			if( input_data & 0x01 ) // 1 Rotate
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
				HAL_Delay(10); // 10 ms ??
				
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
				HAL_Delay(10); // 10 ms ??
				
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
				HAL_Delay(10); // 10 ms ??
				
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
				HAL_Delay(10); // 10 ms ??
			}
			if( input_data & 0x02 ) // 1 Reverse Rotate
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
				HAL_Delay(10); // 10 ms ??
				
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
				HAL_Delay(10); // 10 ms ??
				
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
				HAL_Delay(10); // 10 ms ??
				
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
				HAL_Delay(10); // 10 ms ??
			}
		}
		past_key = input_data;
	}
}
