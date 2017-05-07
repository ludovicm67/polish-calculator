# We define variables for directories
DIROBJ  = obj
DIRINC  = include
DIRSRC  = src

# Compiler
CC      = gcc
CFLAGS  = -g -std=gnu11 -pedantic -O3 -march=native -Wall -Wextra -Werror
LDFLAGS = -lm

# Dependencies, objects, ...
DEPS    = $(wildcard $(DIRINC)/*.h)
SOURCES = $(wildcard $(DIRSRC)/*.c)
OBJETS  = $(SOURCES:$(DIRSRC)/%.c=$(DIROBJ)/%.o)
EXEC    = pc

.SUFFIXES:

# We create targets
$(EXEC) : $(OBJETS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(DIROBJ)/%.o : $(DIRSRC)/%.c $(DEPS) | $(DIROBJ)
	$(CC) $(CFLAGS) -c -I$(DIRINC) -o $@ $<

$(DIROBJ):
	@mkdir -p $@

# Targets to call manually
.PHONY: archive
archive:
	tar -f archive.tar.gz -cvz $(DIRSRC)/*.c $(DIRINC)/*.h Makefile

.PHONY: clean
clean:
	rm -rf $(DIROBJ) $(EXEC) test/*.tmp.txt

.PHONY: test
test: clean $(EXEC)
	./check.sh
