//============================================================================================
// ChunkCopier is a class, which opens two IFF streams, source and destination. Its main task
// is to copy chunks from source to destination, one by one. Subclasses can perform
// additional operations at start or at end of given chunk. Note the important difference
// between source IFF and destination IFF. The source is inherited from superclass.
// Destination is created dynamically in the constructor.
//=============================================================================================

#include "main.h"
#include "iffreader.h"
#include "iffwriter.h"

#include <libraries/iffparse.h>


class ChunkCopier : public IFFReader
{
	IFFWriter *destination;
	void *copyBuffer;
	bool CopyChunk (ContextNode *cn);

	protected:

	bool copyThisChunk;      // subclasses can use this flag to skip a chunk when copying

	public:

	ChunkCopier(const char *sourceName, const char *destName);
	~ChunkCopier();
	bool Parse();
	virtual bool FormStartWork();
	virtual bool PreChunkWork(ContextNode *cn);
	virtual bool PostChunkWork(ContextNode *cn);
	virtual bool FormEndWork();
};
