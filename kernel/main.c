#include <pspkernel.h>
#include <pspsdk.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspinit.h>

#include <model.h>
#include <kern_common.h>
#include <systemctrl.h>

#include <stdio.h>
#include <string.h>

#define NOP 0
#define JUMP(f) (0x08000000 | (((unsigned int)(f) >> 2) & 0x03ffffff))

PSP_MODULE_INFO("UMDREGION_Module", 0x3007, 1, 0);

STMOD_HANDLER previous;

enum {
	REGION_DEFAULT = 0, // default
	REGION_JAPAN = 3, // Japan
    REGION_AMERICA = 4, // America
    REGION_EUROPE = 5, // Europe
};

static void* umd_buf = NULL;

enum
{
    TA_079v1,
    TMU_001v1,
    TA_079v2,
    TMU_001v2,
    TA_079v3,
    TMU_002,
    TA_079v4,
    TA_079v5,
    TA_081v1,
    TA_081v2,
    TA_082,
    TA_086,
    TA_085v1,
    TA_085v2,
    TA_088v1_TA_088v2,
    TA_090v1,
    TA_088v3,
    TA_090v2,
    TA_090v3,
    TA_092,
    TA_091,
    TA_094,
    TA_093v1,
    TA_093v2,
    TA_095v1,
    TA_095v2,
    TA_096_TA_097,
    UNKNOWN
};

// Flush Instruction and Data Cache
void flushCache()
{
    // Flush Instruction Cache
    sceKernelIcacheInvalidateAll();

    // Flush Data Cache
    sceKernelDcacheWritebackInvalidateAll();
}


static int region_num = 0;

void read_region_file(){
    char byte;
	SceUID region = sceIoOpen(UMD_REGION_TYPE, PSP_O_RDONLY, 0777);
	sceIoRead(region, &byte, 1);
	sceIoClose(region);
	
	region_num = byte - '0';
}

// Find Import Library Pointer
SceLibraryStubTable * find_Import_Lib(SceModule2 * pMod, char * library)
{
    // Invalid Arguments
    if(pMod == NULL || library == NULL) return NULL;
    
    // Import Stub Table Start Address
    void * stubTab = pMod->stub_top;
    
    // Iterate Stubs
    int i = 0; while(i < pMod->stub_size)
    {
        // Cast Import Table
        SceLibraryStubTable * pImp = (SceLibraryStubTable *)(stubTab + i);
        
        // Matching Library discovered
        if(pImp->libname != NULL && strcmp(pImp->libname, library) == 0)
        {
            // Return Address
            return pImp;
        }
        
        // Move Pointer
        i += pImp->len * 4;
    }
    
    // Import Library not found
    return NULL;
}

// Find Import Stub Address
unsigned int find_Import_ByNID(SceModule2 * pMod, char * library, unsigned int nid)
{
    // Find Import Library
    SceLibraryStubTable * pImp = find_Import_Lib(pMod, library);
    
    // Found Import Library
    if(pImp != NULL)
    {
        // Iterate Imports
        int i = 0; for(; i < pImp->stubcount; i++)
        {
            // Matching Function NID
            if(pImp->nidtable[i] == nid)
            {
                // Return Function Stub Address
                return (unsigned int)(pImp->stubtable + 8 * i);
            }
        }
    }
    
    // Import Stub not found
    return 0;
}

void PSPOnModuleStart(SceModule2 * mod){

    
    if(strcmp(mod->modname, "sceUmdMan_driver") == 0) {
        patch_umd_idslookup(mod);
        goto flush;
    }

    if (strcmp(mod->modname, "vsh_module") == 0){
        patch_vsh_region_check(mod);
        goto flush;
    }

    if (strcmp(mod->modname, "impose_plugin_module") == 0){
        SceUID kthreadID = sceKernelCreateThread( "umd_region_change", &patch_umd_thread, 1, 0x20000, PSP_THREAD_ATTR_VFPU, NULL);
        if (kthreadID >= 0){
            // start thread and wait for it to end
            sceKernelStartThread(kthreadID, 0, NULL);
        }
        goto flush;
	}

flush:
    flushCache();

    // Forward to previous Handler
    if(previous) previous(mod);
}




