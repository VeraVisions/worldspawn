all:
	mkdir -p ./build
	cd libs && $(MAKE)
	cd src && $(MAKE)
	cd plugins && $(MAKE)
	cd tools && $(MAKE)
	cd resources && $(MAKE)

clean:
	cd libs && $(MAKE) clean
	cd src && $(MAKE) clean
	cd plugins && $(MAKE) clean
	cd tools && $(MAKE) clean
