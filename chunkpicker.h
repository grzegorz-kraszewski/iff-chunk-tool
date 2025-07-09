
//=============================================================================================
// ChunkPicker is an abstract class, which opens an IFF stream for reading, picks a chunk given
// by ID and performs some action on the chunk data. Actions are defined in subclasses.
//=============================================================================================

#ifndef IFFCHUNKTOOL_CHUNKPICKER_H
#define IFFCHUNKTOOL_CHUNKPICKER_H

#include "main.h"
#include "iffreader.h"

#include <libraries/iffparse.h>

class ChunkPicker : public IFFReader
{
	uint32 chunkId;

	public:

	bool ready;
	ChunkPicker(const char *filepath, uint32 chid) : IFFReader(filepath), chunkId(chid) {}
	virtual bool Parse();
	virtual bool ChunkWork(ContextNode *cn) = 0;
};

#endif    // IFFCHUNKTOOL_CHUNKPICKER_H
