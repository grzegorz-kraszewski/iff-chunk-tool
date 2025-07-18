//==============
// IFFChunkTool
//==============

#include "main.h"
#include "locale.h"
#include "ifffile.h"

#include <proto/dos.h>
#include <proto/iffparse.h>

//=============================================================================
// IFFFile::IFFFile
//=============================================================================

IFFFile::IFFFile(const char *filepath, int32 mode) :
	opened(FALSE),
	handle(NULL),
	path(NULL),
	iff(NULL)
{
	DC("IFFFile");
	path = filepath;

	if (iff = AllocIFF())
	{
		if (handle = Open(filepath, mode))
		{
			int32 iffError;

			iff->iff_Stream = handle;
			InitIFFasDOS(iff);
			iffError = OpenIFF(iff, (mode == MODE_OLDFILE) ? IFFF_READ : IFFF_WRITE);
			if (iffError == 0) opened = TRUE;
			else IFFProblem(iffError);
		}
		else FileProblem();
	}
	else IFFProblem(IFFERR_NOMEM);
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
	DD("IFFFile");
}

//=============================================================================
// IFFFile::IFFProblem()
//=============================================================================

bool IFFFile::IFFProblem(int32 error)
{
	int32 index = -1 - error + MSG_IFFERR_UNEXPECTED_EOF;

	if ((index >= 0) && (index < 10))
	{
		Printf("\"%s\": %s.\n", path, Ls[index]);
	}
	else
	{
		Printf(Ls[MSG_IFFERR_UNKNOWN], path, error);
	}

	return FALSE;
}
