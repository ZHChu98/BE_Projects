all: express

clean:
	rm *.o express

express: main.o
	cc main.o -lpthread -o express

main.o: main.c
	cc -lpthread -c main.c