all:
	mkdir -p ../build/plugins
	cd archivezip && $(MAKE)
	cd entity && $(MAKE)
	cd image && $(MAKE)
	cd iqmmodel && $(MAKE)
	cd mapq3 && $(MAKE)
	cd model && $(MAKE)
	cd shaders && $(MAKE)
	cd vfspk3 && $(MAKE)
	cd brushexport && $(MAKE)
	cd prtview && $(MAKE)
clean:
	-rm -rf ../build/plugins
	cd archivezip && $(MAKE) clean
	cd entity && $(MAKE) clean
	cd image && $(MAKE) clean
	cd iqmmodel && $(MAKE) clean
	cd mapq3 && $(MAKE) clean
	cd model && $(MAKE) clean
	cd shaders && $(MAKE) clean
	cd vfspk3 && $(MAKE) clean
	cd brushexport && $(MAKE) clean
	cd prtview && $(MAKE) clean
