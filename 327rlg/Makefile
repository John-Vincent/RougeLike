CXX = g++
CC =gcc
ECHO = echo
RM = rm -vr

CXXFLAGS = -Werror -Wall -ggdb
CFLAGS = -Werror -Wall -ggdb
BIN = ./bin/
OBJS = ./headers/
SRCS = ./src/


CBINARIES = $(addprefix $(BIN), main map gameio path_finder que heap des dungeon)
CBINARIES := $(addsuffix .o, $(CBINARIES))

CXXBINARIES = $(addprefix $(BIN), character npc pc)
CXXBINARIES := $(addsuffix .cpp.o, $(CXXBINARIES))

BINARIES = $(CBINARIES) $(CXXBINARIES)

EXE = $(BIN)RougeLikeGame

default: create_bin $(EXE)

$(EXE): $(CBINARIES) $(CXXBINARIES)
	@$(ECHO) compiling $(EXE)
	@$(CXX) -o $(EXE) $(CXXFLAGS) $(BINARIES) -lncurses

$(BIN)%.cpp.o: $(SRCS)%.cpp
	@$(ECHO) compiling $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN)%.o: $(SRCS)%.c $(OBJS)%.h
	@$(ECHO) compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean create_bin default

clean:
	@[ ! -d $(BIN) ] || $(ECHO) removing all binaries
	@[ ! -d $(BIN) ] || $(RM) $(BIN)

create_bin:
	@[ -d $(BIN) ] || mkdir $(BIN)
