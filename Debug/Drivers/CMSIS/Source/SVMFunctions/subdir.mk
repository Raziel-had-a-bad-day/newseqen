################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Source/SVMFunctions/SVMFunctions.c \
../Drivers/CMSIS/Source/SVMFunctions/SVMFunctionsF16.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f16.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f32.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f16.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f32.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f16.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f32.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f16.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f32.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f16.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f32.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f16.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f32.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f16.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f32.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f16.c \
../Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f32.c 

OBJS += \
./Drivers/CMSIS/Source/SVMFunctions/SVMFunctions.o \
./Drivers/CMSIS/Source/SVMFunctions/SVMFunctionsF16.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f16.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f32.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f16.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f32.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f16.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f32.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f16.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f32.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f16.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f32.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f16.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f32.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f16.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f32.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f16.o \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f32.o 

C_DEPS += \
./Drivers/CMSIS/Source/SVMFunctions/SVMFunctions.d \
./Drivers/CMSIS/Source/SVMFunctions/SVMFunctionsF16.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f16.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f32.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f16.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f32.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f16.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f32.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f16.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f32.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f16.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f32.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f16.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f32.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f16.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f32.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f16.d \
./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Source/SVMFunctions/%.o Drivers/CMSIS/Source/SVMFunctions/%.su: ../Drivers/CMSIS/Source/SVMFunctions/%.c Drivers/CMSIS/Source/SVMFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I"C:/Users/Bobon/Documents/GitHub/newseqen/Drivers/CMSIS/Include/dsp" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Bobon/Documents/GitHub/sound_slave1/extras for cmsis" -I"C:/Users/Bobon/Documents/GitHub/newseqen/samples" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Source-2f-SVMFunctions

clean-Drivers-2f-CMSIS-2f-Source-2f-SVMFunctions:
	-$(RM) ./Drivers/CMSIS/Source/SVMFunctions/SVMFunctions.d ./Drivers/CMSIS/Source/SVMFunctions/SVMFunctions.o ./Drivers/CMSIS/Source/SVMFunctions/SVMFunctions.su ./Drivers/CMSIS/Source/SVMFunctions/SVMFunctionsF16.d ./Drivers/CMSIS/Source/SVMFunctions/SVMFunctionsF16.o ./Drivers/CMSIS/Source/SVMFunctions/SVMFunctionsF16.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f16.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f16.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f16.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f32.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f32.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_init_f32.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f16.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f16.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f16.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f32.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f32.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_linear_predict_f32.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f16.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f16.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f16.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f32.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f32.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_init_f32.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f16.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f16.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f16.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f32.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f32.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_polynomial_predict_f32.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f16.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f16.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f16.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f32.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f32.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_init_f32.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f16.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f16.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f16.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f32.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f32.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_rbf_predict_f32.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f16.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f16.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f16.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f32.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f32.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_init_f32.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f16.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f16.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f16.su ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f32.d ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f32.o ./Drivers/CMSIS/Source/SVMFunctions/arm_svm_sigmoid_predict_f32.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Source-2f-SVMFunctions

