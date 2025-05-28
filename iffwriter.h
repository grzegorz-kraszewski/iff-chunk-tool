//==============
// IFFChunkTool
//==============

#include "ifffile.h"

#include <dos/dos.h>

class IFFWriter : public IFFFile
{
	public:

	bool OpenFile(const char *filepath)
	{
		return IFFFile::OpenFile(filepath, MODE_NEWFILE);
	}

};
