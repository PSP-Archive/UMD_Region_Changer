TARGET = UMD_Region_Changer_ME_fix
CFLAGS = -Wall -O2 -G0
ifdef DEBUG
	CFLAGS += -DDEBUG=$(DEBUG)
endif

LDFLAGS += -mno-crt0 -nostartfiles
CXXFLAGS = $(CFLAGS) -fno-execeptions -fno-rtti
ASFLAGS = $(CFLAGS)

PSP_FW_VERSION = 660
BUILD_PRX = 1

PRX_EXPORTS = exports.exp

OBJS = main.o

USE_KERNEL_LIBC=1
USE_KERNEL_LIBS=1


INCDIR = $(UMDREGIONROOT)/common/include ./include
LIBDIR=$(UMDREGIONROOT)/libs

LIBS = -lc -lcolordebugger -lpspsystemctrl_kernel -lpspkubridge


PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
