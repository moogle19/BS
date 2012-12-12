noprio: readwritelock readwritelocktest

prio: readwritelockprio readwritelocktest

readwritelock: readwritelock.o
	gcc -Wall -o readwritelock readwritelock.c -lpthread

readwritelocktest: readwritelocktest.o
	gcc -Wall -o readwritelocktest readwritelocktest.c

readwritelockprio: readwritelockprio.o
	gcc -Wall -o readwritelockrio readwritelockprio.c

priosched: priosched.o
	gcc -Wall -o priosched priosched.c

%.o: %.c %.h
	gcc -Wall -c $< -lpthread

clean:
	rm -rf *.o readwritelock readwritelocktest readwritelockprio priosched	
