#include "unit_test_class.hpp"

#include <cstring>

extern "C"
{
#include "iso8583_demo.h"
#include "luhn_validation.h"
#include "stt_machine.h"
#include "memory.h"
}


using namespace std;

static SimpleTest test_libiso8583_demo;

void initialize_tests(void)
{
    SimpleUnitTest test1(
            "Test if memory_init initializes memory without failure",
            []() -> bool {
                memory_init();

                if (rec_signal != 0)
                {
                    rec_signal = 0;
                    return false;
                }
                return true;
            });
    test_libiso8583_demo.vec_unit_tests.push_back(test1);

    SimpleUnitTest test2(
            "Test if memory_erase_block erases block",
            []() -> bool {
                if (memory_erase_block(1) != 0 || rec_signal != 0)
                {
                    rec_signal = 0;
                    return false;
                }
                return true;
            });
    test_libiso8583_demo.vec_unit_tests.push_back(test2);

    SimpleUnitTest test3(
            "Test if flash_memory_write write a file successfully",
            []() -> bool {
                char filename[9] = "test.txt";
                char data[13] = "Hello World!";
                if (flash_memory_write(filename, data, 12) != 0 || rec_signal != 0)
                {
                    rec_signal = 0;
                    return false;
                }
                return true;
            });
    test_libiso8583_demo.vec_unit_tests.push_back(test3);

    SimpleUnitTest test4(
            "Test if flash_memory_read finds the written file",
            []() -> bool {
                flash_memory_dir_st *files;
                if (flash_memory_read(&files) != 0 || rec_signal != 0)
                {
                    rec_signal = 0;
                    return false;
                }

                if (strncmp(files[0].filename, "test.txt", 8) != 0)
                    return false;
                return true;
            });
    test_libiso8583_demo.vec_unit_tests.push_back(test4);
}

int main(void)
{
    initialize_tests();
    test_libiso8583_demo.run_tests();
}
