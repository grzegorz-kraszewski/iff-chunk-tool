//==============
// IFFChunkTool
//==============

#include "callargs.h"
#include "chunkcopier.h"
#include "pathbuilder.h"

class Application
{
	CallArgs &arguments;
	PathBuilder builder;
	bool inPlace;
	const char* PrepareDestination();
	void TempFileToSource();
	bool CreateEmptyIFF(const char *filePath, const char *typeString);

	public:

	Application(CallArgs &args) : arguments(args), inPlace(FALSE) {}
	bool Process();
	ChunkDataSource* PrepareDataSource();
	~Application() {}
};
