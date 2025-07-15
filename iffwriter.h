//==============
// IFFChunkTool
//==============

#ifndef IFFCHUNKTOOL_IFFWRITER_H
#define IFFCHUNKTOOL_IFFWRITER_H

#include "ifffile.h"

#include <dos/dos.h>

class IFFWriter : public IFFFile
{
	bool formPushed;

	public:

	bool ready;
	IFFWriter(const char *filepath, uint32 type);
	virtual ~IFFWriter();
};

#endif    // IFFCHUNKTOOL_IFFWRITER_H
