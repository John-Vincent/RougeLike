
CC =gcc
ECHO = echo
RM = rm -r


CFLAGS = -Werror -Wall -ggdb
BIN = ./bin/
OBJS = ./headers/
SRCS = ./src/
BINARIES = $(addprefix $(BIN), main.o map.o gameio.o path_finder.o que.o heap.o)
EXE = $(BIN)dungeonGenerator

default: create_bin $(EXE)

$(EXE): $(BINARIES)
	@$(ECHO) compiling $(EXE)
	@$(CC) -o $(EXE) $(CFLAGS) $(BINARIES)

$(BIN)%: $(SRCS)%.c $(OBJS)%.h
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean create_bin default

clean:
	@$(ECHO) removing all binaries
	@$(RM) $(BIN)

create_bin:
	@[ -d $(BIN) ] || mkdir $(BIN)
