noprio: readwritelocktest

prio: readwritelockpriotest

readwritelock: readwritelock.o
	gcc -Wall -o readwritelock readwritelock.c -lpthread

readwritelocktest: readwritelock.o readwritelocktest.o
	gcc -Wall -o readwritelocktest readwritelocktest.c readwritelock.c -lpthread

readwritelockprio: readwritelockprio.o
	gcc -Wall -o readwritelockprio readwritelockprio.c -lpthread

readwritelockpriotest: readwritelockprio.o readwritelocktest.o
	gcc -Wall -o readwritelocktestprio readwritelocktest.c readwritelockprio.c -lpthread

%.o: %.c %.h
	gcc -Wall -c $< -lpthread

clean:
	rm -rf *.o readwritelock readwritelocktest readwritelocktestprio readwritelockprio priosched	
