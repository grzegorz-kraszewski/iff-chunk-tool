# iff-chunk-tool
Commandline tool for manipulating chunks in IFF files. The tool is intended to be used for automated processing of IFF files in scripts, makefiles etc.

## Features
* Five modes of operation: insert, append, remove, replace, extract.
* Contents of inserted chunk may be specified as a string, or loaded from a file.
* Chunk may be inserted as the first, the last or after chunk specified as argument.

## Usage
Chunk insertion

`IFFChunkTool <source> INSERT <chunk_ID> [AFTER <chunk_ID>] [<text_contents> | DATAFILE <contents>] [TO <destination>]`

Chunk appending

`IFFChunkTool <source> APPEND <chunk_ID> [<text_contents> | DATAFILE <contents>] [TO <destination>]`

Chunk removal

`IFFChunkTool <source> REMOVE <chunk_ID> [TO <destination>]`

Chunk replacement

`IFFChunkTool <source> REPLACE <chunk_ID> [<text_contents> | DATAFILE <contents>] [TO <destination>]`

Chunk contents extraction

`IFFChunkTool <source> EXTRACT <chunk_ID> DATAFILE <contents>`
