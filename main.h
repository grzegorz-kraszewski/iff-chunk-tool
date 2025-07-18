//=============================================================================================
// Hierarchy of classes in the program.
// 
// CallArgs (handles commandline arguments)
//
// Application (processing mode checker and dispatcher)
//
// IFFFile
//   IFFReader
//     ChunkLister       (handles LIST)
//     ChunkPicker       (base class for single chunk processor)
//       ChunkExtractor  (handles EXTRACT)
//       ChunkDumper     (handles DUMP)
//     ChunkCopier       (base class for IFF->IFF operations)
//       ChunkRemover    (handles REMOVE)
//       ChunkReplacer   (handles REPLACE)
//       ChunkInjector   (handles INSERT)
//       ChunkAdder      (handles APPEND)
//   IFFWriter  (ChunkCopier instances have it as member)
//=============================================================================================

#ifndef IFFCHUNKTOOL_MAIN_H
#define IFFCHUNKTOOL_MAIN_H

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
	*IFFParseBase,
	*UtilityBase;

//=======
// debug
//=======

#ifdef DEBUG
#define DC(a) Printf("%s $%08lx.\n", a, this)
#define DD(a) Printf("~%s $%08lx.\n", a, this)
#else
#define DC(a)
#define DD(a)
#endif

//================
// some constants
//================

#define COPYBUF_SIZE 32768    // memory allocation when copying chunks or writing them to a file

//==================
// helper functions
//==================

int32 StrLen(const char *s);
uint32 ValidateChunkID(const char *s);
uint32 ValidateTypeID(const char *s);
void FmtPut(char *dest, const char *fmt, int32 arg1, ...);

//=================
// error reporting
//=================

int32 Problem(const char *msg);
int32 SysProblem(const char *msg);

//===============
// locale stuff
//===============

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

#endif    /* IFFCHUNKTOOL_MAIN_H */
