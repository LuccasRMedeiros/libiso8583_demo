CC=gcc
STD=gnu99
CFLAGS=-Wall -Wextra -Wall
AR=ar

PROJ=pos_demo

SRC=$(wildcard ./src/*.c)
INC=inc
OBJ=$(patsubst %.c, %.o, $(SRC))
LIB=libs
LINK=iso8583_demo


all:
	@$(CC) -std=$(STD) $(CFLAGS) -g -I$(INC) $(SRC) -L $(LIB) -l$(LINK) -o $(PROJ)
	@echo "Finished!"

clean:
	@rm -f $(PROJ)
	@echo "Project cleaned"

re: clean all

.PHONY: all debug clean
