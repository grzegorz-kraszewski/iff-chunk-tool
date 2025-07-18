#include "locale.h"

#include <proto/exec.h>

const char* const BuiltInStrings[MSG_COUNT] = {
	"Can't open iffparse.library v39+",
	"Can't open utility.library v39+",
	"Program arguments error",
	"Out of memory",
	"'%s' is not a valid IFF chunk identifier.\n",
	"Chunk '%s' not found.\n",
	"Specify either string or file as chunk contents, not both",
	"Specify string or file as chunk contents",
	"Can't rename temporary file to source name",
	"Can't delete source file",
	"Unknown operation mode '%s'.\n",
	"Broken escape sequence in CONTENT string.\n",
	"'DATA' argument required for this operation mode.\n",
	"'CHUNK' argument required for this operation mode.\n",
	"IFF file is not FORM",
	"IFF type is required for this operation mode.\n",
	"\"%s\": unexpected end of file.\n",
	"unexpected end of file",
	"unexpected end of IFF chunk",
	"IFF property without scope",
	"out of memory",
	"file read error",
	"file write error",
	"file seek error",
	"file data corrupt",
	"IFF syntax error",
	"not an IFF file",
	"\"%s\": unknown IFF error %ld.\n",
	"'%s' is not a valid IFF type identifier.\n"
};

//=============================================================================
// AppLocale::AppLocale
//=============================================================================

AppLocale::AppLocale(const char *catalogname) : catalog(NULL)
{
	CopyMemQuick(BuiltInStrings, strings, sizeof(strings));

	if (LocaleBase = OpenLibrary("locale.library", 39))
	{
		catalog = OpenCatalogA(NULL, catalogname, NULL);

		for (int32 i = MSG_COUNT - 1; i >= 0; i--)
		{
			strings[i] = GetCatalogStr(catalog, i, strings[i]);
		}

		CloseCatalog(catalog);
	}
}

//=============================================================================
// AppLocale::~AppLocale
//=============================================================================

AppLocale::~AppLocale()
{
	if (LocaleBase) CloseLibrary(LocaleBase);
}
