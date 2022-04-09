/*
 * I2C.h
 *
 *  Created on: Mar 20, 2022
 *      Author: USER
 */
#include <stdint.h>
#include "stm32f4xx.h"

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#define hi2c1 I2C1
#define hi2c2 I2C2

void I2C_init(void);
int I2C_byteRead(I2C_TypeDef *hi2c,char saddr, char maddr,uint8_t* data);
int I2C_byteWrite(I2C_TypeDef *hi2c,char saddr, char maddr, uint8_t* data);

#endif /* SRC_I2C_H_ */
