//==============
// IFFChunkTool
//==============

#include "main.h"
#include "ifffile.h"

#include <proto/dos.h>

//=============================================================================
// IFFFile::OpenFile
//=============================================================================

bool IFFFile::OpenFile(const char *filepath, int32 mode)
{
	path = filepath;

	if (handle = Open(filepath, mode)) return TRUE;
	else return FileProblem();
}

//=============================================================================
// IFFFile::~IFFFile()
//=============================================================================

IFFFile::~IFFFile()
{
	if (handle)
	{
		if (!Close(handle)) FileProblem();
	}
}