int GetHardwareInfo(u32 *ptachyon, u32 *pbaryon, u32 *ppommel, u32 *pmb, u64 *pfuseid)
{
    // taken from Despertar del Cementerio
	u32 tachyon = 0;
	u32 baryon = 0;
	u32 pommel = 0;
	u32 mb = UNKNOWN;
    u64 fuseid;

    u32 (*SysregGetTachyonVersion)() = sctrlHENFindFunction("sceLowIO_Driver", "sceSysreg_driver", 0xE2A5D1EE);
    u64 (*SysregGetFuseId)() = sctrlHENFindFunction("sceLowIO_Driver", "sceSysreg_driver", 0x4F46EEDE);
    void (*SysconGetBaryonVersion)(u32*) = sctrlHENFindFunction("sceSYSCON_Driver", "sceSyscon_driver", 0x7EC5A957);
    void (*SysconGetPommelVersion)(u32*) = sctrlHENFindFunction("sceSYSCON_Driver", "sceSyscon_driver", 0xE7E87741);

    if (SysregGetTachyonVersion == NULL || SysregGetFuseId == NULL || SysconGetBaryonVersion == NULL || SysconGetPommelVersion == NULL) return -1;

	tachyon = SysregGetTachyonVersion();
	SysconGetBaryonVersion(&baryon);
	SysconGetPommelVersion(&pommel);
    fuseid = SysregGetFuseId();
	
	switch (tachyon)
	{
		case 0x00140000:
			switch(baryon)
			{
				case 0x00010600: mb = TA_079v1; break;
				case 0x00010601: mb = TMU_001v1; break;
				case 0x00020600: mb = TA_079v2; break;
				case 0x00020601: mb = TMU_001v2; break;
				case 0x00030600: mb = TA_079v3; break;
				case 0x00030601: mb = TMU_002; break;
			}
			break;

		case 0x00200000:
			switch(baryon)
			{
				case 0x00030600: mb = TA_079v4; break;
				case 0x00040600: mb = TA_079v5; break;
			}
			break;

		case 0x00300000:
			switch(baryon)
			{
				case 0x00040600:
					switch(pommel)
					{
						case 0x00000103: mb = TA_081v1; break;
						case 0x00000104: mb = TA_081v2; break;
					}
					break;
			}
			break;

		case 0x00400000:
			switch(baryon)
			{
				case 0x00114000: mb = TA_082; break;
				case 0x00121000: mb = TA_086; break;
			}
			break;

		case 0x00500000:
			switch(baryon)
			{
				case 0x0022B200: mb = TA_085v1; break;
				case 0x00234000: mb = TA_085v2; break;
				case 0x00243000:
					switch(pommel)
					{
						case 0x00000123: mb = TA_088v1_TA_088v2; break;
						case 0x00000132: mb = TA_090v1; break;
					}
					break;
			}
			break;

		case 0x00600000:
			switch(baryon)
			{
				case 0x00243000: mb = TA_088v3; break;
				case 0x00263100:
					switch(pommel)
					{
						case 0x00000132: mb = TA_090v2; break;
						case 0x00000133: mb = TA_090v3; break;
					}
					break;
				case 0x00285000: mb = TA_092; break;
			}
			break;

		case 0x00720000: mb = TA_091; break;

		case 0x00800000: mb = TA_094; break;

		case 0x00810000:
			switch(baryon)
			{
				case 0x002C4000:
					switch(pommel)
					{
						case 0x00000141: mb = TA_093v1; break;
						case 0x00000143: mb = TA_093v2; break;
					}
					break;
				case 0x002E4000: mb = TA_095v1; break;
			}
			break;


		case 0x00820000: mb = TA_095v2; break;

		case 0x00900000: mb = TA_096_TA_097; break;
	}

    *ptachyon = tachyon;
    *pbaryon = baryon;
    *ppommel = pommel;
    *pmb = mb;
    *pfuseid = fuseid;
    return 0;
}


