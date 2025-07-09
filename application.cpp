//==============
// IFFChunkTool
//==============

#include "main.h"
#include "application.h"
#include "chunklister.h"
#include "chunkextractor.h"
#include "chunkdumper.h"

#include <proto/dos.h>
#include <proto/iffparse.h>
#include <proto/utility.h>
#include <proto/exec.h>

//=============================================================================
// Application::Process()
//=============================================================================

bool Application::Process()
{
	const char *mode = arguments.getString(ARG_MODE);
	const char *source = arguments.getString(ARG_FROM);
	bool result = FALSE;
	IFFReader *processor = NULL;

	if (Stricmp(mode, "APPEND") == 0)
	{

	}
	else if (Stricmp(mode, "DUMP") == 0)
	{
		uint32 chunkid = ValidateChunkID(arguments.getString(ARG_CHUNK));

		if (chunkid)
		{
			processor = new ChunkDumper(source, chunkid);
		}
		else result = Problem("CHUNK argument required in this operation mode");
	}
	else if (Stricmp(mode, "EXTRACT") == 0)
	{
		const char *dataname = arguments.getString(ARG_DATAFILE);
		uint32 chunkid = ValidateChunkID(arguments.getString(ARG_CHUNK));

		if (dataname)
		{
			if (chunkid)
			{
				processor = new ChunkExtractor(source, chunkid, dataname);
			}
			else result = Problem("CHUNK argument required in this operation mode");
		}
		else result = Problem("DATA argument required in this operation mode");
	}
	else if (Stricmp(mode, "INSERT") == 0)
	{

	}
	else if (Stricmp(mode, "LIST") == 0)
	{
		processor = new ChunkLister(source);
	}
	else if (Stricmp(mode, "REMOVE") == 0)
	{

	}
	else if (Stricmp(mode, "REPLACE") == 0)
	{

	}
	else Printf("Unknown operation mode '%s'.\n", mode);

	if (processor && (processor->iffType != 0)) result = processor->Parse();
	return result;
}

#if 0

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

#endif
