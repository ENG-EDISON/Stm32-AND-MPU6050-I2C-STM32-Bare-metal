#include "stm32f4xx.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>
#include "MPU6050.h"
#include "I2C.h"
#include <stdint.h>



static char Buffer[100];
static  MPU6050_GetData MPU6050_Data={0};
static  MPU6050_GetData MPU6050_Data2={0};

int main(void)
{
	I2C_init();
	MPU6050_Init(hi2c1);
	MPU6050_Init(hi2c2);
	Usart2_Init();
	/* configure PA5 for the green LED (LD2) */
	RCC->AHB1ENR |= 1; /* enable GPIOA clock */
	GPIOA->MODER &=~ 0x00000C00; /* clear pin mode */
	GPIOA->MODER |= 0x00000400; /* Set pin to output mode */
	while (1)
	{
		MPU6050_Get_Acce_Gyr(hi2c1,&MPU6050_Data);
		MPU6050_Get_Acce_Gyr(hi2c2,&MPU6050_Data2);
		sprintf(Buffer,"X1=%f		Y1=%f		Z1=%f\r\n",MPU6050_Data.X_Axis,MPU6050_Data.Y_Axis,MPU6050_Data.Z_Axis);
		Uart2_Send(Buffer);
		sprintf(Buffer,"X2=%f		Y2=%f		Z2=%f\r\n",MPU6050_Data2.X_Axis,MPU6050_Data2.Y_Axis,MPU6050_Data2.Z_Axis);
		Uart2_Send(Buffer);
		GPIOA->ODR ^=0x20;
	}

}


