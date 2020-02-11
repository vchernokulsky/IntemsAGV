################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/freertos.c \
../Src/main.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_hal_timebase_tim.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f4xx.c 

CPP_SRCS += \
../Src/SocketClient.cpp \
../Src/UartHelper.cpp \
../Src/W5500_chip.cpp \
../Src/mainpp.cpp 

OBJS += \
./Src/SocketClient.o \
./Src/UartHelper.o \
./Src/W5500_chip.o \
./Src/freertos.o \
./Src/main.o \
./Src/mainpp.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_hal_timebase_tim.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f4xx.o 

C_DEPS += \
./Src/freertos.d \
./Src/main.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_hal_timebase_tim.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f4xx.d 

CPP_DEPS += \
./Src/SocketClient.d \
./Src/UartHelper.d \
./Src/W5500_chip.d \
./Src/mainpp.d 


# Each subdirectory must supply rules for building sources it contributes
Src/SocketClient.o: ../Src/SocketClient.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/SocketClient.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/UartHelper.o: ../Src/UartHelper.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/UartHelper.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/W5500_chip.o: ../Src/W5500_chip.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/W5500_chip.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/freertos.o: ../Src/freertos.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/freertos.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/mainpp.o: ../Src/mainpp.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/mainpp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f4xx_hal_msp.o: ../Src/stm32f4xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f4xx_hal_timebase_tim.o: ../Src/stm32f4xx_hal_timebase_tim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f4xx_hal_timebase_tim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f4xx_it.o: ../Src/stm32f4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32f4xx.o: ../Src/system_stm32f4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Ethernet" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DNS" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/ioLibrary_Driver/Internet/DHCP" -I"/home/data-scientist/STM32CubeIDE/workspace_1.1.0/SLMP/Lib/SLMP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

