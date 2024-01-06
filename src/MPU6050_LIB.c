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
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);

	if(0x68 == checkData){
		return CONN_OK;
	}
	else{
		return ERR_CONN_0;
	}
}

uint16_t MPU6050_GetAccelSensitivity(MPU6050_ConfigTypeDef *config) {
	uint8_t accelConf = config->accelConfig & GET_ACCEL_FS_CONFIG;

	switch(accelConf){
		case ACCEL_CONFIG_SCALE_0:
			return ACCEL_LSB_SEN_0;
			break;
		case ACCEL_CONFIG_SCALE_1:
			return ACCEL_LSB_SEN_1;
			break;
		case ACCEL_CONFIG_SCALE_2:
			return ACCEL_LSB_SEN_2;
			break;
		case ACCEL_CONFIG_SCALE_3:
			return ACCEL_LSB_SEN_3;
			break;
		default:
			return ERR_CONFIG_ACCEL;
			break;
	}
}

float MPU6050_GetGyroSensitivty(MPU6050_ConfigTypeDef *config) {
	uint8_t gyroConf = config->gyroConfig & GET_GYRO_FS_CONFIG;

	switch(gyroConf){
		case GYRO_CONFIG_SCALE_0:
			return GYRO_LSB_SEN_0;
			break;
		case GYRO_CONFIG_SCALE_1:
			return GYRO_LSB_SEN_1;
			break;
		case GYRO_CONFIG_SCALE_2:
			return GYRO_LSB_SEN_2;
			break;
		case GYRO_CONFIG_SCALE_3:
			return GYRO_LSB_SEN_3;
			break;
		default:
			return (float)ERR_CONFIG_GYRO;
			break;
	}
}

uint8_t MPU6050_GetAcceleration(MPU6050_ConfigTypeDef *config , MPU6050_Accelerations *accel) {
	I2C_HandleTypeDef *handleI2C = config->hi2c;
	uint8_t addr = config->address;
	uint8_t data_L;
	uint8_t data_H;

	if(CONN_OK != MPU6050_Test_Conn(config)){
		return ERR_CONN_0;
	}

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ACCEL_XOUT_L, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	accel->rawAccelX =  (int16_t)(data_H << 8) | data_L;

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ACCEL_YOUT_L, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ACCEL_YOUT_H, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	accel->rawAccelY =  (int16_t)(data_H << 8) | data_L;

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ACCEL_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ACCEL_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	accel->rawAccelZ =  (int16_t)(data_H << 8) | data_L;

	uint16_t lsbSen = MPU6050_GetAccelSensitivity(config);

	accel->convertedAccelX = MPU6050_RAW_TO_F_DATA(accel->rawAccelX, lsbSen);
	accel->convertedAccelY = MPU6050_RAW_TO_F_DATA(accel->rawAccelY, lsbSen);
	accel->convertedAccelZ = MPU6050_RAW_TO_F_DATA(accel->rawAccelZ, lsbSen);

	return CONN_OK;
}

uint8_t MPU6050_GetRotation(MPU6050_ConfigTypeDef *config, MPU6050_Rotations *rota) {
	I2C_HandleTypeDef *handleI2C = config->hi2c;
	uint8_t addr = config->address;
	uint8_t data_L;
	uint8_t data_H;

	if(CONN_OK != MPU6050_Test_Conn(config)){
		return ERR_CONN_0;
	}

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_GYRO_XOUT_L, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_GYRO_XOUT_H, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	rota->rawRotaX =  (int16_t)(data_H << 8) | data_L;

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_GYRO_YOUT_L, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_GYRO_YOUT_H, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	rota->rawRotaY =  (int16_t)(data_H << 8) | data_L;

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_GYRO_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_GYRO_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	rota->rawRotaZ =  (int16_t)(data_H << 8) | data_L;

	float lsbSen = MPU6050_GetGyroSensitivty(config);

	rota->convertedRotaX = MPU6050_RAW_TO_F_DATA(rota->rawRotaX, lsbSen);
	rota->convertedRotaY = MPU6050_RAW_TO_F_DATA(rota->rawRotaY, lsbSen);
	rota->convertedRotaZ = MPU6050_RAW_TO_F_DATA(rota->rawRotaZ, lsbSen);

	return CONN_OK;
}

uint8_t MPU6050_GetAccelOffset(MPU6050_ConfigTypeDef *config, MPU6050_AccelOffsets *accelOff) {
	uint8_t addr = config->address;
	I2C_HandleTypeDef *handleI2C = config->hi2c;
	uint8_t data_L;
	uint8_t data_H;

	if(CONN_OK != MPU6050_Test_Conn(config)){
		return ERR_CONN_0;
	}

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_XA_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_XA_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	accelOff->xOffset =  (int16_t)(data_H << 8) | data_L;

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_YA_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_YA_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	accelOff->yOffset =  (int16_t)(data_H << 8) | data_L;

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ZA_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ZA_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	accelOff->zOffset =  (int16_t)(data_H << 8) | data_L;

	return CONN_OK;
}

uint8_t MPU6050_GetGyroOffset(MPU6050_ConfigTypeDef *config, MPU6050_GyroOffsets *gyroOff) {
	uint8_t addr = config->address;
	I2C_HandleTypeDef *handleI2C = config->hi2c;
	uint8_t data_L;
	uint8_t data_H;

	if(CONN_OK != MPU6050_Test_Conn(config)){
		return ERR_CONN_0;
	}

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_XG_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_XG_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	gyroOff->xOffset =  (int16_t)(data_H << 8) | data_L;

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_YG_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_YG_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	gyroOff->yOffset =  (int16_t)(data_H << 8) | data_L;

	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ZG_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ZG_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	gyroOff->zOffset =  (int16_t)(data_H << 8) | data_L;

	return CONN_OK;
}

