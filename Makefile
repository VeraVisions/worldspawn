all:
	cp -Rv ./resources/ ./build/
	cd libs && $(MAKE)
	cd radiant && $(MAKE)
	cd plugins && $(MAKE)
	cd tools && $(MAKE)

clean:
	-rm -rf ./build
	cd libs && $(MAKE) clean
	cd radiant && $(MAKE) clean
	cd plugins && $(MAKE) clean
	cd tools && $(MAKE) clean
