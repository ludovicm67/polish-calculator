# We define variables for directories
DIROBJ  = obj
DIRINC  = include
DIRSRC  = src
DIRBIN  = bin

# Compiler
CC      = gcc
CFLAGS  = -g -std=gnu11 -pedantic -O3 -march=native -Wall -Wextra -Werror

# Dependencies, objects, ...
DEPS    = $(wildcard include/*.h)
SOURCES = $(wildcard src/*.c)
OBJETS  = $(SOURCES:src/%.c=obj/%.o)
EXEC    = pc

.SUFFIXES:

# We create targets
$(DIRBIN)/$(EXEC) : $(OBJETS) | $(DIRBIN)
	$(CC) $(CFLAGS) -o $@ $^

$(DIROBJ)/%.o : $(DIRSRC)/%.c $(DEPS) | $(DIROBJ)
	$(CC) $(CFLAGS) -c -I$(DIRINC) -o $@ $<

$(DIROBJ) $(DIRBIN):
	@mkdir -p $@

# Targets to call manually
.PHONY: archive
archive:
	tar -f archive.tar.gz -cvz $(DIRSRC)/*.c $(DIRINC)/*.h Makefile

.PHONY: clean
clean:
	rm -rf $(DIROBJ) $(DIRBIN)
