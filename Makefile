.PHONY: clean all run
.DEFAULT_GOAL := all

BIN_NAME=minesweeper

CC=gcc

ODIR=obj
BDIR=bin

CFLAGS=-Werror -Wall -pedantic
INCS=
LIBS=

SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(ODIR)/%.o,$(filter-out $(patsubst %,%.c,$(BIN_NAME)), $(SRCS)))
BINS=$(patsubst %,$(BDIR)/%,$(BIN_NAME))

.depend: $(SRCS)
	$(CC) $(CFLAGS) -MM $^ > ./.depend
	sed -i.bak -e 's/^/${ODIR}\//' ./.depend && rm ./.depend.bak

-include .depend

$(BDIR) $(ODIR):
	@echo "MKDIR $@"
	@mkdir -p $@

$(ODIR)/%.o: $(ODIR)
	@echo "GCC   $^ -> $@"
	@$(CC) -c -o $@ $< $(CFLAGS) $(INCS)

$(BINS): $(BDIR)/%: $(ODIR)/%.o $(OBJS) | $(BDIR)
	@echo "GCC   $@"
	@gcc -o $@ $^ $(CFLAGS) $(LIBS)

all: $(BINS)

clean:
	@echo "CLEAN $(ODIR) $(BDIR) .depend"
	@rm -rf $(ODIR) $(BDIR) .depend

run: $(BINS)
	@echo "RUN   $@"
	@./$(BDIR)/$(BIN_NAME)
