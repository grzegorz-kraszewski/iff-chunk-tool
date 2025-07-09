#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/iffparse.h>

#include <dos/rdargs.h>
#include <workbench/startup.h>

#include "main.h"
#include "application.h"


Library *IFFParseBase, *LocaleBase, *UtilityBase;
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

	if (UtilityBase = OpenLibrary("utility.library", 39))
	{
		if (IFFParseBase = OpenLibrary("iffparse.library", 39))
		{
			CallArgs args;

			if (args.ready)
			{
				Application app(args);

				if (app.Process()) result = RETURN_OK;
			}
	
			result = RETURN_OK;
			CloseLibrary(IFFParseBase);
		}
		else Problem(LS(MSG_NO_IFFPARSE_LIBRARY, "Can't open iffparse.library v39+"));
	}
	else Problem(LS(MSG_NO_UTILITY_LIBRARY, "Can't open utility.library v39+"));

	if (LocaleBase)
	{
		CloseCatalog(Cat);
		CloseLibrary(LocaleBase);
	}

	return result;
}

//=============================================================================
// StrLen()
//=============================================================================

int32 StrLen(const char *s)
{
	const char *v = s;

	while (*v) v++;
	return (int32)(v - s);
}

//=============================================================================
// ValidateChunkID()
//-----------------------------------------------------------------------------
// Converts a string to IFF chunk ID and validates it. Returns ID as uint32, or
// 0 for failure. String must have 4 characters and must pass iffparse.library/
// GoodID().
//=============================================================================

uint32 ValidateChunkID(const char *str)
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