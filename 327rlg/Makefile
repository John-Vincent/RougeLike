
CC =gcc
ECHO = echo
RM = rm -vr


CFLAGS = -Werror -Wall -ggdb
BIN = ./bin/
OBJS = ./headers/
SRCS = ./src/
BINARIES = $(addprefix $(BIN), main map gameio path_finder que heap character)
BINARIES := $(addsuffix .o, $(BINARIES))
EXE = $(BIN)RougeLikeGame

default: create_bin $(EXE)

$(EXE): $(BINARIES)
	@$(ECHO) compiling $(EXE)
	@$(CC) -o $(EXE) $(CFLAGS) $(BINARIES)

$(BIN)%.o: $(SRCS)%.c $(OBJS)%.h
	@$(ECHO) compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean create_bin default

clean:
	@[ ! -d $(BIN) ] || $(ECHO) removing all binaries
	@[ ! -d $(BIN) ] || $(RM) $(BIN)

create_bin:
	@[ -d $(BIN) ] || mkdir $(BIN)
