################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fuentes_2015/common/44blib.c 

ASM_SRCS += \
../fuentes_2015/common/44binit.asm 

OBJS += \
./fuentes_2015/common/44binit.o \
./fuentes_2015/common/44blib.o 

C_DEPS += \
./fuentes_2015/common/44blib.d 

ASM_DEPS += \
./fuentes_2015/common/44binit.d 


# Each subdirectory must supply rules for building sources it contributes
fuentes_2015/common/%.o: ../fuentes_2015/common/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"Z:\PH\PruebaSim\common" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

fuentes_2015/common/%.o: ../fuentes_2015/common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -mapcs-frame -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


