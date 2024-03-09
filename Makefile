# Project Name
TARGET = spectral

# Sources
CPP_SOURCES = spectral.cpp

# Library Locations
DAISYSP_DIR ?= ./DaisySP
LIBDAISY_DIR ?= ./libDaisy
USE_FATFS = 1

CPP_STANDARD ?= -std=gnu++17
OPT ?= -O3

C_SOURCES = \
$(LIBDAISY_DIR)/Drivers/CMSIS-DSP/Source/TransformFunctions/arm_rfft_fast_f32.c \
$(LIBDAISY_DIR)/Drivers/CMSIS-DSP/Source/TransformFunctions/arm_rfft_fast_init_f32.c \
$(LIBDAISY_DIR)/Drivers/CMSIS-DSP/Source/CommonTables/arm_common_tables.c \
$(LIBDAISY_DIR)/Drivers/CMSIS-DSP/Source/CommonTables/arm_const_structs.c \
$(LIBDAISY_DIR)/Drivers/CMSIS-DSP/Source/TransformFunctions/arm_cfft_init_f32.c \
$(LIBDAISY_DIR)/Drivers/CMSIS-DSP/Source/TransformFunctions/arm_cfft_f32.c \
$(LIBDAISY_DIR)/Drivers/CMSIS-DSP/Source/TransformFunctions/arm_bitreversal2.c \
$(LIBDAISY_DIR)/Drivers/CMSIS-DSP/Source/TransformFunctions/arm_bitreversal.c \
$(LIBDAISY_DIR)/Drivers/CMSIS-DSP/Source/TransformFunctions/arm_cfft_radix8_f32.c


C_INCLUDES += -I$(LIBDAISY_DIR)/Drivers/CMSIS-DSP/Include -I$(LIBDAISY_DIR)/Drivers/CMSIS-DSP/PrivateInclude 

C_DEFS += -DUSE_ARM_FFT 

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile