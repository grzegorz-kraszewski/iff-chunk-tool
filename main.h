#include <exec/libraries.h>

#include <proto/locale.h>

//===============
// integer types
//===============

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed long int32;
typedef unsigned long uint32;

//======================
// global library bases
//======================

extern Library
	*SysBase,
	*DOSBase,
	*IFFParseBase;

//=================
// error reporting
//=================

int32 Problem(const char *msg);

//===============
// locale stuff
//===============

extern Library *LocaleBase;
extern Catalog *Cat;

static inline char* LS(int32 id, char* defstr)
{
	return (Cat ? GetCatalogStr(Cat, id, defstr) : defstr);
}

#define MSG_NO_IFFPARSE_LIBRARY                 0