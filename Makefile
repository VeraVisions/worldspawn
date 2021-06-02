all:
	mkdir -p ./build
	cd libs && $(MAKE)
	cd radiant && $(MAKE)
	cd plugins && $(MAKE)
	cd tools && $(MAKE)
	cd resources && $(MAKE)

clean:
	cd libs && $(MAKE) clean
	cd radiant && $(MAKE) clean
	cd plugins && $(MAKE) clean
	cd tools && $(MAKE) clean
