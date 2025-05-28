//==============
// IFFChunkTool
//==============

#include "main.h"
#include "ifffile.h"

#include <proto/dos.h>
#include <proto/iffparse.h>

//=============================================================================
// IFFFile::OpenFile
//=============================================================================

bool IFFFile::OpenFile(const char *filepath, int32 mode)
{
	path = filepath;

	if (iff = AllocIFF())
	{
		if (handle = Open(filepath, mode))
		{
			int32 iffError;

			iff->iff_Stream = handle;
			InitIFFasDOS(iff);
			iffError = OpenIFF(iff, (mode == MODE_OLDFILE) ? IFFF_READ :
				IFFF_WRITE);

			if (iffError == 0)
			{
				opened = TRUE;
				return TRUE;
			}
			else return IFFProblem(iffError);
		}
		else return FileProblem();
	}
	else return Problem(LS(MSG_OUT_OF_MEMORY, "Out of memory"));
}

//=============================================================================
// IFFFile::~IFFFile()
//=============================================================================

IFFFile::~IFFFile()
{
	if (opened) CloseIFF(iff);

	if (handle)
	{
		if (!Close(handle)) FileProblem();
	}

	if (iff) FreeIFF(iff);
}