uint8_t MPU6050_SetAccelOffset(MPU6050_ConfigTypeDef *config, MPU6050_AccelOffsets *accelOff) {
	uint8_t addr = config->address;
	I2C_HandleTypeDef *handleI2C = config->hi2c;
	uint8_t data_L;
	uint8_t data_H;
	uint8_t checkData;

	data_L = accelOff->xOffset & LOW_BYTE_MASK;
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_XA_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_XA_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_L){
		return ERR_WRITE_OFF_X_L;
	}

	data_H = ((accelOff->xOffset & HIGH_BYTE_MASK) >> (8));
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_XA_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_XA_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_H){
		return ERR_WRITE_OFF_X_H;
	}

	data_L = accelOff->yOffset & LOW_BYTE_MASK;
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_YA_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_YA_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_L){
		return ERR_WRITE_OFF_Y_L;
	}

	data_H = ((accelOff->yOffset & HIGH_BYTE_MASK) >> (8));
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_YA_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_YA_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_H){
		return ERR_WRITE_OFF_Y_H;
	}

	data_L = accelOff->zOffset & LOW_BYTE_MASK;
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_ZA_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ZA_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_L){
		return ERR_WRITE_OFF_Z_L;
	}

	data_H = ((accelOff->zOffset & HIGH_BYTE_MASK) >> (8));
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_ZA_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ZA_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_H){
		return ERR_WRITE_OFF_Z_H;
	}

	return WRITE_OK;
}

uint8_t MPU6050_SetGyroOffset(MPU6050_ConfigTypeDef *config, MPU6050_GyroOffsets *gyroOff) {
	uint8_t addr = config->address;
	I2C_HandleTypeDef *handleI2C = config->hi2c;
	uint8_t data_L;
	uint8_t data_H;
	uint8_t checkData;

	data_L = gyroOff->xOffset & LOW_BYTE_MASK;
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_XG_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_XG_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_L){
		return ERR_WRITE_OFF_X_L;
	}

	data_H = ((gyroOff->xOffset & HIGH_BYTE_MASK) >> (8));
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_XG_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_XG_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_H){
		return ERR_WRITE_OFF_X_H;
	}

	data_L = gyroOff->yOffset & LOW_BYTE_MASK;
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_YG_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_YG_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_L){
		return ERR_WRITE_OFF_Y_L;
	}

	data_H = ((gyroOff->yOffset & HIGH_BYTE_MASK) >> (8));
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_YG_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_YG_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_H){
		return ERR_WRITE_OFF_Y_H;
	}

	data_L = gyroOff->zOffset & LOW_BYTE_MASK;
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_ZG_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &data_L, sizeof(data_L), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ZG_OFFS_USRL, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_L){
		return ERR_WRITE_OFF_Z_L;
	}

	data_H = ((gyroOff->zOffset & HIGH_BYTE_MASK) >> (8));
	HAL_I2C_Mem_Write(handleI2C, addr<<1, REG_ZG_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &data_H, sizeof(data_H), MPU6050_TIMEOUT_MS);
	HAL_I2C_Mem_Read(handleI2C, addr<<1, REG_ZG_OFFS_USRH, I2C_MEMADD_SIZE_8BIT, &checkData, sizeof(checkData), MPU6050_TIMEOUT_MS);
	if(checkData != data_H){
		return ERR_WRITE_OFF_Z_H;
	}

	return WRITE_OK;
}

uint8_t MPU6050_CalibAccel(MPU6050_ConfigTypeDef *config, float calibTolerance) {
    uint32_t readingsCount = 0;
    uint32_t iterationsCount = 0;

    MPU6050_Accelerations accel;
    MPU6050_AccelOffsets accelOff;

    int32_t avgAccelX = 0;
    int32_t avgAccelY = 0;
    int32_t avgAccelZ = 0;

    MPU6050_GetAccelOffset(config, &accelOff);

    uint16_t lsbSen = MPU6050_GetAccelSensitivity(config);

    uint16_t maxStableError = (uint16_t)(calibTolerance * lsbSen);

    while (iterationsCount < MPU6050_MAX_CALIB_ITERATIONS) {
        readingsCount = 0;
        avgAccelX = 0;
        avgAccelY = 0;
        avgAccelZ = 0;

        while (readingsCount < MPU6050_NUM_CALIB_READINGS) {
            MPU6050_GetAcceleration(config, &accel);

            avgAccelX += accel.rawAccelX;
            avgAccelY += accel.rawAccelY;
            avgAccelZ += accel.rawAccelZ;

            readingsCount++;
        }

        avgAccelX /= MPU6050_NUM_CALIB_READINGS;
        avgAccelY /= MPU6050_NUM_CALIB_READINGS;
        avgAccelZ /= MPU6050_NUM_CALIB_READINGS;

        if (ABS(avgAccelX) <= maxStableError && ABS(avgAccelY) <= maxStableError && ABS(avgAccelZ - lsbSen) <= maxStableError) {
            MPU6050_SetAccelOffset(config, &accelOff);
            return ACCEL_CALIB_OK;
        } else {
            if (avgAccelX > 0) {
                accelOff.xOffset--;
            } else {
                accelOff.xOffset++;
            }

            if (avgAccelY > 0) {
                accelOff.yOffset--;
            } else {
                accelOff.yOffset++;
            }

            if ((avgAccelZ - lsbSen) > 0) {
                accelOff.zOffset--;
            } else {
                accelOff.zOffset++;
            }

            MPU6050_SetAccelOffset(config, &accelOff);
        }

        iterationsCount++;
    }

    return ACCEL_CALIB_TIMEOUT;
}








