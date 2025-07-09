//==============
// IFFChunkTool
//==============

#include "iffreader.h"

#include <proto/iffparse.h>
#include <proto/dos.h>            /* for tests only */

//=============================================================================
// IFFReader::IFFReader()
//=============================================================================

IFFReader::IFFReader(const char *filepath) : IFFFile(filepath, MODE_OLDFILE), iffType(0)
{
	if (opened)
	{
		int32 iffError;
		ContextNode *cn;

		iffError = ParseIFF(iff, IFFPARSE_STEP);

		if (iffError == 0)
		{
			cn = CurrentChunk(iff);
			if (cn->cn_ID == ID_FORM) iffType = cn->cn_Type;
			else Problem("IFF file is not FORM");
		}
		else IFFProblem(iffError);
	}
}
