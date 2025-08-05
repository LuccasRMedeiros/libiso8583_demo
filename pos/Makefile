CC=gcc
STD=gnu99
CFLAGS=-Wall -Wextra -Wall
DEBUG=
AR=ar

PROJ=pos_demo

SRC=$(wildcard ./src/*.c)
INC=inc
OBJ=$(patsubst %.c, %.o, $(SRC))
LIB=lib
LINK=iso8583_demo


all:
	@$(CC) -std=$(STD) $(CFLAGS) $(DEBUG) -I$(INC) $(SRC) -L $(LIB) -l$(LINK) -o $(PROJ)
	@echo "Finished!"

debug: DEBUG=-g 

debug: all

clean:
	@rm -f $(PROJ)
	@echo "Project cleaned"

.PHONY: all debug clean
