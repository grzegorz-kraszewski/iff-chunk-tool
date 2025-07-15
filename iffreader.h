//=============================================================================================
// IFFReader class represents IFF file opened for reading. It also starts parsing by entering
// 'FORM' chunk and reading type of it. Note that other valid kinds of IFF files ('CAT ' and
// 'LIST') are rejected. If an object is succesfully created, it is positioned at start of
// FORM. IFFReaded is an abstract class. Subclasses must implement the Parse() method.
//=============================================================================================

#ifndef IFFCHUNKTOOL_IFFREADER_H
#define IFFCHUNKTOOL_IFFREADER_H

#include "ifffile.h"

#include <dos/dos.h>

class IFFReader : public IFFFile
{
	public:

	bool ready;
	uint32 iffType;
	IFFReader(const char *filepath);
	virtual bool Parse() = 0;
	virtual ~IFFReader();
};

#endif     // IFFCHUNKTOOL_IFFREADER_H
