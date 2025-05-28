//==============
// IFFChunkTool
//==============

#ifndef IFFCHUNKTOOL_IFFFILE_H
#define IFFCHUNKTOOL_IFFFILE_H

#include "main.h"

#include <dos/dos.h>
#include <proto/iffparse.h>

//=============================================================================
// Instances of IFFFile (and subclasses) may be used as IFFHandle in calls to 
// iffparse.library functions, thanks to operator IFFHandle*().
//=============================================================================

class IFFFile
{
	protected:

	BPTR handle;
	const char* path;
	bool opened;
	IFFHandle *iff;
	bool FileProblem() { return SysProblem(path); }

	public:

	operator IFFHandle*() { return iff; }
	IFFFile() : handle(NULL), path(NULL), iff(NULL), opened(FALSE) {}
	bool OpenFile(const char *filepath, int32 mode);
	const char* FileName() { return path; }
	bool IFFProblem() { return Problem("arghh"); }
	~IFFFile();
};

#endif    /* IFFCHUNKTOOL_IFFFILE_H */
