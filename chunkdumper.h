//=============================================================================================
// ChunkDumper is a subclass of ChunkPicker. As its ChunkWork() it hexdumps the chunk contents
// to stdout.
//=============================================================================================

#include "main.h"
#include "chunkpicker.h"

#include <libraries/iffparse.h>

class ChunkDumper : public ChunkPicker
{
	public:

	ChunkDumper(const char *filepath, uint32 chid) : ChunkPicker(filepath, chid) {}
	virtual bool ChunkWork(ContextNode *cn);
};
