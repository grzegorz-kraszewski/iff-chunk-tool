//==============
// IFFChunkTool
//==============

#include "iffreader.h"

#include <proto/iffparse.h>
#include <proto/dos.h>            /* for tests only */

//=============================================================================
// IFFReader::OpenFile()
//=============================================================================

bool IFFReader::OpenFile(const char *filepath)
{
	iffType = 0;

	if (IFFFile::OpenFile(filepath, MODE_OLDFILE))
	{
		int32 iffError;
		ContextNode *cn;

		iffError = ParseIFF(iff, IFFPARSE_STEP);

		if (iffError == 0)
		{
			cn = CurrentChunk(iff);

			if (cn->cn_ID == ID_FORM)
			{
				iffType = cn->cn_Type;
				return TRUE;
			}
			else return Problem("IFF file is not FORM");
		}
		else return IFFProblem(iffError);
	}
	else return FALSE;
}
