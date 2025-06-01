//==============
// IFFChunkTool
//==============

#include "iffwriter.h"

//=============================================================================
// IFFWriter::OpenFile()
//=============================================================================

bool IFFWriter::OpenFile(const char *filepath, uint32 formType)
{
	if (IFFFile::OpenFile(filepath, MODE_NEWFILE))
	{
		int32 iffError;

		iffError = PushChunk(iff, formType, ID_FORM, IFFSIZE_UNKNOWN);

		if (iffError == 0)
		{
			formPushed = TRUE;
			return TRUE;
		}
		else return IFFProblem(iffError);
	}
	else return FALSE;
}

//=============================================================================
// IFFWriter::~IFFWriter()
//=============================================================================

IFFWriter::~IFFWriter()
{
	int32 iffError;

	if (formPushed)
	{
		iffError = PopChunk(iff);
		if (iffError < 0) IFFProblem(iffError);
	}
}
