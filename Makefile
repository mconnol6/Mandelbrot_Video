all: mandel mandelmovie

mandel: mandel.o bitmap.o
	gcc mandel.o bitmap.o -o mandel -lpthread

mandelmovie: mandelmovie.o
	gcc mandelmovie.o -o mandelmovie -lm

mandelmovie.o: mandelmovie.c
	gcc -Wall -g -c mandelmovie.c -o mandelmovie.o

mandel.o: mandel.c
	gcc -Wall -g -c mandel.c -o mandel.o

bitmap.o: bitmap.c
	gcc -Wall -g -c bitmap.c -o bitmap.o

clean:
	rm *.o mandel mandelmovie
