//==============
// IFFChunkTool
//==============

#include "main.h"
#include "locale.h"
#include "application.h"
#include "chunklister.h"
#include "chunkextractor.h"
#include "chunkdumper.h"
#include "chunkremover.h"
#include "chunkadder.h"
#include "chunkreplacer.h"
#include "chunkinjector.h"

#include <proto/dos.h>
#include <proto/iffparse.h>
#include <proto/utility.h>
#include <proto/exec.h>

//=============================================================================
// Application::PrepareDestination()
//=============================================================================

const char* Application::PrepareDestination()
{
	const char *destination = arguments[ARG_TO];

	if (destination) return destination;

	inPlace = TRUE;
	builder.AddPathPart(arguments[ARG_FROM]);
	builder.StripFileName();
	builder.AddPathPart("iffchunktool.temp");
	return builder.Path();
}

//=============================================================================
// Application::TempFileToSource()
//=============================================================================

void Application::TempFileToSource()
{
	if (DeleteFile(arguments[ARG_FROM]))
	{
		if (Rename(builder.Path(), arguments[ARG_FROM])) {}
		else SysProblem(Ls[MSG_ERR_TEMP_FILE_RENAME]);
	}
	else SysProblem(Ls[MSG_DELETE_SOURCE_BEFORE_RENAME]);
}

//=============================================================================
// Application::Process()
//=============================================================================

bool Application::Process()
{
	const char *mode = arguments[ARG_MODE];
	const char *source = arguments[ARG_FROM];
	const char *destination = arguments[ARG_TO];
	bool result = FALSE;
	IFFReader *processor = NULL;
	ChunkDataSource *data = NULL;

	if (Stricmp(mode, "APPEND") == 0)
	{
		destination = PrepareDestination();
		data = PrepareDataSource();
		processor = new ChunkAdder(source, destination, arguments[ARG_CHUNK],
		 data);
	}
	else if (Stricmp(mode, "DUMP") == 0)
	{
		processor = new ChunkDumper(source, arguments[ARG_CHUNK]);
	}
	else if (Stricmp(mode, "EXTRACT") == 0)
	{
		processor = new ChunkExtractor(source, arguments[ARG_CHUNK],
		 arguments[ARG_DATAFILE]);
	}
	else if (Stricmp(mode, "INSERT") == 0)
	{
		destination = PrepareDestination();
		data = PrepareDataSource();
		processor = new ChunkInjector(source, destination,
		 arguments[ARG_CHUNK], data, arguments[ARG_AFTER]);
	}
	else if (Stricmp(mode, "LIST") == 0)
	{
		processor = new ChunkLister(source);
	}
	else if (Stricmp(mode, "REMOVE") == 0)
	{
		destination = PrepareDestination();
		processor = new ChunkRemover(source, destination,
		 arguments[ARG_CHUNK]);
	}
	else if (Stricmp(mode, "REPLACE") == 0)
	{
		destination = PrepareDestination();
		data = PrepareDataSource();
		processor = new ChunkReplacer(source, destination,
		 arguments[ARG_CHUNK], data);
	}
	else if (Stricmp(mode, "NEW") == 0)
	{
		result = CreateEmptyIFF(arguments[ARG_FROM], arguments[ARG_CHUNK]);
	}
	else Printf(Ls[MSG_UNKNOWN_OPERATION_MODE], mode);

	if (processor)
	{
		if (processor->ready) result = processor->Parse();
		delete processor;
	}

	if (inPlace) TempFileToSource();
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
	char *str;
	const char *path;
	ChunkDataSource *data = NULL;

	str = (char*)arguments[ARG_CONTENTS];
	path = arguments[ARG_DATAFILE];

	if (str)
	{
		if (!path) data = new ChunkDataString(str);
		else
		{
			Problem(Ls[MSG_BOTH_STRING_AND_FILE_SPECIFIED]);
			return NULL;
		}
	}
	else
	{
		if (path) data = new ChunkDataFile(path);
		else
		{
			Problem(Ls[MSG_NO_CHUNK_CONTENT_SPECIFIED]);
			return NULL;
		}
	}

	if (data->ready) return data;

	delete data;
	return NULL;
}

//=============================================================================
// Application::CreateEmptyIFF()
//=============================================================================

bool Application::CreateEmptyIFF(const char *filePath, const char *typeString)
{
	uint32 type = ValidateTypeID(typeString);
	bool result = FALSE;

	if (type)
	{
		IFFWriter writer(filePath, type);
		result = writer.ready;
	}

	return result;
}
