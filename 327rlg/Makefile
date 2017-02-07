
default: main.o room.o
	gcc -o dungeonGenerator main.o room.o

main.o: main.c room.h
	gcc -c main.c -Werror -Wall -ggdb

room.o: room.c room.h
	gcc -c room.c -Werror -Wall -ggdb

clean:
	rm main.o dungeonGenerator room.o
