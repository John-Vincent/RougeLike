
default: main.o map.o gameio.o
	gcc -o dungeonGenerator main.o map.o gameio.o

main.o: main.c map.h main.h gameio.h
	gcc -c main.c -Werror -Wall -ggdb

map.o: map.c map.h
	gcc -c map.c -Werror -Wall -ggdb

gameio.o: gameio.c gameio.h
	gcc -c gameio.c -Werror -Wall -ggdb

clean:
	rm -v dungeonGenerator *.o
