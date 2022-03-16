/*********************************************************************************************************
*
* File                : AT45DBXX.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 5.15
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2015, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#ifndef _AT25FXX_H
#define _AT25FXX_H

#include	"stm32f4xx_hal.h"
#include	<stdint.h>

/* Instruction Codes */
#define WREN            0x06
#define WRDI            0x04
#define RDSR            0x05
#define WRSR            0x01
#define READ            0x03
#define PROGRAM         0x02
#define SECTOR_ERASE    0x52
#define CHIP_ERASE      0x62
#define RDID            0x15

#define	ENABLE					1
#define	DISABLE					0

#define AT25FXX_Enable 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
#define AT25FXX_Disable			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

void	AT25FXX_Read_ID(uint8_t *Data);
void	AT25FXX_Write(uint16_t BufferOffset, uint8_t Data);
void	AT25FXX_Ctrl_Write(uint8_t wp);
void	AT25FXX_Erase_CHIP(void);
uint8_t AT25FXX_Read(uint16_t BufferOffset);

#endif /*_AT25FXX_H*/
