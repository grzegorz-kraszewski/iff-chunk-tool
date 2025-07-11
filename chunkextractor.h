//=============================================================================================
// ChunkExtractor is a subclass of ChunkPicker. As its ChunkWork() it stores chunk contents to
// a file.
//=============================================================================================

#include "main.h"
#include "chunkpicker.h"

#include <libraries/iffparse.h>

class ChunkExtractor : public ChunkPicker
{
	const char *outname;
	BPTR output;
	void *copybuf;
	bool CopyLoop(ContextNode *cn);

	public:

	ChunkExtractor(const char *filepath, const char *chunkid, const char *destname);
	virtual bool ChunkWork(ContextNode *cn);
};
