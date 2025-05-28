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
				char type[6];

				IDtoStr(cn->cn_Type, type);
				Printf("\"%s\": IFF FORM of type '%s', %ld bytes.\n", path,
					type, cn->cn_Size);
				return TRUE;
			}
			else Problem("IFF file is not FORM.");
		}
		else return IFFProblem();
	}
	return FALSE;
}
