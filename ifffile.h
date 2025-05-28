//==============
// IFFChunkTool
//==============

#ifndef IFFCHUNKTOOL_IFFFILE_H
#define IFFCHUNKTOOL_IFFFILE_H

#include "main.h"

#include <dos/dos.h>
#include <libraries/iffparse.h>

class IFFFile
{
	protected:

	BPTR handle;
	IFFHandle *iff;
	const char* path;
	bool opened;
	bool FileProblem() { return SysProblem(path); }
	bool IFFProblem() { return Problem("arghh"); }

	public:

	IFFFile() : handle(NULL), path(NULL), iff(NULL), opened(FALSE) {}
	bool OpenFile(const char *filepath, int32 mode);
	~IFFFile();
};

#endif    /* IFFCHUNKTOOL_IFFFILE_H */
