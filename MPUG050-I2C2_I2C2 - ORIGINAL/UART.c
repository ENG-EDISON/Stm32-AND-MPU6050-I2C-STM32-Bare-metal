/*
 * UART.c
 *
 *  Created on: Mar 20, 2022
 *      Author: USER
 */
#include "stm32f4xx.h"
#include "UART.h"
#include <string.h>

void Usart2_Init(void)
{
	RCC->APB1ENR  |=0x20000;    //Enable clock to USART2
	RCC->AHB1ENR  |=1;          //Enable clock to GPIOA
	GPIOA->AFR[0] |=0x700;      //Set PA3 for USART2
	GPIOA->MODER  |=0x20;       //Set PA3 as alternate Function
	USART2->BRR  |=0x683;      //Set baudrate to 9600
	USART2->CR1   |=0x8;        //Enable TX by setting
	USART2->CR1   |=0x2000;     //Enable USART communication
}

int Uart2_Send(char* string)
{
	int n=0;

	for(n=0;n<strlen(string);n++)
	{
		while(!(USART2->SR & 0x80)){};
		USART2->DR=(string[n] & 0xFF);
	}
	return 1;
}
