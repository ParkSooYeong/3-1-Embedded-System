/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 6 */

#include "stm32f4xx_hal.h"

#define TLCD_RS GPIO_PIN_0 // LCD RS ??
#define TLCD_RW GPIO_PIN_1 // LCD RW ??
#define TLCD_E GPIO_PIN_2 // LCD E ??
#define P_DATA GPIOC // LCD DATA ??
#define P_CTL GPIOB // LCD CONTROL ??

void GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	HAL_Init();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void TLCD_EnablePulse(void)
{
	HAL_GPIO_WritePin(P_CTL, TLCD_E, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(P_CTL, TLCD_E, GPIO_PIN_RESET);
}

void TLCD_SendData(unsigned char data)
{
	HAL_GPIO_WritePin(P_CTL, TLCD_RW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(P_CTL, TLCD_RS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(P_DATA, 0xFF, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(P_DATA, data, GPIO_PIN_SET);
	TLCD_EnablePulse();
	HAL_Delay(1);
}

void TLCD_SendCommand(unsigned char data)
{
	HAL_GPIO_WritePin(P_CTL, TLCD_RW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(P_CTL, TLCD_RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(P_DATA, 0xFF, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(P_DATA, data, GPIO_PIN_SET);
	TLCD_EnablePulse();
	HAL_Delay(1);
}

void TLCD_Init(void)
{
	HAL_Delay(30); //30 [ms] ?? ??
	TLCD_SendCommand(0x38); //function : 8bit data, 2Line, 5*7 dot
	TLCD_SendCommand(0x0c); //Display ON 
	TLCD_SendCommand(0x01); //Clear Screen 
	HAL_Delay(2); //?? ?? (about 2ms)
}

void TLCD_DispString(char *str)
{
	while(*str)
		{
			TLCD_SendData(*str);
			str++;
		}
}

void TLCD_Goto(int y, int x)
{
	unsigned char RamAddr;
	if(y==0) RamAddr=0x80+x; // line 1 D.D.RAM address
	else RamAddr=0xC0+x; // line 2 D.D.RAM address
	TLCD_SendCommand(RamAddr);
}

int main()
{
	GPIO_Init();
	TLCD_Init();
	
	//?? ???? ? ????? ??? ?? data? ????
	TLCD_Goto(0,2); //??? ? ?? ????? ?? ??
	TLCD_DispString("Happy Day");
	
	TLCD_Goto (1,4); //2?? ? 5? ????? ?? ??
	TLCD_DispString("Welcome !"); 
}
