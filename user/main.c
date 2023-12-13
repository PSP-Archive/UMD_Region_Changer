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
#include <psputils.h> 
#include <pspthreadman.h> 
#include <psputility.h> 
#include <psputility_osk.h> 
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

PSP_MODULE_INFO("UMD_REGION_CHANGER", 0, 1, 8);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VSH|PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(17*1024);

#define printf pspDebugScreenPrintf
#define BUF_WIDTH  512
#define SCR_WIDTH  480
#define SCR_HEIGHT 272
#define NUM_INPUT_FIELDS    (3)
#define TEXT_LENGTH         (128)


static char arrow[] = "-> ";
static int dir = 0;
static int trueType = -1;
static int offset = 50;
static int region;
static int i = -1;
static char selected[64];
static char check_path[64];
static unsigned int __attribute__((aligned(16))) list[262144];
unsigned short custom[4] = {0};
unsigned short custom_ret[4] = {0};
volatile bool debug = false;
volatile bool init = true;



static char *regions[5] = {
		"Default",
		"Japan",
		"America",
		"Europe",
		"Custom",
};


#define nums (sizeof(regions)/sizeof(regions[0]))-1

static void playSound(bool type) {
	MP3_Init(1);

	if(type)
		MP3_Load("ACCEPT.MP3");
	else
		MP3_Load("SELECTION.MP3");
			
	MP3_Play();
	while(!MP3_EndOfStream()) {}
	MP3_Stop();
}


static void updateScreen(Image* bg, int dir, int type) {
		blitImageToScreen(0, 0, 480, 272, bg, 0, 0);	
		for(i = 0; i<nums+1;i++) {
			if(!debug && i == 4) continue;
			else if(i==dir) {
				sprintf(selected, "%s%s", arrow, regions[i]);
				printTextScreen(0, offset+=16, selected, 0xFF00FF00);
			}
			else{
				printTextScreen(0, offset+=16, regions[i], 0x00FFFFFF);
			}
		}
		region = dir;
		offset=50;
		if(type >= 0) {
			playSound(type);
			trueType = -1;
		}
}


