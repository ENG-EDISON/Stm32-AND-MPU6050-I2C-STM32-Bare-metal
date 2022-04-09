/*
 * MPU6050.h
 *
 *  Created on: Mar 20, 2022
 *      Author: USER
 */
#include "I2C.h"
#include <stdint.h>
#include <stdlib.h>

#ifndef SRC_MPU6050_H_
#define SRC_MPU6050_H_

typedef struct MPU6050_Raw
{
	uint16_t X_axis;
	uint16_t Y_axis;
	uint16_t Z_axis;
	uint16_t GX_axis;
	uint16_t GY_axis;
	uint16_t GZ_axis;
}MPU6050_Raw;

typedef struct MPU6050_Data
{
	double X_Axis;
	double Y_Axis;
	double Z_Axis;
	double GX_Axis;
	double GY_Axis;
	double GZ_Axis;
	double Roll;
	double Pitch;
	double Yaw;
}MPU6050_GetData;

void MPU6050_Init(I2C_TypeDef *hi2c);
void MPU6050_Get_Acce_Gyr(I2C_TypeDef *hi2c,MPU6050_GetData* Read_Value);
uint16_t MP6050_I2cRead(I2C_TypeDef *hi2c,char address);
void MPU6050_Angles(MPU6050_GetData *GetValue);
void MPU6050_ConvertData(MPU6050_Raw *SensorData,MPU6050_GetData* Read_Value);
#endif /* SRC_MPU6050_H_ */
