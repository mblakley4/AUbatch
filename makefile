CC=gcc

OBJS = aubatch

SRCS := $(shell find $(./src) -name *.c)
mSRCS = ./src/aubatch.c \
				./src/scheduler/scheduler.c \
				./src/dispatcher/dispatcher.c \
				./src/cmd_line_tools/cmd_line_parser.c \
				./src/performance/performance.c


TEST = @echo "hi"

REBUILDABLES = $(OBJS)


all: $(SRCS)
	@clear 
	$(CC) -pthread -o $(OBJS) $(SRCS)
	@echo build complete


clean: 
	rm -f $(REBUILDABLES)
	@echo clean complete

mac: $(mSRCS)
	@clear
	$(CC) -pthread -o $(OBJS) $(mSRCS)
	@echo build complete
	

#	gcc -pthread -o aubatch ./src/cmd_line_tools/cmd_line_parser.c ./src/dispatcher/dispatcher.c ./src/performance/performance.c ./src/scheduler/scheduler.c ./src/aubatch.c