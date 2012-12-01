noprio: readwritelock readwritelocktest

prio: readwritelockprio readwritelocktest

readwritelock: readwritelock.o
	gcc -Wall -o readwritelock readwritelock.c -lpthread

readwritelocktest: readwritelocktest.o
	gcc -Wall -o readwritelocktest readwritelocktest.c

readwritelockprio: readwritelockprio.o
	gcc -Wall -o readwritelockrio readwritelockprio.c

%.o: %.c %.h
	gcc -Wall -c $<

clean:
	rm -rf *.o readwritelock readwritelocktest readwritelockprio
