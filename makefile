CC = gcc
CPP = g++
LD = g++
CFLAGS = -nostdlib -O2 -fbaserel -fomit-frame-pointer -mregparm -fno-exceptions -fno-rtti -D__NOLIBBASE__
# CFLAGS += -DDEBUG
LDFLAGS = -nostdlib -fbaserel -fomit-frame-pointer -nostartfiles
LIBS =
OBJS = start.o main.o callargs.o application.o ifffile.o iffreader.o iffwriter.o chunklister.o chunkpicker.o chunkextractor.o chunkdumper.o chunkcopier.o chunkremover.o chunkadder.o chunkreplacer.o chunkinjector.o sysfile.o pathbuilder.o locale.o
EXE = IFFChunkTool

.PHONY: pure dep clean

all: $(OBJS) purevirtual.o
	@echo "Linking $(EXE)..."
	@$(LD) $(LDFLAGS) -o $(EXE).db $^ $(LIBS)
	@strip $(EXE).db -o $(EXE) --strip-unneeded
	@Protect $(EXE) +E
	@List $(EXE) LFORMAT "%N %L"

dep:
	@$(CPP) -MM $(OBJS:.o=.cpp)

clean:
	rm -vf *.o $(EXE) $(EXE).db

start.o: start.cpp
	@echo "Compiling $@..."
	@$(CPP) $(CFLAGS) -fwritable-strings -c -o $@ $<

purevirtual.o: purevirtual.c
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

# dependencies

start.o: start.cpp
main.o: main.cpp main.h locale.h application.h callargs.h chunkcopier.h iffreader.h ifffile.h iffwriter.h sysfile.h pathbuilder.h
callargs.o: callargs.cpp main.h locale.h callargs.h
application.o: application.cpp main.h locale.h application.h callargs.h chunkcopier.h iffreader.h ifffile.h iffwriter.h sysfile.h pathbuilder.h chunklister.h chunkextractor.h chunkpicker.h chunkdumper.h chunkremover.h chunkadder.h chunkreplacer.h chunkinjector.h
ifffile.o: ifffile.cpp main.h ifffile.h
iffreader.o: iffreader.cpp locale.h main.h iffreader.h ifffile.h
iffwriter.o: iffwriter.cpp iffwriter.h ifffile.h main.h
chunklister.o: chunklister.cpp chunklister.h iffreader.h ifffile.h main.h
chunkpicker.o: chunkpicker.cpp locale.h main.h chunkpicker.h iffreader.h ifffile.h
chunkextractor.o: chunkextractor.cpp locale.h main.h chunkextractor.h chunkpicker.h iffreader.h ifffile.h
chunkdumper.o: chunkdumper.cpp chunkdumper.h main.h chunkpicker.h iffreader.h ifffile.h
chunkcopier.o: chunkcopier.cpp locale.h main.h chunkcopier.h iffreader.h ifffile.h iffwriter.h sysfile.h
chunkremover.o: chunkremover.cpp chunkremover.h chunkcopier.h main.h iffreader.h ifffile.h iffwriter.h sysfile.h
chunkadder.o: chunkadder.cpp chunkadder.h chunkcopier.h main.h iffreader.h ifffile.h iffwriter.h sysfile.h
chunkreplacer.o: chunkreplacer.cpp chunkreplacer.h chunkcopier.h main.h iffreader.h ifffile.h iffwriter.h sysfile.h locale.h
chunkinjector.o: chunkinjector.cpp chunkinjector.h chunkcopier.h main.h iffreader.h ifffile.h iffwriter.h sysfile.h locale.h
sysfile.o: sysfile.cpp sysfile.h main.h locale.h
pathbuilder.o: pathbuilder.cpp pathbuilder.h main.h
locale.o: locale.cpp locale.h main.h

