#include "MPU6050_LIB.h"

#ifndef STM32_FAMILY
#define STM32_FAMILY 4  // Change this value to toggle between the different families
#endif

// Depending on the value of STM32_FAMILY, include the appropriate header files.
#if STM32_FAMILY == 1
  #include "stm32f1xx_hal.h"
  #include "stm32f1xx_hal_i2c.h"
#elif STM32_FAMILY == 2
  #include "stm32f2xx_hal.h"
  #include "stm32f2xx_hal_i2c.h"
#elif STM32_FAMILY == 3
  #include "stm32f3xx_hal.h"
  #include "stm32f3xx_hal_i2c.h"
#elif STM32_FAMILY == 4
  #include "stm32f4xx_hal.h"
  #include "stm32f4xx_hal_i2c.h"
#else
  #error "Invalid STM32 family selection"
#endif

uint8_t MPU6050_Init(MPU6050_ConfigTypeDef *config) {
	I2C_HandleTypeDef *handleI2C = config->hi2c;
	uint8_t addr = config->address;
	uint8_t dlpfFsyncConf = config->dlpfFsyncConfig;
	uint8_t smplRateConf = config->smplRateDivConfig;
	uint8_t pwrMgmt1Conf = config->pwrMgmt1Config;
	uint8_t pwrMgmt2Conf = config->pwrMgmt2Config;
	uint8_t accelConf = config->accelConfig;
	uint8_t gyroConf = config->gyroConfig;

	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_CONFIG, I2C_MEMADD_SIZE_8BIT, &dlpfFsyncConf, sizeof(dlpfFsyncConf), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT, &smplRateConf, sizeof(smplRateConf), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &pwrMgmt1Conf, sizeof(pwrMgmt1Conf), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_PWR_MGMT_2, I2C_MEMADD_SIZE_8BIT, &pwrMgmt2Conf, sizeof(pwrMgmt2Conf), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &accelConf, sizeof(accelConf), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &gyroConf, sizeof(gyroConf), MPU6050_TIMEOUT_MS);

	uint8_t checkData;
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_CONFIG, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != dlpfFsyncConf){
		return ERR_INIT_0;
	}
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != smplRateConf){
		return ERR_INIT_1;
	}
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != pwrMgmt1Conf){
		return ERR_INIT_2;
	}
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_PWR_MGMT_2, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != pwrMgmt2Conf){
		return ERR_INIT_3;
	}
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != accelConf){
		return ERR_INIT_4;
	}
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != gyroConf){
		return ERR_INIT_5;
	}

	return INIT_OK;
}

uint8_t MPU6050_Test_Conn(MPU6050_ConfigTypeDef *config) {
	uint8_t addr = config->address;
	I2C_HandleTypeDef *handleI2C = config->hi2c;

	uint8_t checkData;
	HAL_I2C_Mem_Read(handleI2C, addr, REG_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);

	if(0x68 == checkData){
		return TEST_OK;
	}
	else{
		return ERR_CONN_0;
	}
}


