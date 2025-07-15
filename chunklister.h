//=============================================================================================
// ChunkLister list all its IFF chunks (IDs and lengths).
//=============================================================================================

#include "iffreader.h"

class ChunkLister : public IFFReader
{
	char idbuf[6];
	
	public:

	ChunkLister(const char *filepath) : IFFReader(filepath) {}
	virtual bool Parse();
};
