CC 	= gcc
CFLAGS	= -Wall
LDFLAGS = -lssl -lcrypto

OBJ = memusage.o
BIN = memusage.C

memusage: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(BIN) $(OBJ)
