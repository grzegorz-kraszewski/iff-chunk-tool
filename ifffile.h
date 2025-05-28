//==============
// IFFChunkTool
//==============

#ifndef IFFCHUNKTOOL_IFFFILE_H
#define IFFCHUNKTOOL_IFFFILE_H

#include "main.h"

#include <dos/dos.h>

class IFFFile
{
	protected:

	BPTR handle;
	const char* path;
	bool FileProblem() { return SysProblem(path); }

	public:

	IFFFile() : handle(NULL), path(NULL) {}
	bool OpenFile(const char *filepath, int32 mode);
	~IFFFile();
};

#endif    /* IFFCHUNKTOOL_IFFFILE_H */
