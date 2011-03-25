CC=gcc
CFLAGS=-c -Wall
RM=rm -rf


EXECUTABLE=attrbench


SOURCES=src/main.c
OBJECTS=$(SOURCES:.cpp=.o)


all: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@


.c.o:
	$(CC) $(CFLAGS) $< -o $@
	

clean:
	$(RM) *.o attrbench src/*.o

