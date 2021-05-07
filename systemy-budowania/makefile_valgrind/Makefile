# Przyklad prostego Makefile do naszego projektu

# POLECENIA SHELLA
RM := rm -rf
TEE := tee
GREP := grep -q

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

define print_valgrind
    $(if $(Q), @echo "[VAL]       $(1)")
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

DEPS := $(OBJ:%.o=%.d)

LIB :=

# BINARKI

EXEC := main.out
TEXEC := test.out

# VALGRIND
VALGRIND := valgrind
VALGRIND_FLAGS := --leak-check=full -v
VALGRIND_PASS_STRING := "ERROR SUMMARY: 0"
VALGRIND_APP_LOG := ./valgrind_app_log.txt
VALGRIND_TEST_LOG := ./valgrind_test_log.txt

# KOMPILATOR
CC ?= gcc

C_FLAGS := -Wall -Wextra

DEP_FLAGS := -MMD -MP

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

C_FLAGS += $(OPT) $(GGDB) $(DEP_FLAGS)

all: $(EXEC)

test-all: test-regression

test: $(TEXEC)

test-run: $(TEXEC)
	./$(TEXEC)

test-valgrind: $(EXEC) $(TEXEC)
	$(call print_valgrind,$(EXEC))
	$(Q)$(VALGRIND) $(VALGRIND_FLAGS) ./$(EXEC) 2>&1 | $(TEE) $(VALGRIND_APP_LOG) | $(GREP) $(VALGRIND_PASS_STRING) \
		|| (echo "$(EXEC) nie dziala, zobacz logi: $(VALGRIND_APP_LOG)" && false)
	$(call print_valgrind,$(TEXEC))
	$(Q)$(VALGRIND) $(VALGRIND_FLAGS) ./$(TEXEC) 2>&1 | $(TEE) $(VALGRIND_TEST_LOG) | $(GREP) $(VALGRIND_PASS_STRING) \
		|| (echo "$(TEXEC) nie dziala, zobacz logi: $(VALGRIND_TEST_LOG)" && false)

test-regression: | $(EXEC) test test-run test-valgrind

$(EXEC): $(AOBJ)
	$(call print_bin,$@)
	$(Q)$(CC) $(C_FLAGS) $(H_INC) $(AOBJ) -o $@ $(L_INC)

$(TEXEC): $(TOBJ)
	$(call print_bin,$@)
	$(Q)$(CC) $(C_FLAGS) $(H_INC) $(TOBJ) -o $@ $(L_INC)

%.o:%.c %.d
	$(call print_cc,$<)
	$(Q)$(CC) $(C_FLAGS) $(H_INC) -c $< -o $@

clean:
	$(call print_rm,EXEC)
	$(Q)$(RM) $(EXEC)
	$(Q)$(RM) $(TEXEC)
	$(call print_rm,OBJ)
	$(Q)$(RM) $(OBJ)
	$(Q)$(RM) $(VALGRIND_APP_LOG) $(VALGRIND_TEST_LOG)
	$(call print_rm,DEPS)
	$(Q)$(RM) $(DEPS)

$(DEPS):


include $(wildcard $(DEPS))