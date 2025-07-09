#include "chunklister.h"

#include <proto/dos.h>

bool ChunkLister::Parse()
{
	ContextNode *cn;
	int32 iffError;
	bool result = TRUE;

	cn = CurrentChunk(iff);
	Printf("FORM %s %ld\n", IDtoStr(cn->cn_Type, idbuf), cn->cn_Size);

	for (;;)
	{
		iffError = ParseIFF(iff, IFFPARSE_STEP);
		cn = CurrentChunk(iff);

		if (iffError == 0)
		{
			Printf("%s %ld\n", IDtoStr(cn->cn_ID, idbuf), cn->cn_Size);
		}
		else if (iffError == IFFERR_EOC)
		{
			if (cn->cn_ID == ID_FORM) break;
		}
		else
		{
			result = IFFProblem(iffError);
			break;
		}
	}

	return result;
}