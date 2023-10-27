#include <pspkernel.h>
#include <pspsdk.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspinit.h>

#include <systemctrl.h>
#include <globals.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


PSP_MODULE_INFO("UMDREGION_Module_ME", 0x1000, 2, 0);

STMOD_HANDLER previous = NULL;

int module_start(SceSize args, void *argp) {
	//_sw(0x44000000, 0xBC800100);
	//colorDebug(0xFF);
	char mod[] = "ms0:/PSP/GAME/UMD_Region_Changer/UMD_Region_Changer.prx";
	static u8 buf[100*1024];
	const int size = 100*1024;
	//SceUID module;
	//SceUID modstart;

	//module = sceKernelLoadModule(mod, 0, NULL);

	//modstart = sceKernelStartModule(module, sizeof(module), NULL, NULL, NULL);
	//sceKernelStartModule(mod, 0, 0, 0, 0);
	SceUID module = sceIoOpen(mod, PSP_O_RDONLY, 0777);
	SceUID bytes = sceIoRead(module, buf, size);

	sceIoClose(module);


	sctrlHENLoadModuleOnReboot("/kd/umdman.prx", buf, bytes, BOOTLOAD_VSH | BOOTLOAD_UMDEMU | BOOTLOAD_GAME | BOOTLOAD_POPS);

	sceKernelExitGame();


	return 0;
}

int module_stop(SceSize args, void *argp) {
	return 0;
}


// Flush Instruction and Data Cache
/*void flushCache()
{
    // Flush Instruction Cache
    sceKernelIcacheInvalidateAll();

    // Flush Data Cache
    sceKernelDcacheWritebackInvalidateAll();
}


int OnModuleStart(SceModule2 *mod){

	SceUID fd = sceIoOpen("ms0:/dump.txt", PSP_O_CREAT|PSP_O_WRONLY|PSP_O_APPEND, 0777);
	sceIoWrite(fd, mod->modname, strlen(mod->modname));
	sceIoWrite(fd, "\n", 1);
	sceIoClose(fd);

    if(strcmp(mod->modname, "sceUmdMan_driver") == 0) {
        //patch_umd_idslookup(mod);
    	flushCache();
    }

    if (strcmp(mod->modname, "vsh_module") == 0){
        //patch_vsh_region_check(mod);
    	flushCache();
    }


    // Forward to previous Handler
	if(previous) previous(mod);
	else return 0;
}

int module_start(SceSize args, void *argp) {
	_sw(0x44000000, 0xBC800100);
	colorDebug(0xFF);
	previous = sctrlHENSetStartModuleHandler(OnModuleStart);
	return 0;
}

int module_stop(SceSize args, void *argp) {
	return 0;
}
*/
