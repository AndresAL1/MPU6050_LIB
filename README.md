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

