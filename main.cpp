#include <proto/exec.h>
#include <proto/dos.h>

#include <dos/rdargs.h>
#include <workbench/startup.h>

#include "main.h"

Library *IFFParseBase, *LocaleBase;
Catalog *Cat;

//=============================================================================
// Problem
//=============================================================================

int32 Problem(const char* msg)
{
	Printf("%s.\n", msg);
	return FALSE;
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
