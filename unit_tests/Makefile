CC=g++
STD=c++11
CFLAGS=-Wall -Wextra
AR=ar

PROJ=libsimple_test.a
TEST=pos_demo_unit_test

SRC=$(wildcard unit_test_class/*.cpp)
INC=unit_test_class
OBJ=$(patsubst %.cpp, %.o, $(SRC))

TEST_SUBJECT=../lib
TEST_LINK=iso8583_demo
TEST_INC=$(TEST_SUBJECT)/src/
TEST_SRC=test_cases.cpp

all: $(TEST)
	@./$(TEST)

debug: CFLAGS += -g -O0

debug: $(TEST)

%.o: %.cpp
	$(CC) -std=$(STD) $(CFLAGS) -I$(INC) -c $< -o $@

$(TEST): lib
	$(CC) -std=$(STD) $(CFLAGS) $(TEST_SRC) -I$(INC) -I$(TEST_INC) -L$(TEST_SUBJECT) -l$(TEST_LINK) ./$(PROJ) -o $@

lib: $(OBJ)
	@$(AR) r $(PROJ) $(OBJ)

clean:
	@rm -f $(TEST) $(PROJ) $(OBJ)

re: clean all

.PHONY: all clean re
