/*
 * MPU6050_LIB.h
 * Author: Andres Aguinaga Lopez
 * License: GNU General Public License v3.0
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * Disclaimer:
 * This software is provided "as is," without warranty of any kind, express
 * or implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose, and non-infringement. In no event shall
 * the authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising
 * from, out of or in connection with the software or the use or other
 * dealings in the software.
 */

#ifndef MPU6050_LIB
#define MPU6050_LIB

#include <stdint.h>

#define STM32_FAMILY 4  // Change this value to toggle between the different families

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

// MPU6050 Configuration structure
typedef struct {
    I2C_HandleTypeDef *hi2c;		// I2C interface used
    uint8_t address;				// I2C address chosen

    								// Registers written with these values
    uint8_t dlpfFsyncConfig;		// REG_CONFIG
    uint8_t smplRateDivConfig;		// REG_SMPLRT_DIV
    uint8_t pwrMgmt1Config;			// REG_PWR_MGMT_1
    uint8_t pwrMgmt2Config;			// REG_PWR_MGMT_2
    uint8_t accelConfig;			// REG_ACCEL_CONFIG
    uint8_t gyroConfig;				// REG_GYRI_CONFIG
} MPU6050_ConfigTypeDef;

// MPU6050 Acceleration Data structure
typedef struct {
	int16_t rawAccelX;
	int16_t rawAccelY;
	int16_t rawAccelZ;

	float convertedAccelX;
	float convertedAccelY;
	float convertedAccelZ;
} MPU6050_Accelerations;

// MPU6050 Gyroscope Data structure
typedef struct {
	int16_t rawRotaX;
	int16_t rawRotaY;
	int16_t rawRotaZ;

	float convertedRotaX;
	float convertedRotaY;
	float convertedRotaZ;
} MPU6050_Rotations;

// MPU6050 Accelerometer Offset structure
typedef struct {
    int16_t xOffset;
    int16_t yOffset;
    int16_t zOffset;
} MPU6050_AccelOffsets;

// MPU6050 Gyroscope Offset structure
typedef struct {
    int16_t xOffset;
    int16_t yOffset;
    int16_t zOffset;
} MPU6050_GyroOffsets;

// Errors enumeration
typedef enum {
    INIT_OK = 0,
    ERR_INIT_0,
    ERR_INIT_1,
    ERR_INIT_2,
    ERR_INIT_3,
    ERR_INIT_4,
    ERR_INIT_5,
} InitializationError;

typedef enum {
	CONN_OK = 0,
	ERR_CONN_0
} ConnectionError;

typedef enum {
	CONFIG_OK = 0,
	ERR_CONFIG_ACCEL,
	ERR_CONFIG_GYRO
} ConfigurationError;

typedef enum {
	WRITE_OK = 0,
	ERR_WRITE_OFF_X,
	ERR_WRITE_OFF_Y,
	ERR_WRITE_OFF_Z
} WritingError;

// Parameters and constants
#define GRAVITY_ACCEL			9.80665f // m/s^2

#define ACCEL_LSB_SEN_0			16384	// LSB/g
#define ACCEL_LSB_SEN_1			8192
#define ACCEL_LSB_SEN_2			4096
#define ACCEL_LSB_SEN_3			2048

#define GET_ACCEL_FS_CONFIG		0b00011000	// BitMask to get AFS bits

#define GYRO_LSB_SEN_0			131.0f	// LSB/º/S
#define GYRO_LSB_SEN_1			65.5f
#define GYRO_LSB_SEN_2			32.8f
#define GYRO_LSB_SEN_3			16.4f

#define GET_GYRO_FS_CONFIG		0b00011000	// BitMask to get GFS bits
/********END OF PARAMETERS AND CONSTANTS*********/

// Definitions of configuration values

// I2C Configuration
#define MPU6050_TIMEOUT_MS		100

// MPU6050 Configuration

// I2C address is chosen by the logical state of AD0
#define MPU6050_ADDRESS_AD0_L	0x68	// AD0 = 0
#define MPU6050_ADDRESS_AD0_H	0x69	// AD0 = 1

// Configuration values for register REG_CONFIG
											// FSYNC BIT LOCATION
