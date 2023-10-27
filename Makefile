UMDREGIONROOT=$(CURDIR)
export UMDREGIONROOT

SUBDIRS = user \
		#  kernel



all: #subdirs # user kernel
	$(MAKE) $@ -C libs
	$(MAKE) $@ -C user
	$(MAKE) $@ -C kernel
	$(MAKE) $@ -C me_fix
	@mkdir -p $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer || true
	@cp $(UMDREGIONROOT)/user/EBOOT.PBP $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/
	@cp $(UMDREGIONROOT)/kernel/*.prx $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/
	@cp $(UMDREGIONROOT)/IDSREG.PRX $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/
	@cp $(UMDREGIONROOT)/res/matrix_new.png $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/BG.PNG
	@cp $(UMDREGIONROOT)/res/ACCEPT.MP3 $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/ACCEPT.MP3
	@cp $(UMDREGIONROOT)/res/SELECTION.MP3 $(UMDREGIONROOT)/dist/PSP/GAME/UMD_Region_Changer/SELECTION.MP3
	mkdir $(UMDREGIONROOT)/dist/SEPLUGINS || true
	@cp $(UMDREGIONROOT)/me_fix/UMD_Region_Changer_ME_fix.prx $(UMDREGIONROOT)/dist/SEPLUGINS/
	@cp $(UMDREGIONROOT)/me_fix/___README_FIRST____.MD $(UMDREGIONROOT)/dist/SEPLUGINS/
	@echo "Build Done"

clean:
	$(MAKE) $@ -C user
	$(MAKE) $@ -C kernel
	$(MAKE) $@ -C me_fix
	@rm -rf $(UMDREGIONROOT)/dist *.zip
	@find . -type f -name "*.o" -delete -o -name "*.elf" -delete -o -name "*.prx" -delete -o -name "*.PBP" -delete -o -name "*.SFO" -delete -o -name "*.a" -delete
	@echo CLEAN

pkg: package

package:
	zip -r UMD_Region_Changer.zip *

release: all
	cp README.md dist/ && cd dist/ && zip -r UMD_Region_Changer.zip .

