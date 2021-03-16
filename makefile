CC=gcc

OBJS = aubatch

SRCS := $(shell find $(./src) -name *.c)

REBUILDABLES = $(OBJS)

all: $(SRCS)
	@clear 
	$(CC) -pthread -o aubatch $(SRCS)
	@echo build complete


clean: 
	rm -f $(REBUILDABLES)
	@echo clean complete