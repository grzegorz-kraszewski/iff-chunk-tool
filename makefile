CC = g++
LD = g++
CFLAGS = -nostdlib -O2 -fbaserel -fomit-frame-pointer -mregparm -fno-exceptions -fno-rtti -D__NOLIBBASE__
LDFLAGS = -nostdlib -fbaserel -fomit-frame-pointer -nostartfiles
LIBS =
OBJS = start.o main.o callargs.o application.o ifffile.o iffreader.o
EXE = IFFChunkTool

all: $(OBJS)
	@echo "Linking $(EXE)..."
	@$(LD) $(LDFLAGS) -o $(EXE).db $^ $(LIBS)
	@strip $(EXE).db -o $(EXE) --strip-unneeded
	@Protect $(EXE) +E
	@List $(EXE) LFORMAT "%N %L"

dep:
	$(CC) -MM $(OBJS:.o=.cpp)

clean:
	rm -vf *.o $(EXE) $(EXE).db

start.o: start.cpp
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -fwritable-strings -c -o $@ $<

# purevirtual.o: purevirtual.c
# 	@echo "Compiling $@..."
# 	@gcc $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

# dependencies

start.o: start.cpp
main.o: main.cpp main.h application.h iffreader.h ifffile.h iffwriter.h callargs.h
callargs.o: callargs.cpp main.h callargs.h
application.o: application.cpp application.h iffreader.h ifffile.h main.h iffwriter.h callargs.h
ifffile.o: ifffile.cpp main.h ifffile.h
iffreader.o: iffreader.cpp iffreader.h ifffile.h main.h
