#ifndef KERN_COMMON_H
#define KERN_COMMON_H

#define CONCAT(a, b) a ## b
#define EXPAND_CONCAT(a, b) CONCAT(a, b)




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
