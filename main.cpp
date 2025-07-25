#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/iffparse.h>

#include <dos/rdargs.h>
#include <workbench/startup.h>

#include "main.h"
#include "locale.h"
#include "application.h"


Library *IFFParseBase, *UtilityBase;
AppLocale Ls("IffChunkTool.catalog");

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
		else Problem(Ls[MSG_NO_IFFPARSE_LIBRARY]);
	}
	else Problem(Ls[MSG_NO_UTILITY_LIBRARY]);

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
	if (str)
	{
		if (StrLen(str) == 4)
		{
			uint32 id = MAKE_ID(str[0], str[1], str[2], str[3]);
			if (GoodID(id)) return id;
		}
		else Printf(Ls[MSG_INVALID_CHUNK_ID], str);
	}
	else PutStr(Ls[MSG_CHUNK_ARGUMENT_REQUIRED]);

	return 0;
}

//=============================================================================
// ValidateTypeID()
//-----------------------------------------------------------------------------
// Converts a string to IFF type ID and validates it. Returns ID as uint32, or
// 0 for failure. String must have 4 characters and must pass iffparse.library/
// GoodType().
//=============================================================================

uint32 ValidateTypeID(const char *str)
{
	if (str)
	{
		if (StrLen(str) == 4)
		{
			uint32 id = MAKE_ID(str[0], str[1], str[2], str[3]);
			if (GoodType(id)) return id;
		}
		else Printf(Ls[MSG_INVALID_TYPE_ID], str);
	}
	else PutStr(Ls[MSG_IFF_TYPE_ARGUMENT_REQUIRED]);

	return 0;
}


static void ProcPutChar(void)
{
	asm("move.b d0,(a3)+");
}


static void ProcCountChars(void)
{
	asm("addq.l #1,(a3)");
}


void VFmtPut(char *dest, const char *fmt, int32 *args)
{
	RawDoFmt(fmt, args, ProcPutChar, dest);
}


void FmtPut(char *dest, const char *fmt, ...)
{
	int32 *_args = ((int32*)&fmt) + 1;
	VFmtPut(dest, fmt, _args);
}
