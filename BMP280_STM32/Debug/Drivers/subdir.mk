################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/bmp280.c 

OBJS += \
./Drivers/bmp280.o 

C_DEPS += \
./Drivers/bmp280.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o Drivers/%.su Drivers/%.cyclo: ../Drivers/%.c Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"C:/Users/ACER/STM32CubeIDE/workspace_1.15.0/F401_BMP280/Drivers" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -include"C:/Users/ACER/STM32CubeIDE/workspace_1.15.0/F401_BMP280/Drivers/bmp280.h" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers

clean-Drivers:
	-$(RM) ./Drivers/bmp280.cyclo ./Drivers/bmp280.d ./Drivers/bmp280.o ./Drivers/bmp280.su

.PHONY: clean-Drivers

