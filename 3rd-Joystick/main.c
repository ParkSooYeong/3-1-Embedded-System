/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 4 */

// 1) OPEN405 Cortex-M4 Module Port A(PA0,PA1) PB_SW(Input)
// 2) OPEN405 Cortex-M4 Module Port C(PC4 ~ PC7,PC13) JOYSTICK_SW(Input)
// 3) OPEN405 Cortex-M4 Module Port B(PB0 ~ PB3) LED(Output)
#include "stm32f4xx_hal.h"

int main()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	HAL_Init();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_PULLUP; //GPIO_NOPULL-->GPIO_PULLUP
	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_13;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	uint16_t mode = 0;
	while(1)
	{
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4)==RESET)mode = GPIO_PIN_0; //0
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)==RESET)mode = GPIO_PIN_1; //1
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)==RESET)mode = GPIO_PIN_2; //2
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7)==RESET)mode = GPIO_PIN_3; //4
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==RESET)mode = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
		else mode = 0;
		
		if(mode==0)
			HAL_GPIO_WritePin(GPIOB,0x000F,GPIO_PIN_RESET); // OFF all LED
		else
			HAL_GPIO_WritePin(GPIOB,mode&0x000F,GPIO_PIN_SET); // ON LED
	}
}
