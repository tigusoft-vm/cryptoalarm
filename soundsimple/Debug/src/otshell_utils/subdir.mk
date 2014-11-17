################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/otshell_utils/ccolor.cpp \
../src/otshell_utils/runoptions.cpp \
../src/otshell_utils/utils.cpp 

OBJS += \
./src/otshell_utils/ccolor.o \
./src/otshell_utils/runoptions.o \
./src/otshell_utils/utils.o 

CPP_DEPS += \
./src/otshell_utils/ccolor.d \
./src/otshell_utils/runoptions.d \
./src/otshell_utils/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/otshell_utils/%.o: ../src/otshell_utils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


