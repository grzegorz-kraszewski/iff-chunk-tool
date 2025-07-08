//==============
// IFFChunkTool
//==============

#include "ifffile.h"

#include <dos/dos.h>

class IFFReader : public IFFFile
{
	public:

	IFFReader(const char *filepath);
	uint32 iffType;
};
