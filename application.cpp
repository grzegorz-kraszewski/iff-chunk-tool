//==============
// IFFChunkTool
//==============

#include "application.h"

#include <proto/dos.h>
#include <proto/iffparse.h>

//=============================================================================
// Application::Application()
//=============================================================================

Application::Application(CallArgs &args) : arguments(args), ready(FALSE)
{
	if (reader.OpenFile(args.getString(0)))
	{
		if (writer.OpenFile(args.getString(1)))
		{
			char type[6];

			Printf("\"%s\": source IFF form of type '%s'.\n", reader.FileName(),
				IDtoStr(reader.iffType, type));
			ready = TRUE;
		}
	}
}
