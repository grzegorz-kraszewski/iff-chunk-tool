//==============
// IFFChunkTool
//==============

#include "iffreader.h"

#include <proto/iffparse.h>
#include <proto/dos.h>            /* for tests only */

//=============================================================================
// IFFReader::IFFReader()
//=============================================================================

IFFReader::IFFReader(const char *filepath) : IFFFile(filepath, MODE_OLDFILE),
 iffType(0), ready(FALSE)
{
	DC("IFFReader");

	if (opened)
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
				ready = TRUE;
			}
			else Problem(LS(MSG_IFF_FILE_IS_NOT_FORM, "IFF file is not FORM"));
		}
		else IFFProblem(iffError);
	}
}
