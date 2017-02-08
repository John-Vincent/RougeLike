
default: main.o map.o gameio.o path_finder.o que.o heap.o
	gcc -o dungeonGenerator main.o map.o gameio.o path_finder.o que.o heap.o 

main.o: main.c map.h main.h gameio.h
	gcc -c main.c -Werror -Wall -ggdb

map.o: map.c map.h path_finder.h
	gcc -c map.c -Werror -Wall -ggdb

gameio.o: gameio.c gameio.h
	gcc -c gameio.c -Werror -Wall -ggdb

path_finder.o: path_finder.h heap.h path_finder.c
	gcc -c path_finder.c -Werror -Wall -ggdb

que.o: que.c que.h
	gcc -c que.c -Werror -Wall -ggdb

heap.o: heap.h heap.c
	gcc -c heap.c -Werror -Wall -ggdb

clean:
	rm -v dungeonGenerator *.o
