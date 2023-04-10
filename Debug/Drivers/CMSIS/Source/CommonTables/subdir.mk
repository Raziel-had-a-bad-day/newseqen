################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Source/CommonTables/CommonTables.c \
../Drivers/CMSIS/Source/CommonTables/CommonTablesF16.c \
../Drivers/CMSIS/Source/CommonTables/arm_common_tables.c \
../Drivers/CMSIS/Source/CommonTables/arm_common_tables_f16.c \
../Drivers/CMSIS/Source/CommonTables/arm_const_structs.c \
../Drivers/CMSIS/Source/CommonTables/arm_const_structs_f16.c \
../Drivers/CMSIS/Source/CommonTables/arm_mve_tables.c \
../Drivers/CMSIS/Source/CommonTables/arm_mve_tables_f16.c 

OBJS += \
./Drivers/CMSIS/Source/CommonTables/CommonTables.o \
./Drivers/CMSIS/Source/CommonTables/CommonTablesF16.o \
./Drivers/CMSIS/Source/CommonTables/arm_common_tables.o \
./Drivers/CMSIS/Source/CommonTables/arm_common_tables_f16.o \
./Drivers/CMSIS/Source/CommonTables/arm_const_structs.o \
./Drivers/CMSIS/Source/CommonTables/arm_const_structs_f16.o \
./Drivers/CMSIS/Source/CommonTables/arm_mve_tables.o \
./Drivers/CMSIS/Source/CommonTables/arm_mve_tables_f16.o 

C_DEPS += \
./Drivers/CMSIS/Source/CommonTables/CommonTables.d \
./Drivers/CMSIS/Source/CommonTables/CommonTablesF16.d \
./Drivers/CMSIS/Source/CommonTables/arm_common_tables.d \
./Drivers/CMSIS/Source/CommonTables/arm_common_tables_f16.d \
./Drivers/CMSIS/Source/CommonTables/arm_const_structs.d \
./Drivers/CMSIS/Source/CommonTables/arm_const_structs_f16.d \
./Drivers/CMSIS/Source/CommonTables/arm_mve_tables.d \
./Drivers/CMSIS/Source/CommonTables/arm_mve_tables_f16.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Source/CommonTables/%.o Drivers/CMSIS/Source/CommonTables/%.su: ../Drivers/CMSIS/Source/CommonTables/%.c Drivers/CMSIS/Source/CommonTables/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I"C:/Users/Bobon/Documents/GitHub/newseqen/Drivers/CMSIS/Include/dsp" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Bobon/Documents/GitHub/sound_slave1/extras for cmsis" -I"C:/Users/Bobon/Documents/GitHub/newseqen/samples" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Source-2f-CommonTables

clean-Drivers-2f-CMSIS-2f-Source-2f-CommonTables:
	-$(RM) ./Drivers/CMSIS/Source/CommonTables/CommonTables.d ./Drivers/CMSIS/Source/CommonTables/CommonTables.o ./Drivers/CMSIS/Source/CommonTables/CommonTables.su ./Drivers/CMSIS/Source/CommonTables/CommonTablesF16.d ./Drivers/CMSIS/Source/CommonTables/CommonTablesF16.o ./Drivers/CMSIS/Source/CommonTables/CommonTablesF16.su ./Drivers/CMSIS/Source/CommonTables/arm_common_tables.d ./Drivers/CMSIS/Source/CommonTables/arm_common_tables.o ./Drivers/CMSIS/Source/CommonTables/arm_common_tables.su ./Drivers/CMSIS/Source/CommonTables/arm_common_tables_f16.d ./Drivers/CMSIS/Source/CommonTables/arm_common_tables_f16.o ./Drivers/CMSIS/Source/CommonTables/arm_common_tables_f16.su ./Drivers/CMSIS/Source/CommonTables/arm_const_structs.d ./Drivers/CMSIS/Source/CommonTables/arm_const_structs.o ./Drivers/CMSIS/Source/CommonTables/arm_const_structs.su ./Drivers/CMSIS/Source/CommonTables/arm_const_structs_f16.d ./Drivers/CMSIS/Source/CommonTables/arm_const_structs_f16.o ./Drivers/CMSIS/Source/CommonTables/arm_const_structs_f16.su ./Drivers/CMSIS/Source/CommonTables/arm_mve_tables.d ./Drivers/CMSIS/Source/CommonTables/arm_mve_tables.o ./Drivers/CMSIS/Source/CommonTables/arm_mve_tables.su ./Drivers/CMSIS/Source/CommonTables/arm_mve_tables_f16.d ./Drivers/CMSIS/Source/CommonTables/arm_mve_tables_f16.o ./Drivers/CMSIS/Source/CommonTables/arm_mve_tables_f16.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Source-2f-CommonTables

