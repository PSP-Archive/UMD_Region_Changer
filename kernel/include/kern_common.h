#ifndef KERN_COMMON_H
#define KERN_COMMON_H

#define UMD_REGION_NAME "UMD_Region_Changer"

#define UMD_REGION_PATH_SIZE 256

#define GO_ROOT "ef0:/"
#define MS_ROOT "ms0:/"

#define GAME_PATH "PSP/GAME/"

#define CONCAT(a, b) a ## b
#define EXPAND_CONCAT(a, b) CONCAT(a, b)


#define UMD_REGION_PATH MS_ROOT GAME_PATH UMD_REGION_NAME "/"
#define UMD_REGION_FILE "region.txt"
#define UMD_REGION_TYPE UMD_REGION_PATH UMD_REGION_FILE



#define JR_RA 0x03E00008

#define LI_V0(n) ((0x2402 << 16) | ((n) & 0xFFFF))

#define MAKE_DUMMY_FUNCTION_RETURN_1(a) \
    _sw(JR_RA, a);\
    _sw(LI_V0(1), a + 4);\


#include <systemctrl.h>

void patch_umd_idslookup(SceModule2* mod);
int patch_umd_thread(SceSize args, void *argp);
void patch_vsh_region_check(SceModule2* mod);

#endif
