#include "chunklister.h"
#include "locale.h"

#include <proto/dos.h>

//=============================================================================
// ChunkListEntry::PrintAsForm()
//=============================================================================

void ChunkListEntry::PrintAsForm()
{
	static char idbuf[5];

	Printf("FORM %s %ld\n", IDtoStr(extendedId.chunkId, idbuf), chunkLength);
}

//=============================================================================
// ChunkListEntry::Print()
//=============================================================================

void ChunkListEntry::Print()
{
	static char idbuf[5];

	IDtoStr(extendedId.chunkId, idbuf);

	if (showNumber)
	{
		Printf("%s.%ld %ld\n", idbuf, extendedId.number, chunkLength);
	}
	else
	{
		Printf("%s %ld\n", idbuf, chunkLength);
	}
}

//-----------------------------------------------------------------------------
// Note: ChunkListEntry objects dynamically allocated here, are not deallocated
// explicitly, because the default operator new() uses a memory pool, which is
// deleted at exit.
//-----------------------------------------------------------------------------

//=============================================================================
// ChunkLister::ScanChunks()
//=============================================================================

bool ChunkLister::ScanChunks()
{
	ContextNode *cn;
	int32 iffError;
	bool success = TRUE;
	ChunkListEntry *cle;

	cn = CurrentChunk(iff);

	//--------------------------------------------------------------------
	// The first entry in the list has implicit id 'FORM' and IFF type is
	// stored as chunk ID instead.
	//--------------------------------------------------------------------

	if (cle = new ChunkListEntry(cn->cn_Type, cn->cn_Size))
	{
		chunks.addTail(cle);

		while (success)
		{
			iffError = ParseIFF(iff, IFFPARSE_STEP);
			cn = CurrentChunk(iff);

			if (iffError == 0)
			{
				if (cle = new ChunkListEntry(cn->cn_ID, cn->cn_Size))
				{
					chunks.addTail(cle);
				}
				else success = Problem(Ls[MSG_OUT_OF_MEMORY]); 
			}
			else if (iffError == IFFERR_EOC)
			{
				if (cn->cn_ID == ID_FORM) break;
			}
			else success = IFFProblem(iffError);
		}
	}
	else success = Problem(Ls[MSG_OUT_OF_MEMORY]);

	return success;
}

//=============================================================================
// ChunkLister::SetChunkNumbers()
//-----------------------------------------------------------------------------
// The method processes the list of chunks. For multiple chunks with the same 
// ID it numbers these chunks and sets their 'showNumber' to TRUE.
//=============================================================================

void ChunkLister::SetChunkNumbers()
{

}

//=============================================================================
// ChunkLister::ListChunks()
//=============================================================================

void ChunkLister::ListChunks()
{
	ChunkListEntry *cle;
	bool form = TRUE;

	for (cle = chunks.first(); cle; cle = cle->next())
	{
		if (form)
		{
			cle->PrintAsForm();
			form = FALSE;
		}
		else cle->Print();
	}
}

//=============================================================================
// ChunkLister::Parse()
//=============================================================================

bool ChunkLister::Parse()
{
	bool success;

	if (success = ScanChunks())
	{
		SetChunkNumbers();
		ListChunks();
	}

	return success;
}
