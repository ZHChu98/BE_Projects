all: copy

clean:
	rm *.o Copy

copy: MyCopy.o
	cc MyCopy.o -o copy

MyCopy.o: MyCopy.c
	cc -c MyCopy.c
