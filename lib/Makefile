CC=gcc
STD=gnu99
CFLAGS=-Wall -Wextra -Wall
AR=ar

PROJ=libiso8583_demo.a

SRC=$(wildcard ./src/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))


%.o: %.c
	$(CC) -std=$(STD) $(CFLAGS) -c $< -o $@

all: $(OBJ)
	@$(AR) r $(PROJ) $(OBJ)

debug: CFLAGS += -g

debug: $(OBJ)
	@$(AR) r $(PROJ) $(OBJ)

clean:
	@rm -f $(PROJ) $(OBJ)

re: clean all
	
.PHONY: all debug clean re
