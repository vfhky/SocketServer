# Which compiler
CC = gcc

INCLUDE = -Iinc

LIBDIR = -L/usr/lib64

SRCDIR = src

OBJDIR = obj

BINDIR = Debug

LIBS = -llua 

OBJS = obj/main.o obj/server.o obj/child_svr.o obj/bussiness.o

NAME= Server

RESOURCE = resource/*

# Option for developement
CFLAGS = -g -Wall -ansi

# Option for release
# CFLAGS = -O -Wall 

 
build: $(BINDIR)/$(NAME) copy

copy: $(RESOURCE)
	@echo 'Copy resource files'
	cp -rf $(RESOURCE) $(BINDIR)/
	@echo 'Finished copy resource files'
	@echo

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: $(CC) Compiler'
	$(CC) $(INCLUDE) $(LIBDIR) $(CFLAGS) -o "$@" -c "$<"
	@echo 'Finished building: $<'
	@echo

$(BINDIR)/$(NAME): $(OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: $(CC) Linker'
	$(CC) -o $(BINDIR)/$(NAME) $(OBJS) $(LIBDIR) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

.PHONY: build rebuild clean copy

rebuild: clean build

clean:
	-rm -rf $(OBJDIR)/* 
	-rm -rf $(BINDIR)/*

