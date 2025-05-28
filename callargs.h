//==============
// IFFChunkTool
//==============

#include <dos/rdargs.h>

#include "main.h"

class CallArgs
{
	int32 vals[2];
	RDArgs *args;

	public:

	bool ready;
	CallArgs();
	~CallArgs();
	const char* getString(int32 index) { return (const char*)vals[index]; }
};
