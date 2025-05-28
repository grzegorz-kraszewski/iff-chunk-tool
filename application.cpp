//==============
// IFFChunkTool
//==============

#include "application.h"

//=============================================================================
// Application::Application()
//=============================================================================

Application::Application(CallArgs &args) : arguments(args), ready(FALSE)
{
	if (reader.OpenFile(args.getString(0)))
	{
		if (writer.OpenFile(args.getString(1)))
		{
			ready = TRUE;
		}
	}
}
