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
			//---------------------------------------------------
			// All the operation modes below require copyBuffer.
			//----------------------------------------------------

			if (copyBuffer = AllocMem(COPY_BUFFER_SIZE, MEMF_ANY))
			{
				if (Stricmp(mode, "EXTRACT") == 0) return ExtractChunk(chunkID);

				//----------------------------------------------------
				// All the operation modes below require IFF writer.
				//----------------------------------------------------

				// TODO: if output is not given, chunks should be written to a temporary file
				// then original should be deleted and temporary renamed to original

				if (output && writer.OpenFile(output, reader.iffType))
				{
					if (Stricmp(mode, "APPEND") == 0)
					{
						return AppendChunk(chunkID);
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
						return RemoveChunk(chunkID);
					}

					return Problem(LS(MSG_ARGS_UNKNOWN_MODE, "Commandline "
						"arguments: unknown operation mode"));
				}

				FreeMem(copyBuffer, COPY_BUFFER_SIZE);
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
	bool success = TRUE, found = FALSE;

	while (success)
	{
		iffError = ParseIFF(reader, IFFPARSE_STEP);
		cn = CurrentChunk(reader);

		if (iffError == 0)
		{
			if (cn->cn_ID == id)
			{
				found = TRUE;
				success = WriteChunkContents(cn);
				break;
			}
		}
		else if (iffError == IFFERR_EOC)
		{
			if (cn->cn_ID == ID_FORM)
			{
				if (!found)
				{
					Printf(LS(MSG_CHUNK_NOT_FOUND_IN_SOURCE, "No '%s' chunk "
						"found in the source file.\n"), IDtoStr(id, idBuf));
					success = FALSE;
				}

				break;
			}
		}
		else success = reader.IFFProblem(iffError);
	}

	return success;
}

//=============================================================================
// Application::CopyChunk()
//-----------------------------------------------------------------------------
// Reader object is assumed to be positioned at start of chunk described by the
// ContextNode. Copy of this chunk is pushed to writer object. It is assumed
// that copyBuffer is allocated.
//=============================================================================

bool Application::CopyChunk(IFFReader &reader, IFFWriter &writer, ContextNode *cn)
{
	int32 iffError, bytesToCopy;
	bool success = TRUE;

	iffError = PushChunk(writer, cn->cn_Type, cn->cn_ID, cn->cn_Size);

	if (iffError == 0)
	{
		bytesToCopy = cn->cn_Size;

		while (success && (bytesToCopy > 0))
		{
			int32 blocksize = bytesToCopy;

			if (blocksize > COPY_BUFFER_SIZE) blocksize = COPY_BUFFER_SIZE;
			iffError = ReadChunkBytes(reader, copyBuffer, blocksize);

			if (iffError >= 0)
			{
				iffError = WriteChunkBytes(writer, copyBuffer, blocksize);
				if (iffError >= 0) bytesToCopy -= blocksize;
				else success = writer.IFFProblem(iffError);
			}
			else success = reader.IFFProblem(iffError);
		}

		iffError = PopChunk(writer);

		//-------------------------------------------------------------------
		// If read or write failed (success == FALSE), successful PopChunk()
		// should not set 'success' to TRUE.
		//-------------------------------------------------------------------

		if (success && (iffError < 0)) success = writer.IFFProblem(iffError);
	}
	else success = writer.IFFProblem(iffError);

	return success;
}

//=============================================================================
// Application::RemoveChunk()
//=============================================================================

bool Application::RemoveChunk(uint32 id)
{
	ContextNode *cn;
	int32 iffError;
	bool success = TRUE, found = FALSE;

	while (success)
	{
		iffError = ParseIFF(reader, IFFPARSE_STEP);
		cn = CurrentChunk(reader);

		if (iffError == 0)
		{
			if (cn->cn_ID != id) success = CopyChunk(reader, writer, cn);
			else found = TRUE;
		}
		else if (iffError == IFFERR_EOC)
		{
			if (cn->cn_ID == ID_FORM)
			{
				if (!found)
				{
					Printf(LS(MSG_CHUNK_NOT_FOUND_IN_SOURCE,
						"No '%s' chunk found in the source file.\n"),
						IDtoStr(id, idBuf));
				}

				break;
			}
		}
		else success = reader.IFFProblem(iffError);
	}

	return success;
}

//=============================================================================
// Application::AppendChunk()
//=============================================================================

bool Application::AppendChunk(uint32 id)
{
	ContextNode *cn;
	int32 iffError;
	bool success = TRUE, found = FALSE;

	while (success)
	{
		iffError = ParseIFF(reader, IFFPARSE_STEP);
		cn = CurrentChunk(reader);

		if (iffError == 0)
		{
			if (cn->cn_ID != id) success = CopyChunk(reader, writer, cn);
			else found = TRUE;
		}
		else if (iffError == IFFERR_EOC)
		{
			if (cn->cn_ID == ID_FORM)
			{
				success = PushChunkFromSource(id);
				break;
			}
		}
		else success = reader.IFFProblem(iffError);
	}

	return success;
}

//=============================================================================
// Application::PushChunkFromSource()
//=============================================================================

bool Application::PushChunkFromSource(uint32 id)
{
	const char *str;
	const char *path;

	str = arguments.getString(ARG_CONTENTS);
	path = arguments.getString(ARG_DATAFILE);

	if (str)
	{
		if (!path) return PushChunkFromString(str, id);
		else return Problem(LS(MSG_BOTH_STRING_AND_FILE_SPECIFIED, "Specify "
			"either string or file as chunk contents, not both"));
	}
	else
	{
		if (path) return PushChunkFromFile(path, id);
		else return Problem(LS(MSG_NO_CHUNK_CONTENT_SPECIFIED, "Specify "
			"string or file as chunk contents"));
	}
}

//=============================================================================
// Application::PushChunkFromString()
//=============================================================================

bool Application::PushChunkFromString(const char *string, uint32 id)
{
	int32 iffError;
	int32 len;
	bool success;

	success = TRUE;
	len = StrLen(string);

	if ((iffError = PushChunk(writer, reader.iffType, id, len)) == 0)
	{
		if ((iffError = WriteChunkBytes(writer, string, len)) < 0)
		{
			success = writer.IFFProblem(iffError);
		}

		iffError = PopChunk(writer);
		if (success && (iffError < 0)) success = writer.IFFProblem(iffError);
	}
	else success = writer.IFFProblem(iffError);

	return success;
}

//=============================================================================
// Application::PushChunkFromFile()
//=============================================================================

bool Application::PushChunkFromFile(const char *path, uint32 id)
{
}
