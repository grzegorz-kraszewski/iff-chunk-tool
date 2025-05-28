//==============
// IFFChunkTool
//==============

#include "application.h"

#include <proto/dos.h>
#include <proto/iffparse.h>
#include <proto/utility.h>


//=============================================================================
// Application::Process()
//=============================================================================

void Application::Process()
{
	const char *mode = arguments.getString(ARG_MODE);

	if (Stricmp(mode, "LIST") == 0)
	{
		return;
	}

	if (Stricmp(mode, "APPEND") == 0)
	{
		return;
	}

	if (Stricmp(mode, "INSERT") == 0)
	{
		return;
	}

	if (Stricmp(mode, "REPLACE") == 0)
	{
		return;
	}

	if (Stricmp(mode, "REMOVE") == 0)
	{
		return;
	}

	if (Stricmp(mode, "EXTRACT") == 0)
	{
		return;
	}

	Problem(LS(MSG_ARGS_UNKNOWN_MODE, "Commandline arguments: unknown "
		"operation mode"));
}
