################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.c \
../Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.c \
../Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.c \
../Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.c \
../Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.c \
../Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.c \
../Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_f32.c \
../Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.c \
../Drivers/CMSIS/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.c 

OBJS += \
./Drivers/CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.o \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.o \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.o \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.o \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.o \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.o \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_f32.o \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.o \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.o 

C_DEPS += \
./Drivers/CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.d \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.d \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.d \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.d \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.d \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.d \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_f32.d \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.d \
./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Source/QuaternionMathFunctions/%.o Drivers/CMSIS/Source/QuaternionMathFunctions/%.su: ../Drivers/CMSIS/Source/QuaternionMathFunctions/%.c Drivers/CMSIS/Source/QuaternionMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I"C:/Users/Bobon/Documents/GitHub/newseqen/Drivers/CMSIS/Include/dsp" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Bobon/Documents/GitHub/sound_slave1/extras for cmsis" -I"C:/Users/Bobon/Documents/GitHub/newseqen/samples" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Source-2f-QuaternionMathFunctions

clean-Drivers-2f-CMSIS-2f-Source-2f-QuaternionMathFunctions:
	-$(RM) ./Drivers/CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.d ./Drivers/CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.o ./Drivers/CMSIS/Source/QuaternionMathFunctions/QuaternionMathFunctions.su ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.d ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.o ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.su ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.d ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.o ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.su ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.d ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.o ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.su ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.d ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.o ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.su ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.d ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.o ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.su ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_f32.d ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_f32.o ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_f32.su ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.d ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.o ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.su ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.d ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.o ./Drivers/CMSIS/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Source-2f-QuaternionMathFunctions

