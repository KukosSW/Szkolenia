# Prosty Makefile, w takim projekcie duzy makefile to overkill

CC := gcc
CCXX := g++

CCWARNINGS := 	-Wall -Wextra -pedantic -Wcast-align \
				-Winit-self -Wlogical-op -Wmissing-include-dirs \
			 	-Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef  \
				-Wwrite-strings -Wpointer-arith -Wmissing-declarations \
				-Wuninitialized -Wold-style-definition -Wstrict-prototypes \
				-Wmissing-prototypes -Wswitch-default -Wbad-function-cast \
				-Wnested-externs -Wconversion -Wunreachable-code

CCXX_WARNINGS := 	-Wall -Wextra -pedantic -Wcast-align \
					-Winit-self -Wlogical-op -Wmissing-include-dirs \
			 		-Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef  \
					-Wwrite-strings -Wpointer-arith -Wmissing-declarations \
					-Wuninitialized -Wswitch-default \
					-Wconversion -Wunreachable-code

CXX_FLAGS := -std=c++11 $(CCXX_WARNINGS) -O3
CFLAGS := -std=gnu99 $(CCWARNINGS) -O3 -rdynamic

EXEC := main.out

# make V=1 oznacza verbose
ifeq ("$(origin V)", "command line")
  VERBOSE = $(V)
endif

ifndef VERBOSE
  VERBOSE = 0
endif

ifeq ($(VERBOSE),1)
  Q =
else
  Q = @
endif

# Chce print CC main.c zamiast g++ -Wall ...
define print_cc
	$(if $(Q), @echo "[CC]          $(1)")
endef

# Chce print bin main.out
define print_bin
	$(if $(Q), @echo "[BIN]         $(1)")
endef

# Projekt jest maly wiec wszystko w 1 folderze
SDIR := .
IDIR := ./

SRC := $(wildcard $(SDIR)/*.c)
SRC_CXX := $(wildcard $(SDIR)/*.cpp)

# main.o otrzymujemy z main.c itd
OBJ := $(SRC:%.c=%.o)
OBJ_CXX := $(SRC_CXX:%.cpp=%.opp)

# Zawsze rebuild
all: $(EXEC) _FORCE


$(EXEC): $(OBJ) $(OBJ_CXX) _FORCE
	$(call print_bin,$@)
	$(Q)$(CCXX) $(CXX_FLAGS) $(OBJ) $(OBJ_CXX) -I$(IDIR) -o $(EXEC)

# Regex: Jesli musisz zbudowac main.o skompiluj main.c
%.o: %.c _FORCE
	$(call print_cc,$<)
	$(Q)$(CC) $(CFLAGS) -I$(IDIR) -c $< -o $@

# Regex dla cpp
%.opp: %.cpp _FORCE
	$(call print_cc,$<)
	$(Q)$(CCXX) $(CXX_FLAGS) -I$(IDIR) -c $< -o $@


.PHONY: _FORCE
_FORCE:

clean:
	$(Q)rm -rf $(EXEC)
	$(Q)rm -rf $(OBJ)
	$(Q)rm -rf $(OBJ_CXX)
