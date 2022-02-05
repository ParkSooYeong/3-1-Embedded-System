#include "stm32f4xx_hal.h"
#include <stdio.h>
int COUNT[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int EINT_NO;
UART_HandleTypeDef huart;

void UARTx_Init() {
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
int fputc(int ch, FILE *f) {
   HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, 10); 
return ch;
}
GPIO_InitTypeDef GPIO_InitStruct;
void GPIOx_Init() {
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();
   GPIO_InitTypeDef GPIO_InitStruct; //PA9 ------> USART1_TX
   GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10; //PA10 ------> USART1_RX 
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
   GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
      
   GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

uint16_t mode = 0x01;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
   while(1){
   static uint32_t debounce = 0;
   static uint32_t now = 0;
   now = HAL_GetTick();
   if(now - debounce < 200) return;
   debounce = HAL_GetTick();
      
   if(GPIO_Pin == GPIO_PIN_6) {         //c
      EINT_NO = 6;
      COUNT[6]++;
      mode=mode*2;
   }
   else if(GPIO_Pin == GPIO_PIN_5) {   //b
      EINT_NO = 5;
      COUNT[5]++;
      mode=mode/2;
   }
   if(mode>0x08){
      mode=0x01;
   }
   else if(mode<0x01){
      mode=0x08;
   }
   mode=mode;
   GPIOB->ODR=mode;
   }
}
int main()
{
   HAL_Init();
   GPIOx_Init();
   UARTx_Init();
   HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(EXTI4_IRQn);
   HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
   GPIOB->ODR=mode;
   while(1){
      printf("%2d : COUNT %02d\r\n", EINT_NO, COUNT[EINT_NO] );
   }
}
