//==============
// IFFChunkTool
//==============

#include <proto/dos.h>

#include "main.h"
#include "callargs.h"

//=============================================================================
// CallArgs::CallArgs()
//=============================================================================

CallArgs::CallArgs()
{
	vals[0] = 0;
	vals[1] = 0;
	ready = FALSE;

	if (args = ReadArgs("FROM/A,TO/A", vals, NULL)) ready = TRUE;
	else SysProblem(LS(MSG_COMMANDLINE_ARGS, "Program arguments error"));
}

//=============================================================================
// CallArgs::~CallArgs()
//=============================================================================

CallArgs::~CallArgs()
{
	if (args) FreeArgs(args);
}
