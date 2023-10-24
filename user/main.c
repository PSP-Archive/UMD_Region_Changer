#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspiofilemgr.h>
#include <psptypes.h> 
#include <pspgu.h> 
#include <pspaudiocodec.h> 
#include <pspaudiolib.h>
#include <pspaudio.h> 
#include <png.h>
#include <systemctrl.h>
#include <globals.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <graphics.h>
#include <common.h>
#include <mp3player.h>

PSP_MODULE_INFO("UMD_REGION_CHANGER", 0, 1, 5);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

#define printf pspDebugScreenPrintf
#define BUF_WIDTH  512
#define SCR_WIDTH  480
#define SCR_HEIGHT 272


int main(int argc, char *args[]) {
	Image* background;
	pspDebugScreenInit();
	initGraphics();
	/*FILE* fp = fopen(args[0], "rb");
	PBPHeader header;
	fread(&header, 1, sizeof(PBPHeader), fp);
	fclose(fp);
	*/


	background = loadImage("BG.PNG", 0);



	sceDisplayWaitVblankStart();
	pspDebugScreenSetXY(17, 5);

	pspDebugScreenSetTextColor(0xFF0000FF);
	printf("Presented by ARK-4 Team (c)2023");
	pspDebugScreenSetXY(26, 10);
	pspDebugScreenSetTextColor(0xFF00FF00);
	printf("Version 1.5");

	sceKernelDelayThread(3000000);


	if(kuKernelGetModel() == 4) {
		pspDebugScreenClear();
		sceDisplayWaitVblank();
		pspDebugScreenSetTextColor(0xFF0000FF);
		pspDebugScreenSetXY(25, 10);
		printf("GO home your drunk.");
		pspDebugScreenSetXY(10, 20);
		printf("This is for UMD Movies... UMD does your GO/Vita have a UMD Drive?");
		sceDisplayWaitVblank();
	
		sceKernelDelayThread(5000000);
		sceKernelExitGame();
	}


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

#define nums (sizeof(regions)/sizeof(regions[0]))-1

	int offset = 50;

	int i = 0;
	int region;

	char selected[64];

	while(1){
		blitImageToScreen(0, 0, 480, 272, background, 0, 0);	
		sceDisplayWaitVblankStart();
		//pspDebugScreenSetTextColor(0xFF0000);
		//pspDebugScreenSetXY(0, 0);
		sceCtrlReadBufferPositive(&pad, 1);	
		//printf("##########################");
		printTextScreen(0, 5, "##########################", 0xFFFF0000);
		//pspDebugScreenSetXY(0, 1);
		//printf("#                        #");
		printTextScreen(0, 10, "#                        #", 0xFFFF0000);
		//pspDebugScreenSetXY(0, 2);
		//printf("# Please choose a Region #");
		printTextScreen(0, 20, "# Please choose a Region #", 0xFFFF0000);
		//pspDebugScreenSetXY(0, 3);
		//printf("#                        #");
		printTextScreen(0, 30, "#                        #", 0xFFFF0000);
		//pspDebugScreenSetXY(0, 4);
		//printf("##########################");
		//pspDebugScreenSetXY(0, 6);
		printTextScreen(0, 40, "##########################", 0xFFFF0000);
	
	// BGR
	//	pspDebugScreenSetTextColor(0xFFFFFF);


		
		// DEFAULT
		if(1) {
			for(i = 0; i<nums+1;i++) {
				if(i==dir) {
					//pspDebugScreenSetTextColor(0x00FF00);
					sprintf(selected, "%s%s", arrow, regions[i]);
					printTextScreen(0, offset+=16, selected, 0xFF00FF00);
					//printf("%s%s", arrow, regions[i]);
					//pspDebugScreenSetTextColor(0xFFFFFF);
				}
				else{
					//pspDebugScreenSetXY(0, offset+=2);
					printTextScreen(0, offset+=16, regions[i], 0x00FFFFFF);
				}
			}
			region = dir;
			//pspDebugScreenSetXY(15, 32);

			//pspDebugScreenSetTextColor(0x0000FF);
			//printf("Press (X) to accept and return back to XMB");
			//pspDebugScreenSetTextColor(0xFF0000);
			offset=50;
			//blitImageToScreen(0, 0, 480, 272, background, 0, 0);	
		}
		
		// BUTTON DOWN
		if(pad.Buttons & PSP_CTRL_DOWN) {
			dir++;
			if(dir>nums) dir = 0;
			sceKernelDelayThread(100000);
			region = dir;
		}

		// BUTTON UP
		if(pad.Buttons & PSP_CTRL_UP) {
			dir--;
			if(dir<0) dir = nums;
			sceKernelDelayThread(100000);
			region = dir;
		}

		// ACCEPT BUTTON
		else if(pad.Buttons & PSP_CTRL_CROSS) {
			sceDisplayWaitVblankStart();

			pspAudioInit();
			MP3_Init(1);
			MP3_Load("EXIT.MP3");
			MP3_Play();

			while(!MP3_EndOfStream()) {};
			MP3_Stop();

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

			if(module < 0 || size < 0) {
				pspDebugScreenSetXY(5, 25);
				printf("Nah your module size %d is messed up or your byteRead is %d", module, byteRead);
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
	
		offset=50;

		printTextScreen(60, 250, "Press (X) to accept and return back to XMB", 0xFF0000FF);

		flipScreen();
	

	}

	return 0;
}

