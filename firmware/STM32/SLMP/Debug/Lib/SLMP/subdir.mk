################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Lib/SLMP/SLMPRequestBuilder.cpp \
../Lib/SLMP/SLMPResponceParser.cpp 

OBJS += \
./Lib/SLMP/SLMPRequestBuilder.o \
./Lib/SLMP/SLMPResponceParser.o 

CPP_DEPS += \
./Lib/SLMP/SLMPRequestBuilder.d \
./Lib/SLMP/SLMPResponceParser.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/SLMP/SLMPRequestBuilder.o: ../Lib/SLMP/SLMPRequestBuilder.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I../Inc -I../Drivers/CMSIS/Include -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/overrided" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/display" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Lib/SLMP/SLMPRequestBuilder.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Lib/SLMP/SLMPResponceParser.o: ../Lib/SLMP/SLMPResponceParser.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I../Inc -I../Drivers/CMSIS/Include -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/overrided" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/display" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Lib/SLMP/SLMPResponceParser.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

