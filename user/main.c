#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspiofilemgr.h>
#include <psptypes.h> 
#include <pspgu.h> 
#include <png.h>
#include "graphics.h"
#include <systemctrl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "matrix.h"
#include "graphics.h"


PSP_MODULE_INFO("UMD_REGION_CHANGER", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(4096);

#define printf pspDebugScreenPrintf
#define BUF_WIDTH  512
#define SCR_WIDTH  480
#define SCR_HEIGHT 272

int main(int argc, char *args[]) {

	pspDebugScreenInit();

	sceDisplayWaitVblankStart();

	pspDebugScreenSetXY(17, 17);

	pspDebugScreenSetTextColor(0xFF0000FF);
	printf("Presented by ARK-4 Team (c)2023");

	sceDisplayWaitVblank();
	
	sceKernelDelayThread(3000000);


	pspDebugScreenClear();
	SceCtrlData pad;

	char arrow[] = "-> ";

	int dir = 0;

	char *regions[4] = {
		"Default",
		"Japan",
		"America",
		"Europe",
	};

	int nums = 4;

	int offset = 6;

	int i = 0;
	int region;

	while(1){
		sceDisplayWaitVblankStart();
		pspDebugScreenSetTextColor(0xFF0000);
		pspDebugScreenSetXY(0, 0);
		sceCtrlReadBufferPositive(&pad, 1);	
		printf("##########################");
		pspDebugScreenSetXY(0, 1);
		printf("#                        #");
		pspDebugScreenSetXY(0, 2);
		printf("# Please choose a Region #");
		pspDebugScreenSetXY(0, 3);
		printf("#                        #");
		pspDebugScreenSetXY(0, 4);
		printf("##########################");
		pspDebugScreenSetXY(0, 6);
	
	// BGR
		pspDebugScreenSetTextColor(0xFFFFFF);


		
		// DEFAULT
		if(dir==0) {
			for(i = 0; i<nums;i++) {
				if(i==dir) {
					pspDebugScreenSetTextColor(0x00FF00);
					printf("%s%s", arrow, regions[i]);
					pspDebugScreenSetTextColor(0xFFFFFF);
				}
				else{
					pspDebugScreenSetXY(0, offset+=2);
					printf("%s", regions[i]);
				}
			}
			region = dir;
			pspDebugScreenSetXY(15, 32);

			pspDebugScreenSetTextColor(0x0000FF);
			printf("Press (X) to accept and return back to XMB");
			pspDebugScreenSetTextColor(0xFF0000);
			offset=6;
		}
		
		// BUTTON DOWN
		if(pad.Buttons & PSP_CTRL_DOWN) {
			pspDebugScreenClear();
			dir++;
			if(dir>3) {
				dir=0;
				pspDebugScreenSetTextColor(0xFFFFFF);
			}
			else {
				for(i = 0; i<nums;i++) {
					pspDebugScreenSetTextColor(0xFFFFFF);
					if(i==dir) {
						pspDebugScreenSetTextColor(0x00FF00);
						pspDebugScreenSetXY(0, offset+=2);
						printf("%s%s", arrow, regions[i]);
						pspDebugScreenSetTextColor(0xFFFFFF);
					}
					else if(i==0) {
						pspDebugScreenSetXY(0, 6);
						printf("%s", regions[i]);
					}
					else{
						pspDebugScreenSetXY(0, offset+=2);
						printf("%s", regions[i]);
					}
				}
				region = dir;
				pspDebugScreenSetXY(15, 32);
				pspDebugScreenSetTextColor(0x0000FF);
				printf("Press (X) to accept and return back to XMB");
			}
			sceKernelDelayThread(100000);
			offset=6;
		}
		// BUTTON UP
		else if(pad.Buttons & PSP_CTRL_UP) {
			pspDebugScreenClear();
			dir--;
			if(dir<0) {
				dir=3;
				pspDebugScreenSetTextColor(0xFFFFFF);
			}
			
			if(dir!=0 || dir==3){
				for(i = 0; i<nums;i++) {
					if(i==dir){
						pspDebugScreenSetTextColor(0x00FF00);
						pspDebugScreenSetXY(0, offset+=2);
						printf("%s%s", arrow, regions[i]);
						pspDebugScreenSetTextColor(0xFFFFFF);
					}
					else if(i==0) {
						pspDebugScreenSetXY(0, 6);
						printf("%s", regions[i]);
					}
					else{
						pspDebugScreenSetXY(0, offset+=2);
						printf("%s", regions[i]);
					}
				}
				region = dir;
				pspDebugScreenSetXY(15, 32);
				pspDebugScreenSetTextColor(0x0000FF);
				printf("Press (X) to accept and return back to XMB");
			}
			sceKernelDelayThread(80000);
			offset=6;
		}
		// ACCEPT BUTTON
		else if(pad.Buttons & PSP_CTRL_CROSS) {
			// Need to not hardcode this for category lite reasons. 
			char mod[] = "ms0:/PSP/GAME/UMD_Region_Changer/UMD_Region_Changer.prx";
			char region_path[] = "ms0:/PSP/GAME/UMD_Region_Changer/region.txt";
			const int size = 100*1024;
			static u8 buf[100*1024];
			SceUID byteRead;
			SceUID module;

			pspDebugScreenSetXY(5, 25);

			//0 // default
			//3 // Japan
			//4 // America
			//5 // Europe

			if(region==1)
				region = 3;
			else if(region==2)
				region = 4;
			else if(region==3)
				region = 5;
			else
				region = 0;

			SceUID save_region = sceIoOpen(region_path, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);

			if(save_region < 0) {
				pspDebugScreenSetXY(5, 25);
				printf("Could not open : region.txt");
				while(1) {};
			}



			void *region_str = (char*)malloc(sizeof(int));;
			sprintf(region_str, "%d", region);
			sceIoWrite(save_region, region_str, 1);
			free(region_str);


			sceIoClose(save_region);

			sceKernelDelayThread(1000000);



			module = sceIoOpen(mod, PSP_O_RDONLY, 0777);

			byteRead = sceIoRead(module, buf, size);
			sceIoClose(module);

			if(module < 0 || byteRead < 0) {
				pspDebugScreenSetXY(5, 25);
				printf("Nah your module size %d is fucked or your byteRead is %d", module, byteRead);
				while(1){};
			}
			else {
				pspDebugScreenSetXY(0, 30);
				pspDebugScreenSetTextColor(0xFFFFFF);
				printf("Enjoy your freedom ;-)");
				sceKernelDelayThread(500000);
			}
				

			// Start module before umdman starts
			sctrlHENLoadModuleOnReboot("/kd/umdman.prx", buf, byteRead, BOOTLOAD_VSH);
			// Go back to XMB and enjoy region unlocked goodness.
			sctrlKernelExitVSH(NULL);
		}
	
		offset=6;
	

	}

	return 0;
}

