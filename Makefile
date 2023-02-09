CC = gcc
OPT = -O2
CFLAGS = -std=c2x -fPIC -g -Wall -Wextra -pedantic $(OPT)

INCLUDES= -I ./include

CFILES_DIR := ./src
BUILD_DIR := ./build

CFILES := $(wildcard ./src/*.c)
CFILES := $(filter-out src/test.c, $(CFILES))
OBJECTS := $(CFILES:$(CFILES_DIR)/%.c=$(BUILD_DIR)/%.o)

MERGED_OBJECT := ./cpuZ80.o
EXEC := ./cpuZ80test

${EXEC}: ${MERGED_OBJECT}
	gcc ${CFLAGS} ${INCLUDES} $^ -o $@

${MERGED_OBJECT}: ${OBJECTS}
	ld -r -o $@ $^ -g

${BUILD_DIR}/%.o: ${CFILES_DIR}/%.c
	mkdir -p $(dir $@)
	$(CC) ${CFLAGS} ${INCLUDES} -c $< -o $@

clean:
	rm -rf ${OBJECTS}
	rm -rf ${EXEC}
	rm -rf ${MERGED_OBJECT}

prelim: ${EXEC} 
	./$< ./tests/prelim.com

zexdoc: ${EXEC}
	./$< ./tests/zexdoc.cim

zexall: ${EXEC}
	./$< ./tests/zexall.cim

debug: ${EXEC}
	cgdb --args ./$< ./tests/prelim.com
