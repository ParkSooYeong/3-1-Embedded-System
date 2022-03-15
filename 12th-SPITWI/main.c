/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 14 */

#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "AT25FXX.h"

void UARTx_Init(void);
void SPIx_Init(void);
void GPIO_spi_Init(void);

#define NOP 16

int main()
{
	uint8_t ID[2];
	int i;
	
	HAL_Init(); 
	UARTx_Init();
	GPIO_spi_Init(); 
	SPIx_Init();
	
	printf("\r\nStart.....\r\n");
	printf("***************SPI Example*******************************\r\n");
	
	/*##-1- Read the device ID ########################*/
	AT25FXX_Read_ID(ID);
	printf("AT25FXX ID is");
	for(i=0;i<2;i++) {
	printf(" 0x%02X ", ID[i]);
	}
	
	/*##-2- Erase All memory ########################*/
	AT25FXX_Ctrl_Write(ENABLE);
	AT25FXX_Erase_CHIP();
	
	/*##-3-Written to the flash ########################*/ 
	printf("\r\n\nFALSHAT25FXX Write Test: \r\n");
	for(i=0; i<NOP; i++) {
	AT25FXX_Ctrl_Write(ENABLE);
	AT25FXX_Write((uint16_t)i, i);
	printf("%3d ",i);
	}
	
	/*##-4- Read the flash ########################*/ 
	printf("\r\n\nFALSHAT25FXX Read Test: \r\n");
	for(i=0; i<NOP; i++) {
	printf("%3d ", AT25FXX_Read((uint16_t)i));
	}
	printf("\r\n");
}