// generate new UMD keys using idsRegeneration and inject into umdman
int sctrlArkReplaceUmdKeys(){
	u32 psp_model = sceKernelGetModel();

	if (psp_model == PSP_GO) return 0;

    int res = -1;

    // allocate memory buffer
    SceUID memid = sceKernelAllocPartitionMemory(2, "idsBuffer", PSP_SMEM_High, 512*0x20, NULL);
    if (memid < 0) goto fake_ids_end;

	void* big_buffer = sceKernelGetBlockHeadAddr(memid);

    // load and start idsRegeneration module
	// ms0:/PSP/GAME/UMD_Region_Changer/
    char path[UMD_REGION_PATH_SIZE];
    strcpy(path, UMD_REGION_PATH);
    strcat(path, "IDSREG.PRX");

    SceUID modid = sceKernelLoadModule(path, 0, NULL);
	if (modid >= 0){
	    res = sceKernelStartModule(modid, strlen(path) + 1, path, NULL, NULL);
	    if (res < 0){
	        goto fake_ids_end;
	    }
	}
	else {
	    goto fake_ids_end;
	}

    // find idsRegeneration exports
    int (*idsRegenerationSetup)(u32, u32, u32, u32, u64, u32, void*) = 
        sctrlHENFindFunction("pspIdsRegeneration_Driver", "idsRegeneration", 0xBDE13E76);
    int (*idsRegenerationCreateCertificatesAndUMDKeys)(void*) = 
        sctrlHENFindFunction("pspIdsRegeneration_Driver", "idsRegeneration", 0xB79A6C46);

    if (idsRegenerationCreateCertificatesAndUMDKeys == NULL || idsRegenerationSetup == NULL){
		res = -2;
		goto fake_ids_end;
	}
	
    u32 tachyon, baryon, pommel, mb; //, region;
    u64 fuseid;

	// get hardware info
	res = GetHardwareInfo(&tachyon, &baryon, &pommel, &mb, &fuseid);
    if (res < 0) goto fake_ids_end;

	// initialize idsRegeneration with hardware info and new region
	//
	//
    res = idsRegenerationSetup(tachyon, baryon, pommel, mb, fuseid, region_num, NULL);
	if (res < 0) goto fake_ids_end;

	// calculate the new UMD keys
    res = idsRegenerationCreateCertificatesAndUMDKeys(big_buffer);
	if (res < 0) goto fake_ids_end;

    // copy the generated UMD keys to the buffer in umdman
    memcpy(umd_buf, big_buffer+(512*2), 512*5);
    flushCache();

	res = 0;

    // free resources
    fake_ids_end:
    sceKernelFreePartitionMemory(memid);
    return res;
}

static int (*_idStorageLookup)(u16 key, u32 offset, void *buf, u32 len);
static int fakeIdStorageLookupForUmd(u16 key, u32 offset, void *buf, u32 len){
	// obtain buffer where UMD keys are stored in umdman.prx
    if (offset == 0 && key == 0x102){
		umd_buf = buf;
	}
    return _idStorageLookup(key, offset, buf, len); // passthrough
}

void patch_umd_idslookup(SceModule2* mod){
    // this patch allows us to obtain the buffer where umdman stores the UMD keys
    _idStorageLookup = sctrlHENFindFunction("sceIdStorage_Service", "sceIdStorage_driver", 0x6FE062D1);
    u32 addr = find_Import_ByNID(mod, "sceIdStorage_driver", 0x6FE062D1);
    _sw(JUMP(&fakeIdStorageLookupForUmd), addr);
	_sw(NOP, addr+4);
}

void patch_vsh_region_check(SceModule2* mod){
	// patch to remove region check in VSH
	u32 addr = find_Import_ByNID(mod, "sceVshBridge", 0x5C2983C2);
	_sw(JR_RA, addr);
	_sw(LI_V0(1), addr+4);
}


int patch_umd_thread(SceSize args, void *argp){
    sceKernelDelayThread(1000000); // wait for system to load
    read_region_file();
    sctrlArkReplaceUmdKeys(); // replace UMD keys
    sceKernelExitDeleteThread(0);
    return 0;
}


int module_start(SceSize args, void *argp) {

	previous = sctrlHENSetStartModuleHandler(PSPOnModuleStart);

	return 0;
}

