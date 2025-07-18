#include "sysfile.h"
#include "locale.h"


//=============================================================================
// SysFile::SfProblem()
//=============================================================================

bool SysFile::SfProblem()
{
	static char faultBuffer[128];
	char *description;
	LONG error = IoErr();

	if (error)
	{
		Fault(error, "", faultBuffer, 128);
		Printf("\"%s\": %s.\n", filename, &faultBuffer[2]);
	}
	else Printf(Ls[MSG_UNEXPECTED_END_OF_FILE], filename);

	return FALSE;
}

//=============================================================================
// SysFile::SysFile()
//=============================================================================

SysFile::SysFile(const char *path, int32 mode)
{
	filename = path;
	ready = FALSE;

	if (handle = Open(filename, mode))
	{
		ready = TRUE;
	}
	else SfProblem();
}

//=============================================================================
// SysFile::~SysFile()
//=============================================================================

SysFile::~SysFile()
{
	if (handle)
	{
		if (!Close(handle)) SfProblem();
	}
}

//=============================================================================
// SysFile::SfSize()
//=============================================================================

int32 SysFile::SfSize()
{
	int32 result;

	if (Seek(handle, 0, OFFSET_END) < 0) { SfProblem(); return 0; }
	result = Seek(handle, 0, OFFSET_BEGINNING);
	if (result < 0) { SfProblem(); return 0; }
	return result;
}