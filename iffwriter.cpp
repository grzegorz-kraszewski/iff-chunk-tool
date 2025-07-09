//==============
// IFFChunkTool
//==============

#include "iffwriter.h"

//=============================================================================
// IFFWriter::OpenFile()
//=============================================================================

IFFWriter::IFFWriter(const char *filepath, uint32 formType) : IFFFile(filepath, MODE_NEWFILE), 
formPushed(FALSE), ready(FALSE)
{
	if (opened)
	{
		int32 iffError;

		iffError = PushChunk(iff, formType, ID_FORM, IFFSIZE_UNKNOWN);

		if (iffError == 0)
		{
			formPushed = TRUE;
			ready = TRUE;
		}
		else IFFProblem(iffError);
	}
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
