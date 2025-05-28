//==============
// IFFChunkTool
//==============

#include "ifffile.h"

#include <dos/dos.h>

class IFFReader : public IFFFile
{
	public:

	uint32 iffType;
	bool OpenFile(const char *filepath);
};
