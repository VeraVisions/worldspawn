all:
	mkdir -p ../build/plugins
	cd archivezip && $(MAKE)
	cd archivewad && $(MAKE)
	cd archivepak && $(MAKE)
	cd entity && $(MAKE)
	cd image && $(MAKE)
	cd imagehl && $(MAKE)
	cd iqmmodel && $(MAKE)
	cd mapq3 && $(MAKE)
	cd model && $(MAKE)
	cd matsys && $(MAKE)
	cd shaders && $(MAKE)
	cd vfspk3 && $(MAKE)
	cd brushexport && $(MAKE)
	cd prtview && $(MAKE)
clean:
	-rm -rf ../build/plugins
	cd archivezip && $(MAKE) clean
	cd archivewad && $(MAKE) clean
	cd archivepak && $(MAKE) clean
	cd entity && $(MAKE) clean
	cd image && $(MAKE) clean
	cd imagehl && $(MAKE) clean
	cd iqmmodel && $(MAKE) clean
	cd mapq3 && $(MAKE) clean
	cd model && $(MAKE) clean
	cd shaders && $(MAKE) clean
	cd matsys && $(MAKE) clean
	cd vfspk3 && $(MAKE) clean
	cd brushexport && $(MAKE) clean
	cd prtview && $(MAKE) clean
