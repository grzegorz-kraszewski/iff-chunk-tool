//==============
// IFFChunkTool
//==============

#include "application.h"

#include <proto/dos.h>
#include <proto/iffparse.h>
#include <proto/utility.h>
#include <proto/exec.h>

//=============================================================================
// StrLen()
//=============================================================================

static int32 StrLen(const char *s)
{
	const char *v = s;

	while (*v) v++;
	return (int32)(v - s);
}

//=============================================================================
// Application::ValidateChunkID()
//-----------------------------------------------------------------------------
// Converts a string to IFF chunk ID and validates it. Returns ID as uint32, or
// 0 for failure. String must have 4 characters and must pass iffparse.library/
// GoodID().
//=============================================================================

uint32 Application::ValidateChunkID(const char *str)
{
	if (str && (StrLen(str) == 4))
	{
		uint32 id = MAKE_ID(str[0], str[1], str[2], str[3]);
		if (GoodID(id)) return id;
	}

	Printf(LS(MSG_INVALID_CHUNK_ID, "'%s' is not a valid IFF chunk identifier."
		"\n"), str);

	return 0;
}

//=============================================================================
// Application::Process()
//-----------------------------------------------------------------------------
// Source file is needed in all operation modes, so it is opened first, before
// mode checking.
//=============================================================================

bool Application::Process()
{
	const char *mode = arguments.getString(ARG_MODE);
	const char *output = arguments.getString(ARG_TO);
	uint32 chunkID;


	if (reader.OpenFile(arguments.getString(ARG_FROM)))
	{
		if (Stricmp(mode, "LIST") == 0) return ListChunks();

		//-------------------------------------------------
		// All the operation modes below require chunk ID.
		//-------------------------------------------------

		if (chunkID = ValidateChunkID(arguments.getString(ARG_CHUNK)))
		{
			if (Stricmp(mode, "EXTRACT") == 0) return ExtractChunk(chunkID);

			//----------------------------------------------------
			// All the operations modes below require IFF writer.
			//----------------------------------------------------

			if (output && writer.OpenFile(output))
			{
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

				return Problem(LS(MSG_ARGS_UNKNOWN_MODE, "Commandline "
					"arguments: unknown operation mode"));
			}
		}
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
			result = reader.IFFProblem(iffError);
			break;
		}
	}

	return result;
}

//=============================================================================
// Application::WriteChunkContents()
//=============================================================================

bool Application::WriteChunkContents(ContextNode *cn)
{
	bool result = FALSE;
	const char *path = arguments.getString(ARG_DATAFILE);
	BPTR dest;
	void *buffer;
	int32 bytesRead, bytesWritten;

	if (path)
	{
		if (dest = Open(path, MODE_NEWFILE))
		{
			if (buffer = AllocMem(cn->cn_Size, MEMF_ANY))
			{
				bytesRead = ReadChunkBytes(reader, buffer, cn->cn_Size);

				if (bytesRead >= 0)
				{
					bytesWritten = Write(dest, buffer, bytesRead);

					if (bytesWritten < bytesRead) SysProblem(path);
					result = TRUE;
				}
				else reader.IFFProblem(bytesRead);

				FreeMem(buffer, cn->cn_Size);
			}
			else Problem(LS(MSG_OUT_OF_MEMORY, "Out of memory"));

			if (!Close(dest)) SysProblem(path);
		}
		else SysProblem(path);
	}
	else Problem(LS(MSG_NO_OUTPUT_DATAFILE, "No output file specified ("
		"use DATAFILE argument)"));

	return result;
}

//=============================================================================
// Application::ExtractChunk()
//-----------------------------------------------------------------------------
// Writes contents of specified chunk to a file. Requires DATAFILE argument.
//=============================================================================

bool Application::ExtractChunk(uint32 id)
{
	ContextNode *cn;
	int32 iffError;
	bool result = TRUE, found = FALSE;


	for (;;)
	{
		iffError = ParseIFF(reader, IFFPARSE_STEP);
		cn = CurrentChunk(reader);

		if (iffError == 0)
		{
			if (cn->cn_ID == id)
			{
				found = TRUE;
				result = WriteChunkContents(cn);
				break;
			}
		}
		else if (iffError == IFFERR_EOC)
		{
			if (cn->cn_ID == ID_FORM)
			{
				if (!found)
				{
					char buf[6];

					Printf("No '%s' chunk found in the source file.\n", IDtoStr(id, buf));
					result = FALSE;
				}

				break;
			}
		}
		else
		{
			result = reader.IFFProblem(iffError);
			break;
		}
	}

	return result;
}
