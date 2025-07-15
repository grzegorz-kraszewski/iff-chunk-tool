#ifndef IFFCHUNKTOOL_SYSFILE_H
#define IFFCHUNKTOOL_SYSFILE_H

#include "main.h"

#include <proto/dos.h>

class SysFile
{
	BPTR handle;
	const char *filename;

	public:

	bool ready;
	SysFile(const char *path, int32 mode);
	~SysFile();
	int32 SfRead(void *buffer, int32 bytes) { return Read(handle, buffer, bytes); }
	//int32 SfWrite(APTR buffer, LONG bytes);
	//int32 SfSeek(LONG offset, LONG mode);
	int32 SfSize();
	bool SfProblem();
};

#endif    // IFFCHUNKTOOL_SYSFILE_H
