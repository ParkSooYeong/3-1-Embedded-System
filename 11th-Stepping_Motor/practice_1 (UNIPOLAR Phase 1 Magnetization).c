/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 13 */

#include "stm32f4xx_hal.h"

int main()
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	HAL_Init();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/*Configure GPIO pins : PB8 PB9 PB10 PB11 */
	GPIO_InitStructure.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL; //GPIO_PULLUP
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	while(1) 
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
}
