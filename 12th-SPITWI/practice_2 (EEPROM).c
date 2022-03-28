/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 14 */

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

void UARTx_Init(void);
void I2Cx_Init(void);
void GPIO_i2c_Init(void);

#define ADDR_24LCxx_Write 0xA0
#define ADDR_24LCxx_Read 0xA1
extern I2C_HandleTypeDef hi2c;

#define RW_NO 256 //16
uint8_t WriteBuffer[RW_NO], ReadBuffer[RW_NO];

UART_HandleTypeDef huart;

void UARTx_Init()
{
	__HAL_RCC_USART1_CLK_ENABLE();
	huart.Instance = USART1;
	huart.Init.BaudRate = 115200;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&huart);
}

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, 10); 
	return ch;
}

int main()
{
	int i;
	unsigned char RESULT;
	
	HAL_Init(); 
	UARTx_Init(); 
	I2Cx_Init(); GPIO_i2c_Init();
	
	for(i=0; i<RW_NO; i++)
		WriteBuffer[i]=i%128; // WriteBuffer init
	printf("\r\nStart.....\r\n");
	
	printf("***************I2C Example*******************************\r\n");
	for(i=0; i<RW_NO; i++)
	{ // wrinte date to EEPROM 
		RESULT = HAL_I2C_Mem_Write(&hi2c, ADDR_24LCxx_Write, i, I2C_MEMADD_SIZE_8BIT, WriteBuffer+i, 1, 0x1000);
		if( RESULT != HAL_OK ) break;
		HAL_Delay(1);
	}
	if( RESULT == HAL_OK ) printf("EEPROM 24C16 Write Test OK \r\n");
	else printf("EEPROM 24C16 Write Test False (%d %d)\r\n", RESULT, i);
	
	for(i=0; i<RW_NO; i++)
	{ // read date from EEPROM 
		RESULT = HAL_I2C_Mem_Read(&hi2c, ADDR_24LCxx_Read, i, I2C_MEMADD_SIZE_8BIT, ReadBuffer+i, 1, 0x1000);
		if( RESULT != HAL_OK ) break;
	}
	
	if( RESULT == HAL_OK ) printf("EEPROM 24C16 Read Test OK\r\n");
	else printf("EEPROM 24C16 Read Test False\r\n\n");
	
	if(memcmp(WriteBuffer, ReadBuffer, sizeof(WriteBuffer)) == 0 )
		printf("EEPROM 24C16 Compare Test OK\r\n");
	else
		printf("EEPROM 24C16 Compare Test False\r\n\n");
}
