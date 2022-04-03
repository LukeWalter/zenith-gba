
//{{BLOCK(cave)

//======================================================================
//
//	cave, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 4 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 128 + 2048 = 2688
//
//	Time-stamp: 2022-04-02, 21:07:29
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CAVE_H
#define GRIT_CAVE_H

#define caveTilesLen 128
extern const unsigned short caveTiles[64];

#define caveMapLen 2048
extern const unsigned short caveMap[1024];

#define cavePalLen 512
extern const unsigned short cavePal[256];

#endif // GRIT_CAVE_H

//}}BLOCK(cave)
