################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../8led.c \
../button.c \
../exception.c \
../led.c \
../main.c \
../pila.c \
../rebotes.c \
../sudoku.c \
../timer.c \
../timer3.c 

OBJS += \
./8led.o \
./button.o \
./exception.o \
./led.o \
./main.o \
./pila.o \
./rebotes.o \
./sudoku.o \
./timer.o \
./timer3.o 

C_DEPS += \
./8led.d \
./button.d \
./exception.d \
./led.d \
./main.d \
./pila.d \
./rebotes.d \
./sudoku.d \
./timer.d \
./timer3.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -I"Z:\PH\PruebaSim\common" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -mapcs-frame -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


