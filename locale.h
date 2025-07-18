//===============
// locale stuff
//===============

#include "main.h"

#include <proto/locale.h>

extern Library *LocaleBase;
extern Catalog *Cat;

static inline const char* LS(int32 id, const char* defstr)
{
	return (Cat ? GetCatalogStr(Cat, id, defstr) : defstr);
}

#define MSG_NO_IFFPARSE_LIBRARY                 0
#define MSG_NO_UTILITY_LIBRARY                  1
#define MSG_COMMANDLINE_ARGS                    2
#define MSG_OUT_OF_MEMORY                       3
#define MSG_ARGS_UNKNOWN_MODE                   4
#define MSG_INVALID_CHUNK_ID                    5
#define MSG_NO_OUTPUT_DATAFILE                  6
#define MSG_CHUNK_NOT_FOUND_IN_SOURCE           7
#define MSG_BOTH_STRING_AND_FILE_SPECIFIED      8
#define MSG_NO_CHUNK_CONTENT_SPECIFIED          9
#define MSG_ERR_TEMP_FILE_RENAME               10
#define MSG_DELETE_SOURCE_BEFORE_RENAME        11
#define MSG_UNKNOWN_OPERATION_MODE             12
#define MSG_BROKEN_ESCAPE_SEQUENCE             13
#define MSG_DATA_ARGUMENT_REQUIRED             14
#define MSG_CHUNK_ARGUMENT_REQUIRED            15
#define MSG_IFF_FILE_IS_NOT_FORM               16
#define MSG_IFF_TYPE_ARGUMENT_REQUIRED         17
#define MSG_UNEXPECTED_END_OF_FILE             18
#define MSG_IFFERR_UNEXPECTED_EOF              19
#define MSG_IFFERR_UNEXPECTED_EOC              20
#define MSG_IFFERR_NO_PROPERTY_SCOPE           21
#define MSG_IFFERR_OUT_OF_MEMORY               22
#define MSG_IFFERR_FILE_READ                   23
#define MSG_IFFERR_FILE_WRITE                  24
#define MSG_IFFERR_FILE_SEEK                   25
#define MSG_IFFERR_DATA_CORRUPTED              26
#define MSG_IFFERR_SYNTAX                      27
#define MSG_IFFERR_NOT_IFF_FILE                28
#define MSG_IFFERR_UNKNOWN                     29

#define MSG_COUNT                              30
