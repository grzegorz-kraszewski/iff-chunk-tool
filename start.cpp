/*-------------------------------------*/
/* Minimal C++ startup for AmigaOS 3.x */
/* RastPort, 2024                      */
/*-------------------------------------*/

#include <proto/exec.h>
#include <proto/dos.h>
#include <workbench/startup.h>

Library *SysBase;
Library *DOSBase;
APTR TaskPool = NULL;

extern ULONG Main(WBStartup *wbmsg);

#define HAVE_GLOBAL_CONSTRUCTORS

#ifdef HAVE_GLOBAL_CONSTRUCTORS
extern void (*__CTOR_LIST__[])(void);
extern void (*__DTOR_LIST__[])(void);
void GlobalConstructors();
void GlobalDestructors();
#endif

__saveds ULONG Start(void)
{
	Process *myproc = NULL;
	WBStartup *wbmsg = NULL;
	BOOL have_shell = FALSE;
	ULONG result = RETURN_OK;

	SysBase = *(Library**)4L;
	myproc = (Process*)FindTask(NULL);

	if (myproc->pr_CLI) have_shell = TRUE;

	if (!have_shell)
	{
		WaitPort(&myproc->pr_MsgPort);
		wbmsg = (WBStartup*)GetMsg(&myproc->pr_MsgPort);
	}

	result = RETURN_FAIL;

	if (DOSBase = OpenLibrary("dos.library", 39))
	{
		if (TaskPool = CreatePool(MEMF_ANY, 4096, 2048))
		{
			#ifdef HAVE_GLOBAL_CONSTRUCTORS
			GlobalConstructors();
			#endif
			result = Main(wbmsg);
			#ifdef HAVE_GLOBAL_CONSTRUCTORS
			GlobalDestructors();
			#endif
			DeletePool(TaskPool);
		}

		CloseLibrary(DOSBase);
	}

	if (wbmsg)
	{
		Forbid();
		ReplyMsg(&wbmsg->sm_Message);
	}

	return (ULONG)result;
}


__attribute__((section(".text"))) UBYTE VString[] = "$VER: IFFChunkTool 1.0 (18.07.2025)\r\n";



APTR operator new(ULONG size) throw()
{
	ULONG *m;

	size += 4;

	if (m = (ULONG*)AllocPooled(TaskPool, size))
	{
		*m = size;
		return m + 1;
	}
	else return NULL;
}


APTR operator new[](ULONG size)
{
	return operator new(size);
}


void operator delete(APTR memory)
{
	ULONG *m = (ULONG*)memory - 1;

	FreePooled(TaskPool, m, *m);
}


void operator delete[](APTR memory)
{
	operator delete(memory);
}

#ifdef HAVE_GLOBAL_CONSTRUCTORS
void GlobalConstructors()
{
	for (long i = (long)__CTOR_LIST__[0]; i > 0; i--) __CTOR_LIST__[i](); 
}


void GlobalDestructors()
{
	void (**dtor)(void) = __DTOR_LIST__;

	while (*(++dtor)) (*dtor)();
}
#endif
