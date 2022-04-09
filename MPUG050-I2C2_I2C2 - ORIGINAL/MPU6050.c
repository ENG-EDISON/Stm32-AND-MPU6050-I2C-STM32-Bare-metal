/*
 * MPU6050.c
 *
 *  Created on: Mar 20, 2022
 *      Author: USER
 */
#include "MPU6050.h"
#include "I2C.h"
#include <stdint.h>
#include "math.h"

#define Square(X)  X*X

#define RAD_TO_DEG        57.295779513082320876798154814105
#define MPU6050_ADDR      0x68
#define WHO_AM_I_REG      0x75
#define PWR_MGMT_1_REG    0x6B
#define SMPLRT_DIV_REG    0x19
#define ACCEL_CONFIG_REG  0x1C
#define ACCEL_XOUT_H_REG  0x3B
#define ACCEL_YOUT_H_REG  0x3D
#define ACCEL_ZOUT_H_REG  0x3F
#define TEMP_OUT_H_REG    0x41
#define GYRO_CONFIG_REG   0x1B
#define GYRO_XOUT_H_REG   0x43
#define GYRO_YOUT_H_REG   0x45
#define GYRO_ZOUT_H_REG   0X47


static MPU6050_Raw     MPU6050_RawData;

typedef enum Mpu_Choice
{
	Acc_Data=0,
	Gyr_Data,
}Mpu_Choice;


void MPU6050_Init(I2C_TypeDef *hi2c)
{
	uint8_t Configure=0;
	I2C_byteWrite(hi2c,MPU6050_ADDR,PWR_MGMT_1_REG, &Configure);
	Configure=0x7;
	I2C_byteWrite(hi2c,MPU6050_ADDR,SMPLRT_DIV_REG, &Configure);
	Configure=0x00;
	I2C_byteWrite(hi2c,MPU6050_ADDR,ACCEL_CONFIG_REG,&Configure);
	Configure=0x00;
	I2C_byteWrite(hi2c,MPU6050_ADDR,GYRO_CONFIG_REG, &Configure);
}

void MPU6050_Get_Acce_Gyr(I2C_TypeDef *hi2c,MPU6050_GetData* Read_Value)
{

	MPU6050_RawData.X_axis= (uint16_t)MP6050_I2cRead(hi2c,ACCEL_XOUT_H_REG);
	MPU6050_RawData.Y_axis= (uint16_t)MP6050_I2cRead(hi2c,ACCEL_YOUT_H_REG);
	MPU6050_RawData.Z_axis= (uint16_t)MP6050_I2cRead(hi2c,ACCEL_ZOUT_H_REG);
	MPU6050_RawData.GX_axis= (uint16_t)MP6050_I2cRead(hi2c,GYRO_XOUT_H_REG);
	MPU6050_RawData.GY_axis= (uint16_t)MP6050_I2cRead(hi2c,GYRO_YOUT_H_REG);
	MPU6050_RawData.GZ_axis= (uint16_t)MP6050_I2cRead(hi2c,GYRO_ZOUT_H_REG);
	MPU6050_ConvertData(&MPU6050_RawData,Read_Value);
	
}
uint16_t MP6050_I2cRead(I2C_TypeDef *hi2c,char address)
{
	uint8_t I2cData[2]={0};
	I2C_byteRead(hi2c,MPU6050_ADDR,address,&I2cData[0]);
	I2C_byteRead(hi2c,MPU6050_ADDR,address+1,&I2cData[1]);
	return (I2cData[0]<<8 | I2cData[1]);
}

void MPU6050_ConvertData(MPU6050_Raw *SensorData,MPU6050_GetData* Read_Value)
{
		Read_Value->X_Axis= (int16_t)SensorData->X_axis/16384.0;
		Read_Value->Y_Axis= (int16_t)SensorData->Y_axis/16384.0;
		Read_Value->Z_Axis= (int16_t)SensorData->Z_axis/16384.0;
		Read_Value->GX_Axis= (int16_t)SensorData->GX_axis/131.0;
		Read_Value->GY_Axis= (int16_t)SensorData->GY_axis/131.0;
		Read_Value->GZ_Axis= (int16_t)SensorData->GZ_axis/131.0;
		MPU6050_Angles(Read_Value);
}
void MPU6050_Angles(MPU6050_GetData *GetValue)
{

	GetValue->Roll  =atan (GetValue->Y_Axis/sqrt(Square(GetValue->X_Axis) + Square(GetValue->Z_Axis)))*RAD_TO_DEG;
	GetValue->Pitch =atan (GetValue->X_Axis/sqrt(Square(GetValue->Y_Axis) + Square(GetValue->Z_Axis)))*RAD_TO_DEG;
	GetValue->Yaw   = atan(GetValue->Z_Axis/sqrt(Square(GetValue->X_Axis) + Square(GetValue->Z_Axis)))*RAD_TO_DEG;

}
