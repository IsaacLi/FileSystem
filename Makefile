IDIR = .
CC = g++
CFLAGS = -Wall -g -I $(IDIR)

SIR = .

ODIR = .
LDIR = .

LIBS = 

_DEPS = fakedisc.h FBQ.h File.h FileSys.h UserInterface.h Dir.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = fakedisc.o FBQ.o File.o FileSys.o UserInterface.o Dir.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: $(SIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

FileSystem: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
