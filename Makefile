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
	ld -r -o $@ $^

${BUILD_DIR}/%.o: ${CFILES_DIR}/%.c
	mkdir -p $(dir $@)
	$(CC) ${CFLAGS} ${INCLUDES} -c $< -o $@

clean:
	rm -rf ${OBJECTS}
	rm -rf ${EXEC}
	rm -rf ${MERGED_OBJECT}

testleak: ${EXEC}
	valgrind --leak-check=full --show-leak-kinds=all ./$^ ./tests/prelim.com


testrom: ${EXEC} 
	./$< ./tests/prelim.com

debug: ${EXEC}
	cgdb --args ./$< ./tests/prelim.com
