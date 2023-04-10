################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctions.c \
../Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctionsF16.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_cos_f32.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q15.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q31.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q15.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q31.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_sin_f32.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q15.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q31.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q15.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q31.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f16.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f32.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f64.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_vinverse_f16.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f16.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f32.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f64.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q15.c \
../Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q31.c 

OBJS += \
./Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctions.o \
./Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctionsF16.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_f32.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q15.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q31.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q15.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q31.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_f32.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q15.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q31.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q15.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q31.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f16.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f32.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f64.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vinverse_f16.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f16.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f32.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f64.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q15.o \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q31.o 

C_DEPS += \
./Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctions.d \
./Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctionsF16.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_f32.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q15.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q31.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q15.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q31.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_f32.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q15.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q31.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q15.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q31.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f16.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f32.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f64.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vinverse_f16.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f16.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f32.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f64.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q15.d \
./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q31.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Source/FastMathFunctions/%.o Drivers/CMSIS/Source/FastMathFunctions/%.su: ../Drivers/CMSIS/Source/FastMathFunctions/%.c Drivers/CMSIS/Source/FastMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I"C:/Users/Bobon/Documents/GitHub/newseqen/Drivers/CMSIS/Include/dsp" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Bobon/Documents/GitHub/sound_slave1/extras for cmsis" -I"C:/Users/Bobon/Documents/GitHub/newseqen/samples" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Source-2f-FastMathFunctions

clean-Drivers-2f-CMSIS-2f-Source-2f-FastMathFunctions:
	-$(RM) ./Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctions.d ./Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctions.o ./Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctions.su ./Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctionsF16.d ./Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctionsF16.o ./Drivers/CMSIS/Source/FastMathFunctions/FastMathFunctionsF16.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_f32.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_f32.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_f32.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q15.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q15.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q15.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q31.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q31.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_cos_q31.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q15.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q15.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q15.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q31.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q31.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_divide_q31.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_f32.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_f32.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_f32.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q15.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q15.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q15.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q31.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q31.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_sin_q31.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q15.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q15.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q15.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q31.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q31.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_sqrt_q31.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f16.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f16.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f16.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f32.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f32.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f32.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f64.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f64.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_vexp_f64.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_vinverse_f16.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_vinverse_f16.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_vinverse_f16.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f16.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f16.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f16.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f32.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f32.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f32.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f64.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f64.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_f64.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q15.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q15.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q15.su ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q31.d ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q31.o ./Drivers/CMSIS/Source/FastMathFunctions/arm_vlog_q31.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Source-2f-FastMathFunctions

