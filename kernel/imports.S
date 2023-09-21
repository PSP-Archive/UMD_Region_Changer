.set noreorder

#include "pspstub.s"

	STUB_START	"SystemCtrlForKernel",0x00090000,0x00020005
	STUB_FUNC	0xCE0A654E,sctrlHENLoadModuleOnReboot
	STUB_END

