CC=g++
STD=c++11
CFLAGS=-Wall -Wextra -g
AR=ar

PROJ=pos_demo_unit_test

SRC=$(wildcard unit_test_class/*.cpp) test_cases.cpp
INC=-Iunit_test_class -Itest_subject
OBJ=$(patsubst %.cpp, %.o, $(SRC))
LIB=test_subject
LINK=iso8583_demo

%.o: %.cpp
	@$(CC) -std=$(STD) $(CFLAGS) $(INC) -c $< -o $@

all: $(PROJ)

$(PROJ): $(OBJ)
	@$(CC) -std=$(STD) $(CFLAGS) $(INC) $(OBJ) -L$(LIB) -l$(LINK) -o $@
	@echo "Build complete!"

clean:
	@rm -f $(PROJ) $(OBJ)
	@echo "Project cleaned"

re: clean all

.PHONY: all clean re
