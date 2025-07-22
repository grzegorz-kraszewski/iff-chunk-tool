//=============================================================================
// ChunkReplacer copies all the IFF file chunks except of specified one. For
// this one chunk copying the original contents is inhibited and contents for
// a file or commandline argument is written instead.
//=============================================================================

#include "chunkcopier.h"
#include "extchunkid.h"


class ChunkReplacer : public ChunkCopier
{
	ChunkDataSource *data;
	ExtendedChunkID id;
	int32 chunkCounter;

	public:

	ChunkReplacer(const char *sourceName, const char *destName, const char
	 *chunk, ChunkDataSource *data);
	virtual bool PreChunkWork(ContextNode *cn);
	virtual bool FormEndWork();
	virtual ~ChunkReplacer() {}
};
