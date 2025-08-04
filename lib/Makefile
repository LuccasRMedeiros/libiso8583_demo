CC=gcc
STD=c99
CFLAGS=-Wall -Wextra -Wall
DEBUG=
AR=ar

PROJ=libiso8583_demo.a

SRC=$(wildcard ./src/*.c)
INC=$(wildcard ./src/*.h)
OBJ=$(patsubst %.c, %.o, $(SRC))


all:
	@$(CC) -std=$(STD) $(CFLAGS) $(DEBUG) -c $(SRC) -o $(OBJ)
	@$(AR) r $(PROJ) $(OBJ)
	@echo "Finished!"

debug: DEBUG=-g 

debug: all

clean:
	@rm -f $(PROJ) $(OBJ)
	@echo "Project cleaned"

.PHONY: all debug clean
