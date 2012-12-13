all: priosched

priosched: priosched.o
	gcc -Wall -o priosched priosched.c

%.o: %.c %.h
	gcc -Wall -c $< -lpthread

clean:
	rm -rf *.o priosched	
