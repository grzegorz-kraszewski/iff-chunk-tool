# iff-chunk-tool

Commandline tool for manipulating chunks in IFF files. The tool is intended to be used for automated processing of IFF files in scripts, makefiles etc.

Program for AmigaOS 3.0+.

## Features

* Eight modes of operation: list, insert, append, remove, replace, extract, dump, new.
* Contents of inserted chunk may be specified as a string, or loaded from a file.
* Chunk may be inserted as the first, the last or after chunk specified as argument.

## Usage

Chunks listing

`IFFChunkTool <source> LIST`

Chunk insertion

`IFFChunkTool <source> INSERT <chunk_ID> [AFTER <chunk_ID>] [<text_contents> | DATA <contents>] [TO <destination>]`

Chunk appending

`IFFChunkTool <source> APPEND <chunk_ID> [<text_contents> | DATA <contents>] [TO <destination>]`

Chunk removal

`IFFChunkTool <source> REMOVE <chunk_ID> [TO <destination>]`

Chunk replacement

`IFFChunkTool <source> REPLACE <chunk_ID> [<text_contents> | DATA <contents>] [TO <destination>]`

Chunk contents extraction

`IFFChunkTool <source> EXTRACT <chunk_ID> DATA <destfile>`

Hexdump of a chunk

`IFFChunkTool <source> DUMP <chunk_ID>`

Create an empty IFF file of given type

`IFFChunkTool <filename> NEW <typeid>`
