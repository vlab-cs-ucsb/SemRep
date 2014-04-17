################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/depgraph/DepGraph.cpp \
../src/depgraph/DepGraphNode.cpp \
../src/depgraph/DepGraphNormalNode.cpp \
../src/depgraph/DepGraphOpNode.cpp \
../src/depgraph/DepGraphSccNode.cpp \
../src/depgraph/DepGraphUninitNode.cpp 

OBJS += \
./src/depgraph/DepGraph.o \
./src/depgraph/DepGraphNode.o \
./src/depgraph/DepGraphNormalNode.o \
./src/depgraph/DepGraphOpNode.o \
./src/depgraph/DepGraphSccNode.o \
./src/depgraph/DepGraphUninitNode.o 

CPP_DEPS += \
./src/depgraph/DepGraph.d \
./src/depgraph/DepGraphNode.d \
./src/depgraph/DepGraphNormalNode.d \
./src/depgraph/DepGraphOpNode.d \
./src/depgraph/DepGraphSccNode.d \
./src/depgraph/DepGraphUninitNode.d 


# Each subdirectory must supply rules for building sources it contributes
src/depgraph/%.o: ../src/depgraph/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/mona -I/usr/local/include/stranger -O0 -g3 -gdwarf-2 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


