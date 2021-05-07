# Przyklad prostego Makefile do naszego projektu

# POLECENIA SHELLA
RM := rm -rf

# MOJE FUNKCJE
define print_cc
	$(if $(Q), @echo "[CC]        $(1)")
endef

define print_bin
	$(if $(Q), @echo "[BIN]       $(1)")
endef

define print_rm
    $(if $(Q), @echo "[RM]        $(1)")
endef

# PRZETWARZAMY ARGUMENT DO MAKEFILE (Quiet czy Verbose)
ifeq ("$(origin V)", "command line")
	Q :=
else
	Q ?= @
endif

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
CC ?= gcc

C_FLAGS := -Wall -Wextra

H_INC := $(foreach d, $(IDIR), -I$d)
L_INC := $(foreach l, $(LIB), -l$l)

# PRZETWARZAMY ARGUMENTY DO KOMPILACJI
ifeq ($(CC),clang)
	C_FLAGS += -Weverything
else ifneq (, $(filter $(CC), cc gcc))
	C_FLAGS += -rdynamic
endif

ifeq ("$(origin O)", "command line")
	OPT := -O$(O)
else
	OPT := -O3
endif

ifeq ("$(origin G)", "command line")
	GGDB := -ggdb$(G)
else
	GGDB :=
endif

C_FLAGS += $(OPT) $(GGDB)

all: $(EXEC)

test: $(TEXEC)

$(EXEC): $(AOBJ) _FORCE
	$(call print_bin,$@)
	$(Q)$(CC) $(C_FLAGS) $(H_INC) $(AOBJ) -o $@ $(L_INC)

$(TEXEC): $(TOBJ) _FORCE
	$(call print_bin,$@)
	$(Q)$(CC) $(C_FLAGS) $(H_INC) $(TOBJ) -o $@ $(L_INC)

%.o:%.c _FORCE
	$(call print_cc,$<)
	$(Q)$(CC) $(C_FLAGS) $(H_INC) -c $< -o $@

_FORCE:

clean:
	$(call print_rm,EXEC)
	$(Q)$(RM) $(EXEC)
	$(Q)$(RM) $(TEXEC)
	$(call print_rm,OBJ)
	$(Q)$(RM) $(OBJ)
