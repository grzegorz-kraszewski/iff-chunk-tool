#include "chunkpicker.h"


ChunkPicker::ChunkPicker(const char *filename, uint32 chid) : ready(FALSE), chunkId(chid), input(filename)
{
	if (input.ready)
	{
	}
}

bool ChunkPicker::Parse(const char *filename)
{
	if (input.OpenFile(filename))
	{
	}
}
