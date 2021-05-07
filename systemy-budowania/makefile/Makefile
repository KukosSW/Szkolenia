# Przyklad prostego Makefile do naszego projektu

# POLECENIA SHELLA
RM := rm -rf

# MOJE FUNKCJE
define print_cc
	@echo "[CC]        $(1)"
endef

define print_bin
	@echo "[BIN]       $(1)"
endef

# FOLDERY
SDIR := ./src
IDIR := ./inc
ADIR := ./app
TDIR := ./test

# PLIKI
SRC := $(wildcard $(SDIR)/*.c)

ASRC := $(SRC) $(wildcard $(ADIR)/*.c)
TSRC := $(SRC) $(wildcard $(TDIR)/*.c)

AOBJ := $(ASRC:%.c=%.o)
TOBJ := $(TSRC:%.c=%.o)

OBJ := $(AOBJ) $(TOBJ)

LIB :=

# BINARKI

EXEC := main.out
TEXEC := test.out

# KOMPILATOR
CC := gcc

C_FLAGS := -Wall -Wextra

H_INC := $(foreach d, $(IDIR), -I$d)
L_INC := $(foreach l, $(LIB), -l$l)

all: $(EXEC)


test: $(TEXEC)


$(EXEC): $(AOBJ) _FORCE
	$(call print_bin,$@)
	@$(CC) $(C_FLAGS) $(H_INC) $(AOBJ) -o $@ $(L_INC)

$(TEXEC): $(TOBJ) _FORCE
	$(call print_bin,$@)
	@$(CC) $(C_FLAGS) $(H_INC) $(TOBJ) -o $@ $(L_INC)

%.o:%.c _FORCE
	$(call print_cc,$<)
	@$(CC) $(C_FLAGS) $(H_INC) -c $< -o $@

_FORCE:

clean:
	$(RM) $(EXEC)
	$(RM) $(TEXEC)
	$(RM) $(OBJ)