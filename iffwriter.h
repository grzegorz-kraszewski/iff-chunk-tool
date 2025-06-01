//==============
// IFFChunkTool
//==============

#include "ifffile.h"

#include <dos/dos.h>

class IFFWriter : public IFFFile
{
	bool formPushed;

	public:

	IFFWriter() : formPushed(FALSE) {}
	bool OpenFile(const char *filepath, uint32 formType);
	~IFFWriter();
};
