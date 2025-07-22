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
void FmtPut(char *dest, const char *fmt, ...);

//=================
// error reporting
//=================

int32 Problem(const char *msg);
int32 SysProblem(const char *msg);

#endif    /* IFFCHUNKTOOL_MAIN_H */
