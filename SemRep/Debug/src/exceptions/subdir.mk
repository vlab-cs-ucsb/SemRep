################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/exceptions/StrangerAutomatonException.cpp \
../src/exceptions/UnsupportedRegexException.cpp 

OBJS += \
./src/exceptions/StrangerAutomatonException.o \
./src/exceptions/UnsupportedRegexException.o 

CPP_DEPS += \
./src/exceptions/StrangerAutomatonException.d \
./src/exceptions/UnsupportedRegexException.d 


# Each subdirectory must supply rules for building sources it contributes
src/exceptions/%.o: ../src/exceptions/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/mona -I/usr/local/include/stranger -O0 -g3 -gdwarf-2 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


