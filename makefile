CC = g++
LD = g++
CFLAGS = -nostdlib -O2 -fbaserel -fomit-frame-pointer -mregparm -fno-exceptions -fno-rtti -D__NOLIBBASE__
LDFLAGS = -nostdlib -fbaserel -fomit-frame-pointer -nostartfiles
LIBS =
OBJS = start.o main.o
EXE = IFFChunkTool

all: $(OBJS)
	@echo "Linking $(EXE)..."
	@$(LD) $(LDFLAGS) -o $(EXE).db $^ $(LIBS)
	@strip $(EXE).db -o $(EXE) --strip-unneeded
	@Protect $(EXE) +E
	List $(EXE)

dep:
	$(CC) -MM $(OBJS)

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

main.o: main.cpp
start.o: start.cpp