#define FSYNC_CONFIG_0			0b000000	// INPUT DISABLED
#define FSYNC_CONFIG_1			0b001000	// TEMP_OUT_L[0]
#define FSYNC_CONFIG_2			0b010000	// GYRO_XOUT_L[0]
#define FSYNC_CONFIG_3			0b011000	// GYRO_YOUT_L[0]
#define FSYNC_CONFIG_4			0b100000	// GYRO_ZOUT_L[0]
#define FSYNC_CONFIG_5			0b101000	// ACCEL_XOUT_L[0]
#define FSYNC_CONFIG_6			0b110000	// ACCEL_YOUT_L[0]
#define FSYNC_CONFIG_7			0b111000	// ACCEL_ZOUT_L[0]

											// DELAY ACCEL:GYRO:FSGYRO (Hz)
#define DLPF_CONFIG_0			0b000000	// 0.00 : 0.98 : 8000
#define DLPF_CONFIG_1			0b000001	// 2.00 : 1.90 : 1000
#define DLPF_CONFIG_2			0b000010	// 3.00 : 2.80 : 1000
#define DLPF_CONFIG_3			0b000011	// 4.90 : 4.80 : 1000
#define DLPF_CONFIG_4			0b000100	// 8.50 : 8.30 : 1000
#define DLPF_CONFIG_5			0b000101	// 13.8 : 13.4 : 1000
#define DLPF_CONFIG_6			0b000110	// 19.0 : 18.6 : 1000

/*******************END OF REG_CONFIG CONFIGURATION VALUES****************/

// NOTE: SMPLRT_DIV REGISTER IS CONFIGURED BY A 8-BITS UNSIGNED INTEGER
// S_RATE = GYRO_OUTPUT_RATE / (1 + SMPLRT_DIV)

// Configuration values for register REG_PWR_MGMT_1

#define CYCLE_CONFIG_SET		0b00100000	// CYCLE MODE ENABLED
#define TEMP_DIS_CONFIG_SET		0b00001000  // TEMP SENSOR DISABLED

#define CLKSEL_CONFIG_0			0b00000000  // INTERNAL 8MHz OSC
#define CLKSEL_CONFIG_1			0b00000001  // PLL WITH X AXIS GYRO REF
#define CLKSEL_CONFIG_2			0b00000010	// PLL WITH Y AXIS GYRO REF
#define CLKSEL_CONFIG_3			0b00000011  // PLL WITH Z AXIS GYRO REF
#define CLKSEL_CONFIG_4			0b00000100  // PLL WITH EXT-RTC OSC
#define CLKSEL_CONFIG_5			0b00000101	// PLL WITH EXT 19.2MHz OSC
#define CLKSEL_CONFIG_7			0b00000111  // TIMING GENERATOR IN RESET

/***************END OF REG_PWR_MGMT_1 CONFIGURATION VALUES*************/

// Configuration values for register REG_PWR_MGMT_2

											// WAKE UP FREQUENCY (Hz)
#define LP_WAKE_CTRL_CONFIG_0	0b00000000	// 1.25
#define LP_WAKE_CTRL_CONFIG_1	0b01000000	// 5.00
#define LP_WAKE_CTRL_CONFIG_2	0b10000000	// 20.0
#define LP_WAKE_CTRL_CONFIG_3	0b11000000	// 40.0

/*  NOTE: The MPU6050 can be put into Accelerometer Only Low Power Mode using the following steps:
	(i) Set CYCLE bit to 1
	(ii) Set SLEEP bit to 0
	(iii) Set TEMP_DIS bit to 1
	(iv) Set STBY_XG, STBY_YG, STBY_ZG bits to 1 */

											// STANDBY MODE ENABLE
#define STBY_XA_CONFIG_SET		0b00100000
#define STBY_YA_CONFIG_SET		0b00010000
#define STBY_ZA_CONFIG_SET		0b00001000

#define STBY_XG_CONFIG_SET		0b00000100
#define STBY_YG_CONFIG_SET		0b00000010
#define STBY_ZG_CONFIG_SET		0b00000001

/*****************END OF REG_PWR_MGMT_2 CONFIGURATION VALUES************************************/

