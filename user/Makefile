TARGET = UMD_Region_Changer
CFLAGS = -Wall -O2 -G0
CXXFLAGS = $(CFLAGS) -fno-execeptions -fno-rtti
ASFLAGS = $(CFLAGS)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = UMD Region Changer
PSP_EBOOT_ICON = $(UMDREGIONROOT)/res/ICON0.PNG
#PSP_EBOOT_UNKPNG = $(UMDREGIONROOT)/res/matrix_new.png
PSP_EBOOT_PIC1 = $(UMDREGIONROOT)/res/PIC1_BLUR.PNG
PSP_FW_VERSION = 660
BUILD_PRX = 1

OBJS = main.o \
	   mp3player.o \
	   font.o \
	   graphics.o \
	   framebuffer.o 

INCDIR = $(UMDREGIONROOT)/libs/libpng $(UMDREGIONROOT)/libs/libmad/include $(UMDREGIONROOT)/common/include include
LIBDIR= $(UMDREGIONROOT)/libs

LIBS = -lpng -lz -lc -lm -lmad -lpspgu -lpspgum -lpspdebug -lpspdisplay -lpspsystemctrl_user -lpspkubridge -lpspaudiolib -lpspaudio -lpsputility -lpspvfpu
LDFLAGS = -L libs


include $(UMDREGIONROOT)/common/make/global.mak
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
