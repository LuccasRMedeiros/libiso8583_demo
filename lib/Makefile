CC=gcc
STD=gnu99
CFLAGS=-Wall -Wextra -Wall
AR=ar

PROJ=libiso8583_demo.a

SRC=$(wildcard ./src/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))


%.o: %.c
	@$(CC) -std=$(STD) $(CFLAGS) -g -c $< -o $@

all: $(OBJ)
	@$(AR) r $(PROJ) $(OBJ)
	@echo "Finished!"

clean:
	@rm -f $(PROJ) $(OBJ)
	@echo "Project cleaned"

re: clean all
	
.PHONY: all clean re
