//==============
// IFFChunkTool
//==============

#include "ifffile.h"

#include <dos/dos.h>

class IFFReader : public IFFFile
{
	public:

	bool OpenFile(const char *filepath);
};
