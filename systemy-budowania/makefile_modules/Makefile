export CC := gcc
export C_FLAGS := -Wall -Wextra

all:
	@$(MAKE) -f ./module1/Makefile $@ --no-print-directory
	@$(MAKE) -f ./module2/Makefile $@ --no-print-directory
	@$(MAKE) -f ./module3/Makefile $@ --no-print-directory

test:
	@$(MAKE) -f ./module1/Makefile $@ --no-print-directory
	@$(MAKE) -f ./module2/Makefile $@ --no-print-directory
	@$(MAKE) -f ./module3/Makefile $@ --no-print-directory

clean:
	@$(MAKE) -f ./module1/Makefile $@ --no-print-directory
	@$(MAKE) -f ./module2/Makefile $@ --no-print-directory
	@$(MAKE) -f ./module3/Makefile $@ --no-print-directory