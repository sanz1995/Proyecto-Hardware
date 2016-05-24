################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fuentes_2015/8led.c \
../fuentes_2015/button.c \
../fuentes_2015/led.c \
../fuentes_2015/main.c \
../fuentes_2015/timer.c 

OBJS += \
./fuentes_2015/8led.o \
./fuentes_2015/button.o \
./fuentes_2015/led.o \
./fuentes_2015/main.o \
./fuentes_2015/timer.o 

C_DEPS += \
./fuentes_2015/8led.d \
./fuentes_2015/button.d \
./fuentes_2015/led.d \
./fuentes_2015/main.d \
./fuentes_2015/timer.d 


# Each subdirectory must supply rules for building sources it contributes
fuentes_2015/%.o: ../fuentes_2015/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -mapcs-frame -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


