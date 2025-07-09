//=============================================================================================
// IFFReader class represents IFF file opened for reading. It also starts parsing by entering
// 'FORM' chunk and reading type of it. Non-zero 'iffType' means succesfull opening. Note that
// other valid kinds of IFF files ('CAT ' and 'LIST') are rejected. If an object is succesfully
// created, it is positioned at start of FORM. IFFReaded is an abstract class. Subclasses must
// implement the Parse() method.
//=============================================================================================

#include "ifffile.h"

#include <dos/dos.h>

class IFFReader : public IFFFile
{
	public:

	IFFReader(const char *filepath);
	virtual bool Parse() = 0;
	uint32 iffType;
};
