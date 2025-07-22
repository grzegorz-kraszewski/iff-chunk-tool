//=============================================================================
// ChunkLister list all its IFF chunks (IDs and lengths).
//=============================================================================

#include "iffreader.h"
#include "rplist.h"
#include "extchunkid.h"


class ChunkListEntry : public RpNode<ChunkListEntry>
{
	ExtendedChunkID extendedId;
	int32 chunkLength;

	public:

	bool showNumber;

	ChunkListEntry(uint32 chunkId, int32 chunkLength) : extendedId(chunkId),
		chunkLength(chunkLength), showNumber(FALSE) { }

	void PrintAsForm();
	void Print();
};


class ChunkLister : public IFFReader
{
	RpList<ChunkListEntry> chunks;
	bool ScanChunks();
	void SetChunkNumbers();
	void ListChunks();

	public:

	ChunkLister(const char *filepath) : IFFReader(filepath) {}
	virtual bool Parse();
};
