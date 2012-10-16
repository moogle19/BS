CC 	= gcc
CFLAGS	= -Wall
LDFLAGS = -lssl -lcrypto

OBJ = fcp.o
BIN = fcp

fcp: $(OBJ)
	$(CC) $(CFLAGS) -o fcp $(OBJ) $(LDFLAGS)
	rm $(OBJ)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(BIN) $(OBJ)
