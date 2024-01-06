# MPU6050_LIB
The MPU6050_LIB is a library designed for interfacing with the MPU6050 sensor on microcontrollers belonging to the STM32Fx family. This library leverages the STM32 Hardware Abstraction Layer (HAL) and is intended to simplify the integration of the MPU6050 sensor, providing an easy-to-use interface for STM32Fx microcontroller users.
# How to Add MPU6050_LIB to Your STM32 Project

To integrate the `MPU6050_LIB` library into your STM32 project, follow these steps:

1. Clone the `MPU6050_LIB` repository to your local machine.

    ```bash
    git clone https://github.com/AndresAL1/MPU6050_LIB.git
    ```

2. Copy the contents of the `inc` folder (header files) to your project's include directory.

3. Copy the contents of the `src` folder (source files) to your project's source directory.

4. Include the main header file in your source code.

    ```c
    #include "MPU6050_LIB.h"
    ```

5. Choose STM32 Family

    In the "MPU6050_LIB.h" header file, choose your STM32 family by setting the value of `STM32_FAMILY`. This will automatically include the appropriate STM32 HAL header files.

    ```c
    #define STM32_FAMILY 4

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
    ```

6. Build and compile your STM32 project to include the `MPU6050_LIB` functionality.

That's it! You have successfully added the `MPU6050_LIB` library to your STM32 project.

## MPU6050 Configuration

To configure the MPU6050 sensor, you need to initialize the `MPU6050_ConfigTypeDef` structure. This structure includes various configuration parameters for the sensor, such as I2C interface, I2C address, and register settings.

```c
// MPU6050 Configuration structure
typedef struct {
    I2C_HandleTypeDef *hi2c;        // I2C interface used
    uint8_t address;                // I2C address chosen

                                    // Registers written with these values
    uint8_t dlpfFsyncConfig;        // REG_CONFIG
    uint8_t smplRateDivConfig;      // REG_SMPLRT_DIV
    uint8_t pwrMgmt1Config;         // REG_PWR_MGMT_1
    uint8_t pwrMgmt2Config;         // REG_PWR_MGMT_2
    uint8_t accelConfig;            // REG_ACCEL_CONFIG
    uint8_t gyroConfig;             // REG_GYRO_CONFIG
} MPU6050_ConfigTypeDef;
```
## Example Initialization
Here's an example of how to initialize the structure:
```c
MPU6050_ConfigTypeDef mpu6050 = {
    .hi2c = &hi2c1,
    .address = MPU6050_ADDRESS_AD0_L,
    .dlpfFsyncConfig = FSYNC_CONFIG_0 | DLPF_CONFIG_0,
    .smplRateDivConfig = 0x0,
    .pwrMgmt1Config = TEMP_DIS_CONFIG_SET | CLKSEL_CONFIG_0,
    .pwrMgmt2Config = 0x0,
    .accelConfig = ACCEL_CONFIG_SCALE_0,
    .gyroConfig = GYRO_CONFIG_SCALE_0
};
```

## MPU6050 Configuration

To configure the MPU6050 sensor, you need to initialize the `MPU6050_ConfigTypeDef` structure. This structure includes various configuration parameters for the sensor, such as I2C interface, I2C address, and register settings.

```c
// MPU6050 Configuration structure
typedef struct {
    I2C_HandleTypeDef *hi2c;        // I2C interface used
    uint8_t address;                // I2C address chosen

                                    // Registers written with these values
    uint8_t dlpfFsyncConfig;        // REG_CONFIG
    uint8_t smplRateDivConfig;      // REG_SMPLRT_DIV
    uint8_t pwrMgmt1Config;         // REG_PWR_MGMT_1
    uint8_t pwrMgmt2Config;         // REG_PWR_MGMT_2
    uint8_t accelConfig;            // REG_ACCEL_CONFIG
    uint8_t gyroConfig;             // REG_GYRO_CONFIG
} MPU6050_ConfigTypeDef;
```

## Example Initialization

Here's an example of how to initialize the structure:

```c
MPU6050_ConfigTypeDef mpu6050 = {
    .hi2c = &hi2c1,
    .address = MPU6050_ADDRESS_AD0_L,
    .dlpfFsyncConfig = FSYNC_CONFIG_0 | DLPF_CONFIG_0,
    .smplRateDivConfig = 0x0,
    .pwrMgmt1Config = TEMP_DIS_CONFIG_SET | CLKSEL_CONFIG_0,
    .pwrMgmt2Config = 0x0,
    .accelConfig = ACCEL_CONFIG_SCALE_0,
    .gyroConfig = GYRO_CONFIG_SCALE_0
};
```

The configuration values are defined in the header file under `Definitions of configuration values.`

## Initialization Function

Once the structure is set up, use the following function to write the configuration values to the sensor registers:
```c
uint8_t result = MPU6050_Init(&mpu6050);
```
If the initialization is successful, the function returns `INIT_OK`. Otherwise, it returns an error code (`ERR_INIT_X`), where `X` is the index (0 to 5) of the configuration parameter that failed to write:

0. `dlpfFsyncConfig (REG_CONFIG)`
1. `smplRateDivConfig (REG_SMPLRT_DIV)`
2. `pwrMgmt1Config (REG_PWR_MGMT_1)`
3. `pwrMgmt2Config (REG_PWR_MGMT_2)`
4. `accelConfig (REG_ACCEL_CONFIG)`
5. `gyroConfig (REG_GYRI_CONFIG)`
