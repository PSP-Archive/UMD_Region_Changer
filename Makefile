UMDREGIONROOT=$(CURDIR)
export UMDREGIONROOT

SUBDIRS = user \
		#  kernel



all: #subdirs # user kernel
	$(MAKE) $@ -C user
	$(MAKE) $@ -C kernel
	@mkdir -p $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer || true
	@cp $(UMDREGIONROOT)/user/EBOOT.PBP $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/
	@cp $(UMDREGIONROOT)/kernel/*.prx $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/
	@cp $(UMDREGIONROOT)/IDSREG.PRX $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/
	@echo "Build Done"

clean:
	$(MAKE) $@ -C user
	$(MAKE) $@ -C kernel
	@rm -rf $(UMDREGIONROOT)/dist *.zip
	@find . -type f -name "*.o" -delete -o -name "*.elf" -delete -o -name "*.prx" -delete -o -name "*.PBP" -delete -o -name "*.SFO" -delete
	@echo CLEAN

pkg: package

package:
	zip -r UMD_Region_Changer.zip *

release: all
	cp README.md dist/ && cd dist/ && zip -r UMD_Region_Changer.zip .

