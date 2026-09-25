CC      = gcc
CFLAGS  = -O2 -std=c11 -Wall -Wextra
BINDIR  = bin
SRC     = src

.PHONY: all clean

all: $(BINDIR)/clean_merge $(BINDIR)/build_tokenizer

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/clean_merge: $(SRC)/clean_merge.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BINDIR)/build_tokenizer: $(SRC)/build_tokenizer.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(BINDIR)
