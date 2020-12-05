# WorldSpawn
The editor we use to create levels. It was forked from NetRaidiant and was a result of necessity.
We wanted to move away from a proprietary toolchain that assumed a different texture coordinate system and had itechnical issues
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

## Compiling
You can compile the editor directly from within the Nuclide src tree using the build_editor.sh script.
It'll also integrate it properly into the filesystem you're working with.
Other than that, it can be built in either debug or release form with the scripts provided in this tree.

The codebase also compiles the editor under MSYS2, but the compiler will currently segfault on NT.

## Support
The whole thing is held together with duct-tape.
The codebase it is based on is not very portable and while effort had been made to rectify that,
it's just such a time consuming affair.
If you've got any issues, feel free to let us know, but we can't promise any support right away.
Here be dragons, etc.
