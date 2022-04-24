
//{{BLOCK(cave)

//======================================================================
//
//	cave, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 1024 tiles not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 32768 + 2048 = 35328
//
//	Time-stamp: 2022-04-22, 22:52:41
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CAVE_H
#define GRIT_CAVE_H

#define caveTilesLen 32768
extern const unsigned short caveTiles[16384];

#define caveMapLen 2048
extern const unsigned short caveMap[1024];

#define cavePalLen 512
extern const unsigned short cavePal[256];

#endif // GRIT_CAVE_H

//}}BLOCK(cave)
