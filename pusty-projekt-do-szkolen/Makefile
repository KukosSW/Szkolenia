# SHELL SECTION
RM := rm -rf

# PRETTY PRINTS SECTION
define print_cc
	$(if $(Q), @echo "[CC]        $(1)")
endef

define print_bin
	$(if $(Q), @echo "[BIN]       $(1)")
endef

define print_rm
    $(if $(Q), @echo "[RM]        $(1)")
endef

# VERBOSE or NOT?
ifeq ("$(origin V)", "command line")
	Q :=
else
	Q ?= @
endif

# PROJECT TREE
SDIR := ./src
IDIR := ./inc ./external/KTest/inc
ADIR := ./app
TDIR := ./test

# FILES
SRC := $(wildcard $(SDIR)/*.c)

ASRC := $(SRC) $(wildcard $(ADIR)/*.c)
TSRC := $(SRC) $(wildcard $(TDIR)/*.c)

AOBJ := $(ASRC:%.c=%.o)
TOBJ := $(TSRC:%.c=%.o)

OBJ := $(AOBJ) $(TOBJ)

DEPS := $(OBJ:%.o=%.d)

LIB_PATH := ./external/KTest/
LIB := ktest

# EXEC
EXEC := main.out
TEXEC := test.out

# COMPILATOR SECTION

# By default use gcc
CC ?= gcc

C_FLAGS := -Wall -Wextra

DEP_FLAGS := -MMD -MP

H_INC := $(foreach d, $(IDIR), -I$d)
L_INC := $(foreach l, $(LIB), -l$l)
L_PATH := $(foreach p, $(LIB_PATH), -L$p)

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

C_FLAGS += $(OPT) $(GGDB) $(DEP_FLAGS)

all: $(EXEC)

test: $(TEXEC)

$(EXEC): $(AOBJ)
	$(call print_bin,$@)
	$(Q)$(CC) $(C_FLAGS) $(H_INC) $(AOBJ) -o $@ $(L_PATH) $(L_INC)

$(TEXEC): $(TOBJ)
	$(call print_bin,$@)
	$(Q)$(CC) $(C_FLAGS) $(H_INC) $(TOBJ) -o $@ $(L_PATH) $(L_INC)

%.o:%.c %.d
	$(call print_cc,$<)
	$(Q)$(CC) $(C_FLAGS) $(H_INC) -c $< -o $@

clean:
	$(call print_rm,EXEC)
	$(Q)$(RM) $(EXEC)
	$(Q)$(RM) $(TEXEC)
	$(call print_rm,OBJ)
	$(Q)$(RM) $(OBJ)
	$(call print_rm,DEPS)
	$(Q)$(RM) $(DEPS)

$(DEPS):

include $(wildcard $(DEPS))