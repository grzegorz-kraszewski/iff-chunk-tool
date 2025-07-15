//=============================================================================
// ChunkAdder appends a chunk to the IFF file. It is done by implementing 
// FormEndWork() method of ChunkCopier.
//=============================================================================

#include "chunkcopier.h"


class ChunkAdder : public ChunkCopier
{
	ChunkDataSource *data;
	uint32 chunkId;

	public:

	ChunkAdder(const char *sourceName, const char *destName, const char *chunk,
	 ChunkDataSource *data);
	virtual bool FormEndWork();
	virtual ~ChunkAdder() { DD("ChunkAdder"); }
};
