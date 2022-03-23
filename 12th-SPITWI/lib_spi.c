#include "stm32f4xx_hal.h"

SPI_HandleTypeDef hspi;

void SPIx_Init(void)
{
	__SPI1_CLK_ENABLE();
	hspi.Instance = SPI1;
	hspi.Init.Mode = SPI_MODE_MASTER;
	hspi.Init.Direction = SPI_DIRECTION_2LINES;
	hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi.Init.NSS = SPI_NSS_SOFT;
	hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi.Init.TIMode = SPI_TIMODE_DISABLED;
	hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	hspi.Init.CRCPolynomial = 10;
	HAL_SPI_Init(&hspi);
}

void GPIO_spi_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_4; // SPI:SS - Configure GPIO pin : PA4
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