// Configuration values for register REG_ACCEL_CONFIG

											// AFS_SEL:FS_RANGE:LSB SENSITIVITY
#define ACCEL_CONFIG_SCALE_0	0b00000000  // 0:+-2g:16384LSB/g
#define ACCEL_CONFIG_SCALE_1	0b00001000  // 1:+-4g:8192LSB/g
#define ACCEL_CONFIG_SCALE_2	0b00010000  // 2:+-8g:4096LSB/g
#define ACCEL_CONFIG_SCALE_3	0b00011000  // 3:+-16g:2048LSB/g

#define ACCEL_CONFIG_STEST_X	0b10000000	// Self test enables
#define ACCEL_CONFIG_STEST_Y	0b01000000
#define ACCEL_CONFIG_STEST_Z	0b00100000

/*************END OF REG_ACCEL_CONFIG CONFIGURATION VALUES********************/

// Configuration values for register REG_GYRO_CONFIG

											// GFS_SEL:GFS_RANGE:LSB SENSITIVITY
#define GYRO_CONFIG_SCALE_0		0b00000000  // 0:+-250º/s:131LSB/º/S
#define GYRO_CONFIG_SCALE_1		0b00001000  // 1:+-500º/s:65.5LSB/º/S
#define GYRO_CONFIG_SCALE_2		0b00010000  // 2:+-1000º/s:32.8LSB/º/S
#define GYRO_CONFIG_SCALE_3		0b00011000  // 3:+-2000º/s:16.4LSB/º/S

#define GYRO_CONFIG_STEST_X		0b10000000	// Self test enables
#define GYRO_CONFIG_STEST_Y		0b01000000
#define GYRO_CONFIG_STEST_Z		0b00100000

/*************END OF REG_GYRO_CONFIG CONFIGURATION VALUES**********************/

// MPU6050 Register Map
#define REG_XA_OFFS_USRH		0x06
#define REG_XA_OFFS_USRL		0x07
#define REG_YA_OFFS_USRH		0x08
#define REG_YA_OFFS_USRL		0x09
#define REG_ZA_OFFS_USRH		0x0A
#define REG_ZA_OFFS_USRL		0x0B

#define REG_XG_OFFS_USRH		0x13
#define REG_XG_OFFS_USRL		0x14
#define REG_YG_OFFS_USRH		0x15
#define REG_YG_OFFS_USRL		0x16
#define REG_ZG_OFFS_USRH		0x17
#define REG_ZG_OFFS_USRL		0x18

#define REG_SELF_TEST_X      	0x0D
#define REG_SELF_TEST_Y      	0x0E
#define REG_SELF_TEST_Z      	0x0F
#define REG_SELF_TEST_A      	0x10

#define REG_SMPLRT_DIV       	0x19	// S_RATE = G_OUTPUT_RATE / (1 + SMPLRT_DIV)
#define REG_CONFIG           	0x1A
#define REG_GYRO_CONFIG      	0x1B	// GFS_SEL(1:0) B4:B3
#define REG_ACCEL_CONFIG     	0x1C	// AFS_SEL(1:0) B4:B3

#define REG_FIFO_EN          	0x23

#define REG_I2C_MST_CTRL     	0x24

#define REG_I2C_SLV0_ADDR    	0x25
#define REG_I2C_SLV0_REG     	0x26
#define REG_I2C_SLV0_CTRL    	0x27

#define REG_I2C_SLV1_ADDR    	0x28
#define REG_I2C_SLV1_REG     	0x29
#define REG_I2C_SLV1_CTRL    	0x2A

#define REG_I2C_SLV2_ADDR    	0x2B
#define REG_I2C_SLV2_REG     	0x2C
#define REG_I2C_SLV2_CTRL    	0x2D

#define REG_I2C_SLV3_ADDR    	0x2E
#define REG_I2C_SLV3_REG     	0x2F
#define REG_I2C_SLV3_CTRL    	0x30

#define REG_I2C_SLV4_ADDR    	0x31
#define REG_I2C_SLV4_REG     	0x32
#define REG_I2C_SLV4_DO      	0x33
#define REG_I2C_SLV4_CTRL    	0x34
#define REG_I2C_SLV4_DI      	0x35

