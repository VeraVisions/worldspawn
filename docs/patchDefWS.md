We neede the ability to manipulate vertex colors for patch segments.
So we did two things, first our map compiler (vmap) supports patchDef3 (for fixed tesselation patches) alongside patchDef2.
Those are named `patchDef2WS` and `patchDef3WS`. We mostly use the latter, but some maps still have the old format.

Regarding patchDef3 type patches:

```
patchDef3WS
{
next/rusty1_to_rusty5
( 3 3 0 0 0 0 0 )
```

First two parms (3 and 3) are the width and height of our patch - as expected. That should be the same as before.
The third and fourth arguments (0 and 0) specify additional subdivisions.
If those are set to -1 and -1, it'll just do the 'automatic', adjustable patch.
So far this shouldn't be much different from say, Doom 3. That's where patchDef3 originates from what I understand.

Now, onto what makes this 'WS" special:

```
(
( ( 192 -160 64 0 0 ) ( 192 -160 0 0 -0.5 1 1 1 0.6000000238 ) ( 192 -160 -64 0 -1 1 1 1 0.25 ) )
( ( 192 -96 64 0.6172839999 0 ) ( 192 -96 0 0.6172839999 -0.5 1 1 1 0.5 ) ( 192 -96 -64 0.6172839999 -1 1 1 1 0 ) )
( ( 192 -32 64 1.2345679998 0 ) ( 192 -32 0 1.2345679998 -0.5 1 1 1 0.75 ) ( 192 -32 -64 1.2345679998 -1 1 1 1 0 ) )
)
```

The following lines represent the individual 'columns' of the patch, and each () component contains the position (X,Y,Z) and texture mapping info (S,T). Usually there's 5 bits of information in each token, but we added 4 more - making it 9 in total. Those are simply normalized R, G, B and A values.

Our compiler, vmap, then deals with them accordingly. We needed this to build lots of specialized terrain and it has proven extremely useful. I don't expect anyone adopt this into their map compiler (as this also requires engines to recognize patch types with color). But if an editor like NetRadiant-Custom was to support it, it might get adopted more elsewhere and most importantly, we'd probably switch over in a heartbeat.

Our interface for manipulating them in WorldSpawn was always quite bad. We'd just scroll through a list of row/column id's and set the RGBA values normally with no real way to preview what we were doing (as that would also require the editor to parse and understand our material + GLSL). But there is probably some way you can involve clicking a segment and setting its RGBA. Just an idea, I could never figure out a good way to make it happen, but an editor like NRC has seen so many changes it might be more feasible to implement than in our NetRadiant fork of eons ago.