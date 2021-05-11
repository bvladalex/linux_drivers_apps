################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fonts.c \
../oled96.c \
../oled_display_temp.c 

OBJS += \
./fonts.o \
./oled96.o \
./oled_display_temp.o 

C_DEPS += \
./fonts.d \
./oled96.d \
./oled_display_temp.d 


# Each subdirectory must supply rules for building sources it contributes
fonts.o: D:/Electro\ stuff/bbb_course/eclipse_projects/oled_display_temp/fonts.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"fonts.d" -MT"fonts.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

oled96.o: D:/Electro\ stuff/bbb_course/eclipse_projects/oled_display_temp/oled96.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"oled96.d" -MT"oled96.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