#define REG_I2C_MST_STATUS   	0x36

#define REG_INT_PIN_CFG      	0x37
#define REG_INT_ENABLE       	0x38
#define REG_INT_STATUS       	0x3A

#define REG_ACCEL_XOUT_H     	0x3B
#define REG_ACCEL_XOUT_L     	0x3C
#define REG_ACCEL_YOUT_H     	0x3D
#define REG_ACCEL_YOUT_L     	0x3E
#define REG_ACCEL_ZOUT_H     	0x3F
#define REG_ACCEL_ZOUT_L     	0x40

#define REG_TEMP_OUT_H       	0x41
#define REG_TEMP_OUT_L       	0x42

#define REG_GYRO_XOUT_H      	0x43
#define REG_GYRO_XOUT_L      	0x44
#define REG_GYRO_YOUT_H      	0x45
#define REG_GYRO_YOUT_L      	0x46
#define REG_GYRO_ZOUT_H      	0x47
#define REG_GYRO_ZOUT_L      	0x48

#define REG_EXT_SENS_DATA_00 	0x49
#define REG_EXT_SENS_DATA_01 	0x4A
#define REG_EXT_SENS_DATA_02 	0x4B
#define REG_EXT_SENS_DATA_03 	0x4C
#define REG_EXT_SENS_DATA_04 	0x4D
#define REG_EXT_SENS_DATA_05 	0x4E
#define REG_EXT_SENS_DATA_06 	0x4F
#define REG_EXT_SENS_DATA_07 	0x50
#define REG_EXT_SENS_DATA_08 	0x51
#define REG_EXT_SENS_DATA_09 	0x52
#define REG_EXT_SENS_DATA_10 	0x53
#define REG_EXT_SENS_DATA_11 	0x54
#define REG_EXT_SENS_DATA_12 	0x55
#define REG_EXT_SENS_DATA_13 	0x56
#define REG_EXT_SENS_DATA_14 	0x57
#define REG_EXT_SENS_DATA_15 	0x58
#define REG_EXT_SENS_DATA_16 	0x59
#define REG_EXT_SENS_DATA_17 	0x5A
#define REG_EXT_SENS_DATA_18 	0x5B
#define REG_EXT_SENS_DATA_19 	0x5C
#define REG_EXT_SENS_DATA_20 	0x5D
#define REG_EXT_SENS_DATA_21 	0x5E
#define REG_EXT_SENS_DATA_22 	0x5F
#define REG_EXT_SENS_DATA_23 	0x60

#define REG_I2C_SLV0_DO      	0x63
#define REG_I2C_SLV1_DO      	0x64
#define REG_I2C_SLV2_DO      	0x65
#define REG_I2C_SLV3_DO      	0x66
#define REG_I2C_MST_DELAY_CTRL 	0x67

#define REG_SIGNAL_PATH_RESET  	0x68
#define REG_USER_CTRL       	0x6A

#define REG_PWR_MGMT_1      	0x6B
#define REG_PWR_MGMT_2      	0x6C

#define REG_FIFO_COUNTH     	0x72
#define REG_FIFO_COUNTL     	0x73
#define REG_FIFO_R_W        	0x74

#define REG_WHO_AM_I        	0x75

// FUNCTIONS PROTOTYPES
uint8_t MPU6050_Init(MPU6050_ConfigTypeDef *config);
uint8_t MPU6050_Test_Conn(MPU6050_ConfigTypeDef *config);
uint8_t MPU6050_GetAcceleration(MPU6050_ConfigTypeDef *config, MPU6050_Accelerations *accel);
uint8_t MPU6050_GetRotation(MPU6050_ConfigTypeDef *config, MPU6050_Rotations *rota);
uint8_t MPU6050_GetAccelOffset(MPU6050_ConfigTypeDef *config, MPU6050_AccelOffsets *accelOff);

// FUNCTIONS LIKE-MACROS
#define MPU6050_RAW_TO_F_DATA(rawData, lsbSen) ( ((float)(rawData)/(float)(lsbSen)) * GRAVITY_ACCEL)

#endif /* MPU6050_LIB */
