all:
	mkdir -p ../build/plugins
	cd brushexport && $(MAKE)
	cd prtview && $(MAKE)
clean:
	-rm -rf ../build/plugins
	cd brushexport && $(MAKE) clean
	cd prtview && $(MAKE) clean
