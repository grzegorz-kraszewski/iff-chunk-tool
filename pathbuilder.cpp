#include "pathbuilder.h"

#include <proto/exec.h>

//=============================================================================
// PathBuilder::~PathBuilder()
//=============================================================================

PathBuilder::~PathBuilder()
{
	if (path) delete path;
}

//=============================================================================
// PathBuilder::Extend()
//=============================================================================

bool PathBuilder::Extend()
{
	char *extended;
	int32 oldSize;

	oldSize = currentSize;
	currentSize += PBUF_INCREMENT;
	extended = new char[currentSize];

	if (extended)
	{
		CopyMemQuick(path, extended, oldSize);
		delete path;
		path = extended;
		return TRUE;
	}

	return FALSE;
}

//=============================================================================
// PathBuilder::Path()
//=============================================================================

char* PathBuilder::Path()
{
	if (path && ready) return path;
	else return NULL;
}

//=============================================================================
// PathBuilder::AddPathPart()
//=============================================================================

void PathBuilder::AddPathPart(const char *part)
{
	if (currentSize == 0) ready = Extend();
	
	while (ready)
	{
		if (AddPart(path, part, currentSize)) return;
		ready = Extend();
	}
}

//=============================================================================
// 
//=============================================================================