int main(int argc, char *args[]) {
	Image* background;
	pspDebugScreenInit();
	initGraphics();
	pspAudioInit();
	background = loadImage("BG.PNG", 0);



	sceDisplayWaitVblankStart();


	strcpy(check_path, UMD_REGION_PATH);
	strcat(check_path, "EBOOT.PBP");

	if(strcmp(args[0], check_path) != 0) {
		pspDebugScreenSetXY(5, 5);
		pspDebugScreenSetTextColor(0x00FFFFFF);
		printf("Sorry for right now Categories Lite is not supported...");
		pspDebugScreenSetXY(5, 8);
		printf("Exiting...");
		sceKernelDelayThread(5000000);
		sctrlKernelExitVSH(NULL);
	}


	pspDebugScreenSetXY(17, 5);

	pspDebugScreenSetTextColor(0xFF0000FF);
	printf("Presented by ARK-4 Team (c)2023");
	pspDebugScreenSetXY(26, 10);
	pspDebugScreenSetTextColor(0xFF00FF00);
	printf("Version 1.8");
	pspDebugScreenSetXY(48, 10);
	pspDebugScreenSetTextColor(0xFF00FF00);

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
	u32 currentTime;
	u32 lastClick = 0;

	while(1){
		sceKernelGetSystemTime(&currentTime);
		sceDisplayWaitVblankStart();
		sceCtrlReadBufferPositive(&pad, 1);	
		u32 elapsedTime = currentTime - lastClick;	
		if(pad.Buttons & PSP_CTRL_LTRIGGER) {
			if(elapsedTime >= 80000) {
				debug = !debug;
				lastClick = currentTime;
			}
		}
		updateScreen(background, dir, trueType);	
		printTextScreen(0, 5,  "##########################", 0xFFFF0000);
		printTextScreen(0, 10, "#                        #", 0xFFFF0000);
		printTextScreen(0, 20, "# Please choose a Region #", 0xFFFF0000);
		printTextScreen(0, 30, "#                        #", 0xFFFF0000);
		printTextScreen(0, 40, "##########################", 0xFFFF0000);


		// BUTTON DOWN
		if(pad.Buttons & PSP_CTRL_DOWN) {
			if(elapsedTime >= 80000) {
				dir++;
				if(!debug && dir>nums-1) dir = 0;
				else if(dir>nums) dir = 0;
				region = dir;
				trueType = 0;
				lastClick = currentTime;
			}
		}

		// BUTTON UP
		if(pad.Buttons & PSP_CTRL_UP) {
			if(elapsedTime >= 80000) {
				dir--;
				if(!debug && dir<0) dir = nums-1;
				else if(dir<0) dir = nums;
				region = dir;
				trueType = 0;
				lastClick = currentTime;
			}
		}

		// ACCEPT BUTTON
		if(pad.Buttons & PSP_CTRL_CROSS) {
			if(dir == 4) {
				unsigned short desc[] = {'C', 'u', 's', 't', 'o', 'm', ' ', 'R', 'e', 'g', 'i', 'o', 'n', '\0'};
				unsigned short intext[] = {'0', '\0'};

				SceUtilityOskData oskData;
				memset(&oskData, 0, sizeof(SceUtilityOskData));

				oskData.language = PSP_UTILITY_OSK_LANGUAGE_ENGLISH;
				oskData.lines = 1;
				oskData.unk_24 = 1;
				oskData.inputtype = PSP_UTILITY_OSK_INPUTTYPE_LATIN_DIGIT;
				oskData.desc = desc;
				oskData.intext = intext;
				oskData.outtextlength = 2;
				oskData.outtextlimit = 2;
				oskData.outtext = custom;


				SceUtilityOskParams oskParams;
				memset(&oskParams, 0, sizeof(SceUtilityOskParams));

    			oskParams.base.size = sizeof(SceUtilityOskParams);
    			oskParams.base.language = PSP_UTILITY_OSK_LANGUAGE_ENGLISH;
				oskParams.base.buttonSwap = PSP_UTILITY_ACCEPT_CROSS;
    			oskParams.base.graphicsThread = 17;
				oskParams.base.accessThread = 19;
    			oskParams.base.fontThread = 18;
    			oskParams.base.soundThread = 16;
    			oskParams.datacount = 1;
				oskParams.data = &oskData;
				int done = 0;
				

				sceUtilityOskInitStart(&oskParams);
				while(!done) {
					sceGuStart(GU_DIRECT,list);
					sceGuClear(GU_COLOR_BUFFER_BIT);

					sceGuFinish();
					sceGuSync(0,0);

					sceGuClear(GU_COLOR_BUFFER_BIT);

					switch(sceUtilityOskGetStatus())
					{
						case PSP_UTILITY_DIALOG_INIT:
							sceKernelDelayThread(100000);
							break;
						case PSP_UTILITY_DIALOG_VISIBLE:
							sceUtilityOskUpdate(1);
							break;
						case PSP_UTILITY_DIALOG_QUIT:
							sceUtilityOskShutdownStart();
							break;
						case PSP_UTILITY_DIALOG_FINISHED:
							break;
						case PSP_UTILITY_DIALOG_NONE:
							done = 1;
						default :
							break;
					}

					sceDisplayWaitVblankStart();
					sceGuSwapBuffers();
				}

				custom_ret[0] = oskParams.data[0].outtext[0];
								
			}
			sceUtilityOskShutdownStart();
			sceDisplayWaitVblankStart();
			updateScreen(background, dir, 1);
			// Need to not hardcode this for category lite reasons. 
			//char mod[] = "ms0:/PSP/GAME/UMD_Region_Changer/UMD_Region_Changer.prx";
			//char mod[] = "ms0:/SEPLUGINS/UMD_Region_Changer_ME_fix.prx";
			char mod[] = "UMD_Region_Changer.prx";
			//char region_path[] = "ms0:/PSP/GAME/UMD_Region_Changer/region.txt";
			char region_path[] = "region.txt";
			const int size = 100*1024;
			static u8 buf[100*1024];
			SceUID byteRead;
			SceUID module;

			pspDebugScreenSetXY(5, 25);

			//0 // default
			//3 // Japan
			//4 // America
			//5 // Europe (Australia)
			if(custom_ret[0] != NULL) {
				int custom_convert = custom_ret[0] - '0';
				region = custom_convert;
			}
			else {
				if(region==1)
					region = 3;
				else if(region==2)
					region = 4;
				else if(region==3)
					region = 5;
				else
					region = 0;
			}

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
				printf("Nah your module size %d is messed up or your byteRead is %d", module, byteRead);
				while(1){};
			}
			else {
				clearScreen(0x00000000);
				printTextScreen(130, 135, "Enjoy your freedom ;-)", 0x00FFFFFF);
				flipScreen();
				sceKernelDelayThread(500000);
			}
				
			int status;

			// Start module before umdman starts
			sctrlHENLoadModuleOnReboot("/kd/umdman.prx", buf, byteRead, (BOOTLOAD_VSH | BOOTLOAD_GAME | BOOTLOAD_POPS | BOOTLOAD_UPDATER | BOOTLOAD_UMDEMU | BOOTLOAD_APP | BOOTLOAD_MLNAPP));
			//sctrlHENLoadModuleOnReboot("/kd/systemctrl.prx", buf, byteRead, BOOTLOAD_VSH);
			// Go back to XMB and enjoy region unlocked goodness.
			sctrlKernelExitVSH(NULL);
		}
	
		offset=50;

		printTextScreen(60, 250, "Press (X) to accept and return back to XMB", 0xFF0000FF);

		flipScreen();
	

	}

	return 0;
}

