//=============================================================================
// PathBuilder builds AmigaDOS paths with autoallocating buffer.
// USAGE:
// * create object,
// * call AddPathPart() as many times as needed,
// * get path with Path(),
// * if Path() returns NULL it means out of memory.
//=============================================================================

#include "main.h"

#include <proto/dos.h>

#define PBUF_INCREMENT  128


class PathBuilder
{
	char *path;
	int32 currentSize;
	bool Extend();

	public:

	bool ready;
	PathBuilder() : path(NULL), currentSize(0), ready(TRUE) {}
	~PathBuilder();
	void AddPathPart(const char *part);
	void StripFileName() { *PathPart(path) = 0x00; }
	char* Path();
};
