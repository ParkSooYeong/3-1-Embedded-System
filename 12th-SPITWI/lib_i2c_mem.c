#include "stm32f4xx_hal.h"

I2C_HandleTypeDef		hi2c;
void I2Cx_Init(void)
{
	__I2C1_CLK_ENABLE();

  hi2c.Instance = I2C1;
  hi2c.Init.ClockSpeed = 400000;
  hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c.Init.OwnAddress1 = 0;
  hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c.Init.OwnAddress2 = 0;
  hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

  HAL_I2C_Init(&hi2c);
}

void GPIO_i2c_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__GPIOB_CLK_ENABLE();

    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
