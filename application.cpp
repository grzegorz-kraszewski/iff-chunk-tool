//==============
// IFFChunkTool
//==============

#include "main.h"
#include "application.h"
#include "chunklister.h"
#include "chunkextractor.h"
#include "chunkdumper.h"
#include "chunkremover.h"
#include "chunkadder.h"

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
	const char *destination = arguments.getString(ARG_TO);
	bool result = FALSE;
	IFFReader *processor = NULL;
	ChunkDataSource *data = NULL;

	if (Stricmp(mode, "APPEND") == 0)
	{
		data = PrepareDataSource();
		processor = new ChunkAdder(source, destination,
		 arguments.getString(ARG_CHUNK), data);
	}
	else if (Stricmp(mode, "DUMP") == 0)
	{
		processor = new ChunkDumper(source, arguments.getString(ARG_CHUNK));
	}
	else if (Stricmp(mode, "EXTRACT") == 0)
	{
		processor = new ChunkExtractor(source, arguments.getString(ARG_CHUNK),
			arguments.getString(ARG_DATAFILE));
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
		processor = new ChunkRemover(source, arguments.getString(ARG_TO),
			arguments.getString(ARG_CHUNK));
	}
	else if (Stricmp(mode, "REPLACE") == 0)
	{

	}
	else Printf("Unknown operation mode '%s'.\n", mode);

	if (processor)
	{
		if (processor->ready) result = processor->Parse();
		delete processor;
	}

	if (data) delete data;
	return result;
}

//=============================================================================
// Application::PrepareDataSource()
//
// Used in APPEND, INSERT and REPLACE modes. They need a file data source if
// DATA argument is specified, string data source otherwise.
//=============================================================================

ChunkDataSource* Application::PrepareDataSource()
{
	const char *str;
	const char *path;
	ChunkDataSource *data = NULL;

	str = arguments.getString(ARG_CONTENTS);
	path = arguments.getString(ARG_DATAFILE);

	if (str)
	{
		if (!path) data = new ChunkDataString(str);
		else
		{
			Problem(LS(MSG_BOTH_STRING_AND_FILE_SPECIFIED, "Specify either "
			 "string or file as chunk contents, not both"));
			return NULL;
		}
	}
	else
	{
		if (path) data = new ChunkDataFile(path);
		else
		{
			Problem(LS(MSG_NO_CHUNK_CONTENT_SPECIFIED, "Specify string or "
			 "file as chunk contents"));
			return NULL;
		}
	}

	if (data->ready) return data;

	delete data;
	return NULL;
}
