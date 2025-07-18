//==============
// IFFChunkTool
//==============

#include <dos/rdargs.h>

#include "main.h"

#define ARG_FROM       0
#define ARG_MODE       1
#define ARG_CHUNK      2
#define ARG_CONTENTS   3
#define ARG_DATAFILE   4
#define ARG_AFTER      5
#define ARG_TO         6

#define NUM_ARGS       7


class CallArgs
{
	int32 vals[NUM_ARGS];
	RDArgs *args;

	public:

	bool ready;
	CallArgs();
	~CallArgs();
	const char* operator[](int32 index) const { return (const char*)vals[index]; }
};
