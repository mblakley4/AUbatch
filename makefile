CC=gcc

OBJS = aubatch

#SRCS := $(shell find $(./src) -name *.c)
SRCS = ./src/aubatch.c \
				./src/scheduler/scheduler.c \
				./src/dispatcher/dispatcher.c \
				./src/cmd_line_tools/cmd_line_parser.c \
				./src/performance/perf_info.c \
				./src/performance/auto_eval.c


TEST = @echo "hi"

REBUILDABLES = $(OBJS)

all: $(SRCS)
	@clear 
	$(CC) -pthread -o $(OBJS) $(SRCS)
	@echo build complete


clean: 
	rm -f $(REBUILDABLES)
	@echo clean complete

	