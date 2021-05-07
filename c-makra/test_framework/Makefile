# Prosty Makefile, w takim projekcie duzy makefile to overkill

CC := gcc

CCWARNINGS := 	-Wall -Wextra -pedantic -Wcast-align \
				-Winit-self -Wlogical-op -Wmissing-include-dirs \
			 	-Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef  \
				-Wwrite-strings -Wpointer-arith -Wmissing-declarations \
				-Wuninitialized -Wold-style-definition -Wstrict-prototypes \
				-Wmissing-prototypes -Wswitch-default -Wbad-function-cast \
				-Wnested-externs -Wconversion -Wunreachable-code

CFLAGS := -std=gnu17 $(CCWARNINGS) -O3 -rdynamic

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
SDIR := ./
IDIR := ./

SRC := $(wildcard $(SDIR)/*.c)

# main.o otrzymujemy z main.c itd
OBJ := $(SRC:%.c=%.o)

# Zawsze rebuild
all: $(EXEC) _FORCE


$(EXEC): $(OBJ) _FORCE
	$(call print_bin,$@)
	$(Q)$(CC) $(CFLAGS) $(OBJ) -I$(IDIR) -lm -o $(EXEC)

# Regex: Jesli musisz zbudowac main.o skompiluj main.c
%.o: %.c _FORCE
	$(call print_cc,$<)
	$(Q)$(CC) $(CFLAGS) -I$(IDIR) -c $< -o $@

.PHONY: _FORCE
_FORCE:


clean:
	$(Q)rm -rf $(EXEC)
	$(Q)rm -rf $(OBJ)
