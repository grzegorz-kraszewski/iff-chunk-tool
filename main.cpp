#include <proto/exec.h>
#include <proto/dos.h>

#include <dos/rdargs.h>
#include <workbench/startup.h>

#include "main.h"
#include "application.h"


Library *IFFParseBase, *LocaleBase;
Catalog *Cat;

//=============================================================================
// Problem
//-----------------------------------------------------------------------------
// General purpose error reporting function. Prints the message.
//=============================================================================

int32 Problem(const char* msg)
{
	Printf("%s.\n", msg);
	return FALSE;
}

//=============================================================================
// SysProblem
//-----------------------------------------------------------------------------
// Error reporting function for errors indicated by IoErr(). Uses message as
// a header and appends IoErr() description to it.
//=============================================================================

int32 SysProblem(const char *msg)
{
	PrintFault(IoErr(), msg);
}

//=============================================================================
// Main()
//=============================================================================

int32 Main(WBStartup *wbmsg)
{
	int32 result = RETURN_FAIL;

	if (LocaleBase = OpenLibrary("locale.library", 39))
	{
		Cat = OpenCatalogA(NULL, "iffchunktool.catalog", NULL);
	}

	if (IFFParseBase = OpenLibrary("iffparse.library", 39))
	{
		CallArgs args;

		if (args.ready)
		{
			Application app(args);

			if (app.ready)
			{
				result = app.Process() ? RETURN_OK : RETURN_ERROR;
			}
		}

		result = RETURN_OK;
		CloseLibrary(IFFParseBase);
	}
	else Problem(LS(MSG_NO_IFFPARSE_LIBRARY, "Can't open iffparse.library v39+"));

	if (LocaleBase)
	{
		CloseCatalog(Cat);
		CloseLibrary(LocaleBase);
	}

	return result;
}
