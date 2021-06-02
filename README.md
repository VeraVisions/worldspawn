# WorldSpawn
The worlds most opinionated fork of QER.

The editor we use at Vera Visions to create BSP levels.
It was forked from NetRaidiant and was a result of necessity originally.

We wanted to move away from a proprietary toolchain that assumed a different texture coordinate system and had technical issues the developer would not ever get back to us about, so we had to take matters into our own hands.

Use it if you actually want to use the features listed below - note that they require a modified engine.

There's plenty of other editors for the first-party id Tech games.

## Key changes
- All texture coordinates use the Valve 220 format for compatibility with WorldCraft exported .map files
- Integration with our material system (goodbye .shader files)
- Support for vertex-color/alpha editing of patches using our new fixed patch format, allowing technologies such as 4-way texture blending and whatever your designers can imagine.
- Support for VVM/IQM model format in the BSP compiler
- Support for High-Dynamic-Range lightmaps in the BSP compiler
- Support for Cubemap aware surfaces in the BSP compiler
- Lots of bug fixes, like the 'ghost-ent' bug, which places dummy ents at the center of your map which somehow had flown other peoples radar for 20+ years
- Simplified build system, so less dependencies!

## Compiling
To compile on a standard GNU/Linux system:
`LDFLAGS=-ldl gmake -j $(nproc)`

For BSD systems you'll have to point CFLAGS to whereever your package headers are installed.
gtkgtlext-1.0 is notorious for using both include and lib/gtkgtlext-1.0/include for headers. Yes.

Clang should also be supported, pass CC=clang and CXX=clang++ if you want to use it.

We don't work on NT. You're on your own with that one.

## Support
If you need help with this, you're better off using an alternative editor.
Compatibility is not a priority here.
