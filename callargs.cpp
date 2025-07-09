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
	for (int16 i = NUM_ARGS - 1; i >= 0; i--) vals[i] = 0;
	ready = FALSE;

	if (args = ReadArgs("FROM/A,MODE/A,CHUNK,CONTENTS,DATA/K,AFTER/K,TO/K",
		vals, NULL)) ready = TRUE;
	else SysProblem(LS(MSG_COMMANDLINE_ARGS, "Program arguments error"));
}

//=============================================================================
// CallArgs::~CallArgs()
//=============================================================================

CallArgs::~CallArgs()
{
	if (args) FreeArgs(args);
}
