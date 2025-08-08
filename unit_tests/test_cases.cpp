#include "unit_test_class.hpp"

#include <cstring>
#include <iostream>

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
    test_libiso8583_demo.vec_unit_tests.push_back(test2);

    SimpleUnitTest test3(
            "Test if flash_memory_read finds the written file",
            []() -> bool {
                flash_memory_dir_st *files;

                if (flash_memory_read(&files) != 0 || rec_signal != 0)
                {
                    rec_signal = 0;
                    return false;
                }

                if (memcmp(files[0].filename, "test.txt", 8) != 0)
                    return false;
                return true;
            });
    test_libiso8583_demo.vec_unit_tests.push_back(test3);

    SimpleUnitTest test4(
            "Test if flash_memory_get_str_filename create a valid null terminated string",
            []() -> bool {
                char dir_filename[FILENAME_SIZE+1] = { '\0' };
                byte test_str[FILENAME_SIZE] = { 0xff };
                
                memcpy(test_str, "foobar", 6);

                if (flash_memory_get_str_filename(test_str, dir_filename) != 0 || rec_signal != 0)
                {
                    rec_signal = 0;
                    return false;
                }

                if (strlen(dir_filename) != 6 || rec_signal != 0) // Meaning it is not null terminated
                {
                    rec_signal = 0;
                    return false;
                }
                return true;
            });
    test_libiso8583_demo.vec_unit_tests.push_back(test4);

    SimpleUnitTest test5(
            "Test if flash_memory_read_file reads the content of \"test.txt\"",
            []() -> bool {
                byte *dy_file_content = NULL;
                
                if (flash_memory_read_file("test.txt", &dy_file_content) != 0 || rec_signal != 0)
                {
                    if (dy_file_content != NULL)
                        free(dy_file_content);

                    rec_signal = 0;
                    return false;
                }

                if (memcmp(dy_file_content, "Hello World!", 12) != 0 || rec_signal != 0)
                {
                    if (dy_file_content != NULL)
                        free(dy_file_content);

                    rec_signal = 0;
                    return false;
                }

                free(dy_file_content);
                return true;
            });
    test_libiso8583_demo.vec_unit_tests.push_back(test5);

    SimpleUnitTest test6(
            "Test if memory_erase_block erases block",
            []() -> bool {
                flash_memory_dir_st *files;

                if (memory_erase_block(1) != 0 || rec_signal != 0)
                {
                    rec_signal = 0;
                    return false;
                }

                flash_memory_read(&files);
                if (rec_signal != 0)
                {
                    rec_signal = 0;
                    cout << ERROR_FORMAT << "flash_memory_read broke!" << RESET_FORMAT << endl;
                    return false;
                }

                if (files[0].filename[0] != 0xff) // Meaning it was not erased
                    return false;
                return true;
            });
    test_libiso8583_demo.vec_unit_tests.push_back(test6);
}

int main(void)
{
    initialize_tests();
    test_libiso8583_demo.run_tests();
}
