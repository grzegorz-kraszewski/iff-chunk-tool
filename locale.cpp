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
// MSG_IFFERR_UNEXPECTED_EOF              19
// MSG_IFFERR_UNEXPECTED_EOC              20
// MSG_IFFERR_NO_PROPERTY_SCOPE           21
// MSG_IFFERR_OUT_OF_MEMORY               22
// MSG_IFFERR_FILE_READ                   23
// MSG_IFFERR_FILE_WRITE                  24
// MSG_IFFERR_FILE_SEEK                   25
// MSG_IFFERR_DATA_CORRUPTED              26
// MSG_IFFERR_SYNTAX                      27
// MSG_IFFERR_NOT_IFF_FILE                28
// MSG_IFFERR_UNKNOWN                     29
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
