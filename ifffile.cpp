//==============
// IFFChunkTool
//==============

#include "main.h"
#include "ifffile.h"

#include <proto/dos.h>
#include <proto/iffparse.h>

//=============================================================================
// IFF errror messages
//-----------------------------------------------------------------------------
// As no custom streams/hooks are used, IFFERR_NOHOOK and IFF_RETURN2CLIENT
// should never happen. They will be reported as "unknown error [-11|-12]".
//=============================================================================

static const char* IFFErrorMessages[10] = 
{
	"unexpected end of file",         // for IFFERR_EOF
	"unexpected end of IFF chunk",    // for IFFERR_EOC
	"IFF property without scope",     // for IFFERR_NOSCOPE
	"out of memory",                  // for IFFERR_NOMEM
	"file read error",                // for IFFERR_READ
	"file write error",               // for IFFERR_WRITE
	"file seek error",                // for IFFERR_SEEK
	"file data corrupt",              // for IFFERR_MANGLED
	"IFF syntax error",               // for IFFERR_SYNTAX
	"not an IFF file"                 // for IFFERR_NOTIFF
};

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
	int32 index = -1 - error;

	if ((index >= 0) && (index < 10))
	{
		Printf("\"%s\": %s.\n", path, IFFErrorMessages[index]);
	}
	else
	{
		Printf("\"%s\": unknown IFF error %ld.\n", path, error);
	}

	return FALSE;
}

//=============================================================================
// IFFFile::Localize()
//-----------------------------------------------------------------------------
// This function replaces entries in IFFErrorMessages[] with translations.
//=============================================================================

void IFFFile::Localize()
{
	// TODO: implement IFFFile::Localize()
}
