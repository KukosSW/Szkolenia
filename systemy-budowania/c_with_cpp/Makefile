# Przyklad prostego Makefile do naszego projektu

# POLECENIA SHELLA
RM := rm -rf

# MOJE FUNKCJE
define print_cc
	@echo "[CC]        $(1)"
endef

define print_ccpp
	@echo "[CC++]      $(1)"
endef

define print_bin
	@echo "[BIN]       $(1)"
endef

define print_rm
	@echo "[RM]        $(1)"
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

# PLIKI C++
# Gdybym zrobil rule w poprzednim src, to wtedy %.c=%.o nie dziala
# Poniewaz jesli nie znajdzie %.c (czyli file2.cpp) to przepusci plik bez zmiany
# Zatem bedziemy mieli w  OBJ pliki .cpp z rula %.c i pliki .c z rula %.cpp
SRC_CPP := $(wildcard $(SDIR)/*.cpp)
OBJ_CPP := $(SRC_CPP:%.cpp=%.opp)

AOBJ += $(OBJ_CPP)
TOBJ += $(OBJ_CPP)

OBJ := $(AOBJ) $(TOBJ)

LIB :=

# BINARKI

EXEC := main.out
TEXEC := test.out

# KOMPILATOR
CC := gcc

CCPP := g++

C_FLAGS := -Wall -Wextra
CPP_FLAGS := -Wall -Wextra -std=c++11

H_INC := $(foreach d, $(IDIR), -I$d)
L_INC := $(foreach l, $(LIB), -l$l)

all: $(EXEC)


test: $(TEXEC)


$(EXEC): $(AOBJ) _FORCE
	$(call print_bin,$@)
	@$(CCPP) $(CPP_FLAGS) $(H_INC) $(AOBJ) -o $@ $(L_INC)

$(TEXEC): $(TOBJ) _FORCE
	$(call print_bin,$@)
	@$(CCPP) $(CPP_FLAGS) $(H_INC) $(TOBJ) -o $@ $(L_INC)

%.o:%.c _FORCE
	$(call print_cc,$<)
	@$(CC) $(C_FLAGS) $(H_INC) -c $< -o $@

# Musi byc inne rozszerzenie, bo inaczej object by wszedl w rula wyzej
%.opp:%.cpp _FORCE
	$(call print_ccpp,$<)
	@$(CCPP) $(CPP_FLAGS) $(H_INC) -c $< -o $@

_FORCE:

clean:
	$(call print_rm,EXEC)
	@$(RM) $(EXEC)
	@$(RM) $(TEXEC)
	$(call print_rm,OBJ)
	@$(RM) $(OBJ)