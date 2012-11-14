all: memusage memalloc

memusage: memusage.o
	gcc -Wall -o memusage memusage.C

memalloc: memalloc.o
	gcc -Wall -o memalloc memalloc.C

%.o: %.C %.h
	gcc -Wall -c $<

clean:
	rm -rf memalloc memusage memalloc.o memusage.o
