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
	IFFHandle *iff;
	bool FileProblem() { return SysProblem(path); }
	bool opened;

	public:

	IFFFile(const char *filename, int32 mode);
	static void Localize();
	const char* FileName() { return path; }
	bool IFFProblem(int32 error);
	virtual ~IFFFile();
};

#endif    /* IFFCHUNKTOOL_IFFFILE_H */
