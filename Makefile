UMDREGIONROOT=$(CURDIR)
export UMDREGIONROOT

SUBDIRS = libs \
		  user \
		#  kernel



all: #subdirs # user kernel
	$(MAKE) $@ -C libs
	$(MAKE) $@ -C user
	$(MAKE) $@ -C kernel
	@mkdir -p $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer || true
	@cp $(UMDREGIONROOT)/user/EBOOT.PBP $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/
	@cp $(UMDREGIONROOT)/kernel/*.prx $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/
	@cp $(UMDREGIONROOT)/IDSREG.PRX $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/
	@echo "Build Done"

clean:
	@rm -rf $(UMDREGIONROOT)/dist *.zip
	@find . -type f -name "*.a" -delete -o -name "*.o" -delete -o -name "*.elf" -delete -o -name "*.prx" -delete -o -name "*.PBP" -delete -o -name "*.SFO" -delete
	@echo CLEAN

package:
	zip -r UMD_Region_Changer.zip *
