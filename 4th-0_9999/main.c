/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 5 */

#include "stm32f4xx_hal.h"

unsigned char FONT[18] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07,
0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0X08, 0x80 };

void Segment(int val)
{
	int i;
	unsigned char N1000, N100, N10, N1;
	int Buff;
	
	N1000 = val /1000; Buff = val % 1000;
	N100 = Buff / 100; Buff = Buff % 100;
	N10 = Buff /10; N1 = Buff % 10;
	
	for(i = 0; i < 30; i++)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, FONT[N1000], GPIO_PIN_SET);
		HAL_Delay(1); 
		HAL_GPIO_WritePin(GPIOC, FONT[N1000], GPIO_PIN_RESET);
		
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, FONT[N100], GPIO_PIN_SET);
		HAL_Delay(1); 
		HAL_GPIO_WritePin(GPIOC, FONT[N100], GPIO_PIN_RESET);
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, FONT[N10], GPIO_PIN_SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOC, FONT[N10], GPIO_PIN_RESET);
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, FONT[N1], GPIO_PIN_SET);
		HAL_Delay(1); 
		HAL_GPIO_WritePin(GPIOC, FONT[N1], GPIO_PIN_RESET);
	}
}
	
int main()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	int count = 0;
	
	HAL_Init();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	while(1)
	{
		Segment(count); 
		if(count==9999) count = 0;
		else count += 1;
	}
}
