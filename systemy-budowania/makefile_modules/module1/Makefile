# MAKEFILE_LIST to lista Makaefile do odpalenia, wiec ostatni wyraz w liscie to nasz Makefile
# abspath tworzy z pliku pelna sciezke np /home/Kukos/a/b/c/file
# dir usunie file z poprzedniego stringa
# Teraz jak zrobic z /a/b/c/d/ samo d?, skoro dir zwraca dir to notdir zwroci filename
# Ale my nie mamy plikow :(
# Usunmy ostatni / i wtedy Makefile pomysli ze d to plik
THIS_DIR := $(notdir $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST))))))

SDIR := ./src

all:
	@echo "Wywolales $@ z $(THIS_DIR)"
	@echo "	CC = $(CC)"
	@echo "	C_FLAGS = $(C_FLAGS)"

test:
	@echo "Wywolales $@ z $(THIS_DIR)"

clean:
	@echo "Wywolales $@ z $(THIS_DIR)"
