# Which compiler
CC = gcc

LD_PRELOAD=/usr/lib64/liblua-5.1.so
INCLUDE = -Iinc -I/home/vfhky/include/test/

LIBDIR = -L/usr/lib64

LIBS = -llua -ldl -lm -lprintnolog

SRCDIR = src

OBJDIR = obj

BINDIR = bin

INCDIR = inc

OBJS = obj/main.o obj/server.o obj/child_svr.o obj/bussiness.o

NAME= Server

RESOURCE = resource/*

# Option for developement
CFLAGS = -g -Wall -ansi -DLUA_32BITS

# Option for release
# CFLAGS = -O -Wall 

.PHONY: build rebuild clean copy

all: build

build: $(BINDIR)/$(NAME) copy

copy: $(RESOURCE)
	@echo 'Copy resource files'
	-cp -r $(RESOURCE) $(BINDIR)/
	@echo 'Finished copy resource files'
	@echo

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/%.h
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
	@echo 

rebuild: clean build

clean:
	-rm -rf $(OBJDIR)/* 
	-rm -rf $(BINDIR)/*

