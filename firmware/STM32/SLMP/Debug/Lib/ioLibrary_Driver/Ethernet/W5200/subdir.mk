################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib/ioLibrary_Driver/Ethernet/W5200/w5200.c 

OBJS += \
./Lib/ioLibrary_Driver/Ethernet/W5200/w5200.o 

C_DEPS += \
./Lib/ioLibrary_Driver/Ethernet/W5200/w5200.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/ioLibrary_Driver/Ethernet/W5200/w5200.o: ../Lib/ioLibrary_Driver/Ethernet/W5200/w5200.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lib/ioLibrary_Driver/Ethernet/W5200/w5200.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

