SUBDIRS =  SystemCtrlForUser SystemCtrlForKernel

.PHONY: subdirs $(SUBDIRS)

all: subdirs
	$(MAKE) -C SystemCtrlForUser 
	$(MAKE) -C SystemCtrlForKernel 
	$(MAKE) -C colordebugger  
	$(MAKE) -C libpng  
	$(MAKE) -C KUBridge  
	$(MAKE) -C libmad  
	cp SystemCtrlForUser/*.a .
	cp SystemCtrlForKernel/*.a .
	cp colordebugger/*.a .
	cp libpng/*.a .
	cp KUBridge/*.a .
	cp libmad/lib/*.a .

distclean clean:
	$(MAKE) $@ -C SystemCtrlForUser $(OPT)
	$(MAKE) $@ -C SystemCtrlForKernel $(OPT)
	$(MAKE) $@ -C colordebugger $(OPT)
	$(MAKE) $@ -C libpng $(OPT)
	$(MAKE) $@ -C KUBridge $(OPT)
	$(MAKE) $@ -C libmad $(OPT)



subdirs: $(SUBDIRS)


clean:
	@rm -f *.a $(SUBDIRS)/*.o $(SUBDIRS)/*.a
	@echo CLEAN
