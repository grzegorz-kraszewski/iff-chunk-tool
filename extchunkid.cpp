#include "extchunkid.h"

#include <proto/iffparse.h>
#include <proto/dos.h>

static bool StrToUInt32(const char *str, uint32 *value)
{
	char c;
	bool success = TRUE;
	uint32 x = 0;

	while (success && (c = *str++))
	{
		c -= '0';

		if ((c >= 0) && (c <= 9))
		{
			if ((x < 429496729) || ((x == 429496729) && (c < 6)))
			{
				x *= 10;
				x += c;
			}
			else success = FALSE;
		}
		else success = FALSE;
	}

	if (success) *value = x;
	return success;
}

//=============================================================================
// ExtendedChunkID::ExtendedChunkID()
//=============================================================================

ExtendedChunkID::ExtendedChunkID(const char *id) :
	chunkId(0),
 	number(0)
{
	int32 length;
	const char *numpos = NULL;

	if (!id) return;
	length = StrLen(id);

	if (length >= 4)
	{
		chunkId = *(uint32*)id;

		if (GoodID(chunkId))
		{
			if ((length > 5) && (id[4] == '/'))
			{
				if (!StrToUInt32(&id[5], &number)) chunkId = 0;
			}
			else chunkId = 0;
		}
		else chunkId = 0;
	}

	if (!chunkId) Printf("'%s' is not a valid chunk identifier.\n", id);
}
