################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ImageComputer.cpp \
../src/PerfInfo.cpp \
../src/RegExp.cpp \
../src/SemRepair.cpp \
../src/SemRepairDebugger.cpp \
../src/StrangerAutomaton.cpp \
../src/main.cpp 

OBJS += \
./src/ImageComputer.o \
./src/PerfInfo.o \
./src/RegExp.o \
./src/SemRepair.o \
./src/SemRepairDebugger.o \
./src/StrangerAutomaton.o \
./src/main.o 

CPP_DEPS += \
./src/ImageComputer.d \
./src/PerfInfo.d \
./src/RegExp.d \
./src/SemRepair.d \
./src/SemRepairDebugger.d \
./src/StrangerAutomaton.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/mona -I/usr/local/include/stranger -O0 -g3 -gdwarf-2 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


