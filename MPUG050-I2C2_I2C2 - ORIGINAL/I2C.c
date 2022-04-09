/*
 * I2C.c
 *
 *  Created on: Mar 20, 2022
 *      Author: USER
 */
#include "stm32f4xx.h"
#include "I2C.h"




void I2C_init(void)
{
	RCC->AHB1ENR |= 2;  /* Enable GPIOB clock to PORTB*/
	RCC->APB1ENR |=0x00600000; /* Enable I2C1 AND I2Cclock */
	/* configure PB3, PB10 pins for I2C1 */
	GPIOB->MODER  |= 0x2A0280;    /* PB3, PB10 use alternate function */
	GPIOB->OTYPER |= 0x708;  /* output open-drain */ 
	GPIOB->PUPDR  |= 0x00050100;
	GPIOB->OSPEEDR|=0x3000C0;    /*Fast mode*/
	GPIOB->AFR[0] |= 0x9000; //PB3 to Alternate function AF9
	GPIOB->AFR[1] |= 0x444;  //PB10 to Alternate function AF9
	
	//Configure I2C1
	hi2c1->CR1      = 0x8000;            /* software reset I2C1 */
	hi2c1->CR1     &= ~0x8000;           /* out of reset */
	hi2c1->CR2      = 0x0010;            /* peripheral clock is 16 MHz*/
	hi2c1->CCR      = 80;                /* standard mode, 100kHz clock */
	hi2c1->TRISE    = 17;                 /* maximum rise time */
	hi2c1->CR1     |= 0x0001;            /* enable I2C1module */
	//Configure I2C2
	
	while(!(RCC->APB1ENR)); /* wait for enabling i2c2*/
	hi2c2->CR1     &=~ I2C_CR1_PE;
	hi2c2->CR1     = 0x8000; /* software reset I2C2 */
	hi2c2->CR1     &= ~0x8000;/* out of reset */
	hi2c2->CR2     = 0x2a; /* peripheral clock is 16 MHz*/
	hi2c2->CCR     = 0x8023; /* standard mode, 100kHz clock */
	hi2c2->TRISE   =0xd0; /* maximum rise time */
	hi2c2->CR1    |= 0x01; /* enable I2C2 module */
	
}


/* this funtion reads a byte of data from the memory location
maddr of a device with I2C slave device address saddr. * For
simplicity, no error checking or error report is done. */

int I2C_byteRead(I2C_TypeDef *hi2c,char saddr, char maddr,uint8_t* data)
{
	
	volatile uint8_t tmp=0;
	while (hi2c->SR2 & 2); /* wait until bus not busy */
	hi2c->CR1 |= 0x100; /* generate start */
	while (!(hi2c->SR1 & 1));/* wait until start flag is set */
	hi2c->DR =saddr << 1; /* transmit slave address + Write */
	while (!(hi2c->SR1 & 2)); /* wait until addr flag is set */
	tmp = hi2c->SR2;/* clear addr flag */
	while (!(hi2c->SR1 & 0x80)); /* wait until data register empty */
	hi2c->DR = maddr; /* send memory address */
	while (!(hi2c->SR1 & 0x80)); /* wait until data register empty */
	hi2c->CR1 |= 0x100; /* generate restart */
	while (!(hi2c->SR1 & 1));/* wait until start flag is set */
	hi2c->DR = saddr << 1 | 1; /*transmit slave address + Read */
	while (!(hi2c->SR1 & 2)); /* wait until addr flag is set */
	hi2c->CR1 &= ~0x400; /* Disable Acknowledge */
	tmp = hi2c->SR2; /* clear addr flag */
	hi2c->CR1 |= 0x200; /* generate stop after data received */
	while (!(hi2c->SR1 & 0x40)); /* Wait until RXNE flag is set */
	*data++ = hi2c->DR; /* Read data from DR */
	return 0;
}
int I2C_byteWrite(I2C_TypeDef *hi2c,char saddr, char maddr, uint8_t* data)
{
	volatile int tmp=0;
	while (hi2c->SR2 & 2); /* wait until bus not busy */
	hi2c->CR1 |= 0x100; /* generate start */
	while (!(hi2c->SR1 & 1));/* wait until start flag is set */
	hi2c->DR = saddr << 1; /* transmit slave address */
	while (!(hi2c->SR1 & 2)); /* wait until addr flag is set */
	tmp = hi2c->SR2; /*clear addr flag */
	while (!(hi2c->SR1 & 0x80)); /* wait until data register empty */
	hi2c->DR = maddr; /* send memory address */
	while (!(hi2c->SR1 & 0x80)); /* wait until data register empty */
	hi2c->DR = *data; /* transmit data */
	while (!(hi2c->SR1 & 4)); /* wait until transfer finished */
	hi2c->CR1 |= 0x200; /* generate stop */
	return 0;
}
