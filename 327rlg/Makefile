
CC =gcc
ECHO = echo
RM = rm -r

CFLAGS = -Werror -Wall -ggdb
BIN = ./bin/
SOURCES = $(addprefix $(BIN), main.o map.o gameio.o path_finder.o que.o heap.o)
EXE = $(BIN)dungeonGenerator

default: create_bin $(EXE)

$(EXE): $(SOURCES)
	@$(ECHO) compiling $(EXE)
	@$(CC) -o $(EXE) $(CFLAGS) $(SOURCES)

$(BIN)%.o: %.c %.h
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean create_bin default

clean:
	@$(ECHO) removing all binaries
	@$(RM) $(BIN)

create_bin:
	@[ -d $(BIN) ] || mkdir $(BIN)
