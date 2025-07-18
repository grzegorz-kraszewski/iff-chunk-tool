//=============================================================================
// ChunkCopier is a class, which opens two IFF streams, source and destination.
// Its main task is to copy chunks from source to destination, one by one.
// Subclasses can perform additional operations at start or at end of given
// chunk and start and end of FORM. Note the important difference between
// the source IFF and destination IFF. The source is inherited from superclass.
// Destination is created dynamically in the constructor.
//=============================================================================

#ifndef IFFCHUNKTOOL_CHUNKCOPIER_H
#define IFFCHUNKTOOL_CHUNKCOPIER_H

#include "main.h"
#include "iffreader.h"
#include "iffwriter.h"
#include "sysfile.h"

#include <libraries/iffparse.h>

//=============================================================================
// ChunkDataSource is a helper class, which represents data source for INSERT
// and APPEND, it is an abstract class with two subclasses: ChunkDataFile and
// ChunkDataString.
//=============================================================================

class ChunkDataSource
{
	public:

	bool ready;
	ChunkDataSource() : ready(FALSE) {}
	virtual bool CopyData(IFFWriter *dest, void *buffer) = 0;
	virtual ~ChunkDataSource() {}
};


class ChunkDataFile : public ChunkDataSource
{
	SysFile source;

	public:

	ChunkDataFile(const char *filepath) : source(filepath, MODE_OLDFILE)
		{ ready = source.ready; }
	virtual bool CopyData(IFFWriter *dest, void *buffer);
	virtual ~ChunkDataFile() {}
};


class ChunkDataString : public ChunkDataSource
{
	uint8 *string;
	int32 strsize;
	bool Unescape();

	public:

	ChunkDataString(char *string);
	virtual bool CopyData(IFFWriter *dest, void *buffer);
	virtual ~ChunkDataString() {}
};


//=============================================================================
// ChunkCopier class itself.
//=============================================================================

class ChunkCopier : public IFFReader
{
	IFFWriter *destination;
	void *copyBuffer;
	bool CopyChunk (ContextNode *cn);

	protected:

	uint32 chunkId;
	bool copyThisChunk;      // subclasses can use this flag to skip a chunk when copying
	bool chunkFound;
	bool PushChunkFromDataSource(uint32 chunkid, ChunkDataSource *source);

	public:

	ChunkCopier(const char *sourceName, const char *destName);
	~ChunkCopier();
	bool Parse();
	virtual bool FormStartWork() {}
	virtual bool PreChunkWork(ContextNode *cn) {}
	virtual bool PostChunkWork(ContextNode *cn) {}
	virtual bool FormEndWork();
};

#endif     // IFFCHUNKTOOL_CHUNKCOPIER_H
