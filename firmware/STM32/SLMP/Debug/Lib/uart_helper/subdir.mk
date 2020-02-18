################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Lib/uart_helper/uart_helper.cpp 

OBJS += \
./Lib/uart_helper/uart_helper.o 

CPP_DEPS += \
./Lib/uart_helper/uart_helper.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/uart_helper/uart_helper.o: ../Lib/uart_helper/uart_helper.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I../Inc -I../Drivers/CMSIS/Include -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/overrided" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/display" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/chips" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/udpClient" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/uart_helper" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Lib/uart_helper/uart_helper.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

