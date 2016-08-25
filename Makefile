CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=
LIBS=-lm
SOURCES=main.c turner.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=simpleocr

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

