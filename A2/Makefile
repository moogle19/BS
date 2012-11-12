CC 	= gcc
CFLAGS	= -Wall
LDFLAGS = -lssl -lcrypto

OBJ = memusage.o
OBJ1 = memalloc.o
BIN = memusage
BIN1 = memalloc

all: memusage memalloc

memusage: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

memalloc: $(OBJ1)
	$(CC) $(CFLAGS) -o $(BIN1) $(OBJ1) $(LDFLAGS)

%.o: %.C %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(BIN) $(OBJ) $(BIN1) $(OBJ1)
