noprio: readwritelocktest

prio: readwritelockprio readwritelocktest

readwritelock: readwritelock.o
	gcc -Wall -o readwritelock readwritelock.c -lpthread

readwritelocktest: readwritelock.o readwritelocktest.o
	gcc -Wall -o readwritelocktest readwritelocktest.c readwritelock.c -lpthread

readwritelockprio: readwritelockprio.o
	gcc -Wall -o readwritelockrio readwritelockprio.c

%.o: %.c %.h
	gcc -Wall -c $< -lpthread

clean:
	rm -rf *.o readwritelock readwritelocktest readwritelockprio priosched	
