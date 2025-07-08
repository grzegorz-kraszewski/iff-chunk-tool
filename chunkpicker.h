
//=============================================================================================
// ChunkPicker is an abstract class, which opens an IFF stream for reading, picks a chunk given
// by ID and performs some action on the chunk data. Actions are defined in subclasses.
//---------------------------------------------------------------------------------------------
// Application
//   +---> ChunkPicker
//           +--> IFFFile : IFFReader
//=============================================================================================

#include "main.h"
#include "iffreader.h"

#include <libraries/iffparse.h>

class ChunkPicker
{
	IFFReader input;
	uint32 chunkId;

	public:

	bool ready;
	ChunkPicker(const char *filename, uint32 chid);
	~ChunkPicker();
	bool Parse();
	virtual bool ChunkWork(ContextNode *cn) = 0;
};
