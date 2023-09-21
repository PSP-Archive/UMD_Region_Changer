TARGET = UMD_Region_Changer
CFLAGS = -Wall -O2 -G0
CXXFLAGS = $(CFLAGS) -fno-execeptions -fno-rtti
ASFLAGS = $(CFLAGS)

INCDIR = $(UMDREGIONROOT)/common/include ./include

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = UMD Region Changer
PSP_EBOOT_ICON = $(UMDREGIONROOT)/res/ICON0.PNG
PSP_EBOOT_PIC1 = $(UMDREGIONROOT)/res/PIC1.PNG
PSP_FW_VERSION = 660
BUILD_PRX = 1



OBJS = main.o graphics.o

LIBS = -lpng -lz -lc -lm -lpspgu -lpspgum -lpspdebug -lpspsystemctrl_user

LIBDIR=$(UMDREGIONROOT)/libs

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
