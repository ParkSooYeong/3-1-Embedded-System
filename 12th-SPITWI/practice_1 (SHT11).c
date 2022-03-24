/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 1 , Week 14 */

#include "stm32f4xx_hal.h"
#include "lib_sensor.h"
#include <stdio.h>

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
	uint16_t temp=0, humi=0, seq=0;
	HAL_Init();
	UARTx_Init();
	initialize_SHT11();
	printf("Start.....\r\n");
	
	while (1)
	{
		temp = get_SHT11_data(TEMP); delay_ms(100);
		humi = get_SHT11_data(HUMI); delay_ms(100);
		printf("Seq : %1d\r\n", seq++);
		printf("Temp : %4.1fC \r\n", temp/10.0);
		printf("Humi : %4.1f%% \r\n\n", humi/10.0);
	}
}
