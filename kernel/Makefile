TARGET = UMD_Region_Changer
CFLAGS = -Wall -O2 -G0
ifdef DEBUG
	CFLAGS += -DDEBUG=$(DEBUG)
endif

CXXFLAGS = $(CFLAGS) -fno-execeptions -fno-rtti
ASFLAGS = $(CFLAGS)

INCDIR = ./include

PSP_FW_VERSION = 660
BUILD_PRX = 1

PRX_EXPORTS = exports.exp

OBJS = main.o

USE_KERNEL_LIBC=1
USE_KERNEL_LIBS=1

INCDIR = $(UMDREGIONROOT)/common/include ./include

LIBS = -lc -lpspsystemctrl_kernel

LIBDIR=$(UMDREGIONROOT)/libs

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
