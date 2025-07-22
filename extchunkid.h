//=============================================================================
// ExtendedChunkID
// The class helps to handle multiple chunks with the same ID in a file
// Such chunks are referenced as '<id>/<n>' where 'id' is standard 4-letter
// chunk ID, 'n' is a decimal integer starting from 0 (0 is the first chunk)
// Notation is backwards compatible. Simply '<id>' means '<id>/0'.
//=============================================================================

#include "main.h"

class ExtendedChunkID
{
	public:

	uint32 chunkId;
	uint32 number;

	bool valid() const
	{
		return (chunkId != 0);
	}

	ExtendedChunkID() : chunkId(0), number(0) {}

	ExtendedChunkID(uint32 chunkId) : chunkId(chunkId), number(0) {}

	ExtendedChunkID(const char *id);

	bool operator==(const ExtendedChunkID& b) const
	{
		return ((chunkId == b.chunkId) && (number == b.number));
	}
};
