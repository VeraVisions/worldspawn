# WorldSpawn
The editor we use to create levels. It was forked from NetRaidiant and was a result of necessity.
We wanted to move away from a proprietary toolchain that assumed a different texture coordinate system and had technical issues
the author would not ever get back to us about, so we had to take matters into our own hands.
Use it if you actually want to use the features listed below - note that they require a modified engine.
There's plenty of other editors for the first-party id Tech games.

## Key changes
- All texture coordinates use the Valve 220 format for compatibility with J.A.C.K. exported .map files
- Integration with our material system instead of long .shader files
- Support for vertex-color/alpha editing of patches using our new fixed patch format
- Support for VVM in the BSP compiler
- Support for HDR lightmaps in the BSP compiler
- Support for automatic cubemap surface-picking in the BSP compiler
- Lots of bug fixes, like the 'ghost-ent' bug, which places dummy ents at the center of your map
- Simplified build system, so less dependencies

## Compiling
To compile on a standard GNU/Linux system:
`LDFLAGS=-ldl gmake -j $(nproc)`

For BSD systems you'll have to point CFLAGS to whereever your package headers are installed.
gtkgtlext-1.0 is notorious for using both include and lib/gtkgtlext-1.0/include for headers. Yes.

Clang should also be supported, pass CC=clang and CXX=clang++ if you want to use it.

We don't work on NT. You're on your own with that one.

## Support
If you need help with this, you're better off using an alternative editor.
