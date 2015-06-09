 .PHONY: build
 
# Which compiler
CC = gcc

INCLUDE = -Iinc

LIBDIR =

SRCDIR = src

OBJDIR = obj

BINDIR = Debug

LIBS =

OBJS = obj/main.o obj/server.o obj/child_svr.o obj/bussiness.o

# Option for developement
CFLAGS = -g -Wall -ansi

# Option for release
# CFLAGS = -O -Wall 

 
build: $(BINDIR)/Server


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: $(CC) Compiler'
	$(CC) $(INCLUDE) $(LIBDIR) $(CFLAGS) -o "$@" -c "$<"
	@echo 'Finished building: $<'
	@echo

$(BINDIR)/Server: $(OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: $(CC) Linker'
	$(CC) -o $(BINDIR)/Server $(OBJS) $(LIBDIR) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

.PHONY: rebuild

rebuild: clean build

.PHONY: clean

clean:
	-rm $(OBJDIR)/* 
	-rm $(BINDIR)/*

