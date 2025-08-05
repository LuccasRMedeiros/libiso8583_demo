CC=gcc
STD=gnu99
CFLAGS=-Wall -Wextra -Wall
DEBUG=
AR=ar

PROJ=libiso8583_demo.a

SRC=$(wildcard ./src/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))


%.o: %.c
	@$(CC) -std=$(STD) $(CFLAGS) $(DEBUG) -c $< -o $@

all: $(OBJ)
	@$(AR) r $(PROJ) $(OBJ)
	@echo "Finished!"

debug: DEBUG=-g 

debug: all

clean:
	@rm -f $(PROJ) $(OBJ)
	@echo "Project cleaned"

.PHONY: all debug clean
