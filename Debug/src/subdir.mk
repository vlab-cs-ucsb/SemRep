################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DepGraph.cpp \
../src/DepGraphNode.cpp \
../src/DepGraphNormalNode.cpp \
../src/DepGraphOpNode.cpp \
../src/DepGraphSccNode.cpp \
../src/DepGraphUninitNode.cpp \
../src/ImageComputer.cpp \
../src/PerfInfo.cpp \
../src/RegExp.cpp \
../src/SemRepair.cpp \
../src/SemRepairDebugger.cpp \
../src/StrangerAutomaton.cpp \
../src/StrangerAutomatonException.cpp \
../src/StringAnalyzer.cpp \
../src/UnsupportedRegexException.cpp \
../src/main.cpp 

OBJS += \
./src/DepGraph.o \
./src/DepGraphNode.o \
./src/DepGraphNormalNode.o \
./src/DepGraphOpNode.o \
./src/DepGraphSccNode.o \
./src/DepGraphUninitNode.o \
./src/ImageComputer.o \
./src/PerfInfo.o \
./src/RegExp.o \
./src/SemRepair.o \
./src/SemRepairDebugger.o \
./src/StrangerAutomaton.o \
./src/StrangerAutomatonException.o \
./src/StringAnalyzer.o \
./src/UnsupportedRegexException.o \
./src/main.o 

CPP_DEPS += \
./src/DepGraph.d \
./src/DepGraphNode.d \
./src/DepGraphNormalNode.d \
./src/DepGraphOpNode.d \
./src/DepGraphSccNode.d \
./src/DepGraphUninitNode.d \
./src/ImageComputer.d \
./src/PerfInfo.d \
./src/RegExp.d \
./src/SemRepair.d \
./src/SemRepairDebugger.d \
./src/StrangerAutomaton.d \
./src/StrangerAutomatonException.d \
./src/StringAnalyzer.d \
./src/UnsupportedRegexException.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/abaki/workspace/strangerlib" -I"/home/abaki/workspace/MONA/BDD" -I"/home/abaki/workspace/MONA/DFA" -I"/home/abaki/workspace/MONA/Mem" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


