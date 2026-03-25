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
	$(CC) -std=$(STD) $(CFLAGS) -I$(INC) $(SRC) -L $(LIB) -l$(LINK) -o $(PROJ)

debug: CFLAGS += -g

debug:
	@$(CC) -std=$(STD) $(CFLAGS) -I$(INC) $(SRC) -L $(LIB) -l$(LINK) -o $(PROJ)

clean:
	@rm -f $(PROJ)

re: clean all

.PHONY: all debug clean re
