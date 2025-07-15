#include "chunkdumper.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/iffparse.h>

//=============================================================================================
// ChunkWork() for ChunkDumper hexdumps current chunk contents to the standard output. Current
// version reads chunk in 16-byte blocks (one line of dump), which may be slow. Some buffering
// may be added later.
//=============================================================================================

bool ChunkDumper::ChunkWork(ContextNode *cn)
{
	bool success = TRUE;
	int32 bytes, offset = 0;
	uint8 buf[16];
	char line[64];

	while (success)
	{
		bytes = ReadChunkBytes(iff, buf, 16);

		if (bytes > 0)
		{
			int16 p = 11;

			//---------------------
			// offset in the chunk
			//---------------------

			FmtPut(line, "%9ld: ", offset);

			//------------------
			// hexadecimal dump
			//------------------

			for (int16 i = 0; i < bytes; i++)
			{
				FmtPut(&line[p], "%02lX", buf[i]);
				p += 2;
				if ((i & 3) == 3) line[p++] = ' ';
			}

			//-------------------------------
			// spaces to the characters area
			//-------------------------------

			while (p < 47) line[p++] = ' ';

			//------------
			// characters
			//------------

			for (int16 i = 0; i < bytes; i++)
			{
				uint8 k = buf[i];

				if (k < 32) k = '.';
				else if ((k >= 128) && (k < 160)) k = '.';
				line[p++] = k;
			}

			line[p++] = 0x0A;
			WriteChars(line, p);
			offset += bytes;
		}
		else if (bytes == 0) break;         // end of chunk
		else success = IFFProblem(bytes);
	}

	return success;
}
