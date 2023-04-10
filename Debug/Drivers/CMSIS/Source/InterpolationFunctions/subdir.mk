################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctions.c \
../Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctionsF16.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f16.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f32.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q15.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q31.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q7.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f16.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f32.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q15.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q31.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q7.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_f32.c \
../Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_init_f32.c 

OBJS += \
./Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctions.o \
./Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctionsF16.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f16.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f32.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q15.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q31.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q7.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f16.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f32.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q15.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q31.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q7.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_f32.o \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_init_f32.o 

C_DEPS += \
./Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctions.d \
./Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctionsF16.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f16.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f32.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q15.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q31.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q7.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f16.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f32.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q15.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q31.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q7.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_f32.d \
./Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_init_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Source/InterpolationFunctions/%.o Drivers/CMSIS/Source/InterpolationFunctions/%.su: ../Drivers/CMSIS/Source/InterpolationFunctions/%.c Drivers/CMSIS/Source/InterpolationFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I"C:/Users/Bobon/Documents/GitHub/newseqen/Drivers/CMSIS/Include/dsp" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Bobon/Documents/GitHub/sound_slave1/extras for cmsis" -I"C:/Users/Bobon/Documents/GitHub/newseqen/samples" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Source-2f-InterpolationFunctions

clean-Drivers-2f-CMSIS-2f-Source-2f-InterpolationFunctions:
	-$(RM) ./Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctions.d ./Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctions.o ./Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctions.su ./Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctionsF16.d ./Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctionsF16.o ./Drivers/CMSIS/Source/InterpolationFunctions/InterpolationFunctionsF16.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f16.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f16.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f16.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f32.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f32.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_f32.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q15.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q15.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q15.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q31.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q31.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q31.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q7.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q7.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_bilinear_interp_q7.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f16.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f16.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f16.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f32.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f32.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_f32.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q15.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q15.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q15.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q31.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q31.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q31.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q7.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q7.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_linear_interp_q7.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_f32.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_f32.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_f32.su ./Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_init_f32.d ./Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_init_f32.o ./Drivers/CMSIS/Source/InterpolationFunctions/arm_spline_interp_init_f32.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Source-2f-InterpolationFunctions

