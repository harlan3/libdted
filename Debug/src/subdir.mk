################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Dted_Acc.cpp \
../src/Dted_Cell.cpp \
../src/Dted_Cell_Path_Entry.cpp \
../src/Dted_Database.cpp \
../src/Dted_Directory.cpp \
../src/Dted_Dsi.cpp \
../src/Dted_Hdr.cpp \
../src/Dted_Record.cpp \
../src/Dted_Uhl.cpp \
../src/Dted_Vol.cpp \
../src/Endian.cpp 

OBJS += \
./src/Dted_Acc.o \
./src/Dted_Cell.o \
./src/Dted_Cell_Path_Entry.o \
./src/Dted_Database.o \
./src/Dted_Directory.o \
./src/Dted_Dsi.o \
./src/Dted_Hdr.o \
./src/Dted_Record.o \
./src/Dted_Uhl.o \
./src/Dted_Vol.o \
./src/Endian.o 

CPP_DEPS += \
./src/Dted_Acc.d \
./src/Dted_Cell.d \
./src/Dted_Cell_Path_Entry.d \
./src/Dted_Database.d \
./src/Dted_Directory.d \
./src/Dted_Dsi.d \
./src/Dted_Hdr.d \
./src/Dted_Record.d \
./src/Dted_Uhl.d \
./src/Dted_Vol.d \
./src/Endian.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -O0 -g3 -Wall -fPIC -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


