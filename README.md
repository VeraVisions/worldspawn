# ![WorldSpawn Logo](icon.png) WorldSpawn
The worlds most opinionated fork of Radiant.

The editor we use at Vera Visions to create BSP levels.
It was forked from NetRadiant in June of 2018 and was a result of necessity.

We wanted to move away from a proprietary toolchain that had technical issues the developer would not ever get back to us about, so we ended up here.

Use it if you actually want to use the features listed below - note that they require a modified engine as our BSP format is different from standard idTech 3 BSP.
You will not be able to make levels compatible with other games and engines.

There's plenty of other editors for the first-party id Tech games.

![Screenshot](docs/screen.jpg)

## Editor Changes
- Valve 220 format is used **top to bottom**, **imported & exported**, with texture coords handled internally the same way, including the compiler
- Integration with our **own material format** (no more giant .shader files)
- Support for vertex-color/alpha editing of patches using our new **fixed patch format**, allowing technologies such as **4-way texture blending** and whatever your designers can imagine
- Gracefully deals with duplicate entity attribute key/value pairs, to support features like Source Engine style **Input/Output system** for triggers
- Support for **VVM** (based on IQM) model format in the BSP compiler as well as the editor
- Support for **internal** and external **High-Dynamic-Range lightmaps** in the BSP compiler
- Support for **cubemap aware surfaces** in the BSP compiler
- Support for our patchDef2WS and patchDef3WS curved surfaces in the BSP compiler
- *More bug-fixes than you could possibly imagine*

## Why
Back then there was no fork of Radiant supporting the 220 format like said program had exported.
We had to make that happen on our own, since then we've expanded and kept changing more to meet our own needs.
On top of that, there's other benefits from working in our own Radiant fork.

Rapid experimentation and tech development doesn't fit into an editor like GtkRadiant,
which aims to be stable, reliable and support a specific set of games really, really well.

We can afford to break compat here in order to achieve our goals. It will break, be unstable
and all that jazz at the cost of supporting the greatest and latest of we're working on.

Some code in here (like the IQM support in the compiler) has made it into other Radiant forks
when we deem it to be mature. However not everything in here is going to be of interest
to other Radiant forks.

A lot of the changes are specific to our BSP format & engine too.
So tech like per-surface picked and generated environment maps will never make it into other games.
Sorry!

## Compiling
To compile on a standard GNU/Linux system:
`LDFLAGS=-ldl make -j $(nproc)`

On BSD you should probably use GNU make right now. The Makefiles are simple enough however.
Clang should also be supported, pass `CC=clang and CXX=clang++` if you want to use it.

On NT you'll probably have a build command-line likes this:
`CC=x86_64-w64-mingw32-gcc CXX=x86_64-w64-mingw32-g++ PKG_CONFIG_PATH=/usr/x86_64-w64-mingw32/sys-root/mingw/lib/pkgconfig make -j 4`
Not all works yet, but stay tuned.

It'll compile everything into a subdirectory 'build'. At the end it'll copy files from ./resources into it too.

In the Nuclide SDK, build_engine.sh will call make with the appropriate flags for Linux/BSD automatically and
move it into Nuclide's ./bin directory.

## Dependencies
* GNU make
* gcc-core
* gcc-c++ (or clang++)
* gtk2-devel
* gtkglext-devel
* libxml2-devel
* libjpeg8-devel
* minizip-devel

## Support
As mentioned before, if you need help with this: you're on your own.

Please use [GtkRadiant](https://github.com/TTimo/GtkRadiant) if you want to make levels for existing games.
