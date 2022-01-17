/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 2 */ 

#include	"stm32f4xx_hal.h"
#define LED_PORT GPIOB

int main() {	GPIO_InitTypeDef   GPIO_InitStructure;
	
	HAL_Init();
	__HAL_RCC_GPIOB_CLK_ENABLE();						//__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStructure);
	
	while(1) {
//		HAL_GPIO_WritePin(LED_PORT, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);
		LED_PORT->ODR = 0x000F;
		HAL_Delay(500);
		
//		HAL_GPIO_WritePin(LED_PORT, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);
		LED_PORT->ODR = 0x0000;
		HAL_Delay(500);
	}
}
