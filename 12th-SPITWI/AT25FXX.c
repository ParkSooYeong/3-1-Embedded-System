#include "AT25FXX.h"
#include	<stdio.h>

extern SPI_HandleTypeDef hspi;
static void AT25FXX_BUSY(void)
{
	uint8_t status;
	AT25FXX_Disable;	
	uint8_t cmd[1];
	cmd[0] = RDSR;
	do {
		AT25FXX_Enable;
		HAL_SPI_Transmit(&hspi, cmd, 1, 1000);	
		HAL_SPI_Receive(&hspi, &status, 1, 1000);
		AT25FXX_Disable;
//		printf("status=%d\n\r", status);
	} while( (status&0x01) != 0 );

}

void AT25FXX_Read_ID(uint8_t *ID)
{
	uint8_t cmd[1];
	cmd[0] = RDID;

	AT25FXX_BUSY();
	AT25FXX_Enable;
	HAL_SPI_Transmit(&hspi, cmd, 1, 1000);	
	HAL_SPI_Receive(&hspi, ID, 2, 1000);
	AT25FXX_Disable;
}

void AT25FXX_Ctrl_Write(uint8_t wp)
{
	uint8_t cmd[1];
	if( wp == ENABLE ) cmd[0] = WREN;
	else cmd[0] = WRDI;

	AT25FXX_BUSY();
	AT25FXX_Enable;
	HAL_SPI_Transmit(&hspi, cmd, 1, 1000);
	AT25FXX_Disable;
}

void AT25FXX_Erase_CHIP()
{
	uint8_t cmd[1];
	cmd[0] = CHIP_ERASE;

	AT25FXX_BUSY();
	AT25FXX_Enable;
	HAL_SPI_Transmit(&hspi, cmd, 1, 1000);
	AT25FXX_Disable;
}

void AT25FXX_Write(uint16_t BufferOffset, uint8_t Data)
{
	uint8_t cmd[5], res;
	cmd[0] = PROGRAM;
	cmd[1] = 0x00;
	cmd[2] = (uint8_t)(BufferOffset>>8);
	cmd[3] = (uint8_t)BufferOffset;
	cmd[4] = Data;
	
	AT25FXX_BUSY();
	AT25FXX_Enable;
	res = HAL_SPI_Transmit(&hspi, cmd, 5, 1000);
	if( res != HAL_OK ) printf("Error at %d", Data);

	AT25FXX_Disable;
}

uint8_t AT25FXX_Read(uint16_t BufferOffset)
{		
	uint8_t cmd[4];
	cmd[0] = READ;
	cmd[1] = 0x00;
	cmd[2] = (uint8_t)BufferOffset>>8;
	cmd[3] = (uint8_t)BufferOffset;

	AT25FXX_BUSY();
	AT25FXX_Enable;
	HAL_SPI_Transmit(&hspi, cmd, 4, 1000);
	HAL_SPI_Receive(&hspi, cmd, 1, 1000);
	AT25FXX_Disable;
	return cmd[0];
}
