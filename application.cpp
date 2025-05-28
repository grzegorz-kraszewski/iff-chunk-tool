//==============
// IFFChunkTool
//==============

#include "application.h"

#include <proto/dos.h>
#include <proto/iffparse.h>
#include <proto/utility.h>


//=============================================================================
// Application::Process()
//-----------------------------------------------------------------------------
// Source file is needed in all operation modes, so it is opened first, before
// mode checking.
//=============================================================================

bool Application::Process()
{
	const char *mode = arguments.getString(ARG_MODE);

	if (reader.OpenFile(arguments.getString(ARG_FROM)))
	{
		if (Stricmp(mode, "LIST") == 0)
		{
			return ListChunks();
		}

		if (Stricmp(mode, "APPEND") == 0)
		{
			//return AppendChunk();
		}

		if (Stricmp(mode, "INSERT") == 0)
		{
			//return InsertChunk();
		}

		if (Stricmp(mode, "REPLACE") == 0)
		{
			//return ReplaceChunk();
		}

		if (Stricmp(mode, "REMOVE") == 0)
		{
			//return RemoveChunk();
		}

		if (Stricmp(mode, "EXTRACT") == 0)
		{
			//return ExtractChunk();
		}

		return Problem(LS(MSG_ARGS_UNKNOWN_MODE, "Commandline arguments: unknown "
			"operation mode"));
	}

	return FALSE;
}

//=============================================================================
// Application::ListChunks()
//-----------------------------------------------------------------------------
// Lists FORM chunk, then all its subchunks.
//=============================================================================

bool Application::ListChunks()
{
	ContextNode *cn;
	char buf[6];
	int32 iffError;
	bool result = TRUE;

	cn = CurrentChunk(reader);
	Printf("FORM %s %ld\n", IDtoStr(cn->cn_Type, buf), cn->cn_Size);

	for (;;)
	{
		iffError = ParseIFF(reader, IFFPARSE_STEP);
		cn = CurrentChunk(reader);

		if (iffError == 0)
		{
			Printf("%s %ld\n", IDtoStr(cn->cn_ID, buf), cn->cn_Size);
		}
		else if (iffError == IFFERR_EOC)
		{
			if (cn->cn_ID == ID_FORM) break;
		}
		else
		{
			result = reader.IFFProblem();
			break;
		}
	}

	return result;
}
