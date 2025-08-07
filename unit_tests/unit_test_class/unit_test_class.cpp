#include "test_case.hpp"

#include <iostream>

#define TITLE_FORMAT    "\e[1;34m"
#define SUCCESS_FORMAT  "\e[1;32m"
#define ERROR_FORMAT    "\e[1;31m"
#define RESET_FORMAT    "\e[0m"

SimpleUnitTest::SimpleUnitTest(
        std::string test_title,
        std::function<bool(void)> test_case)
{
    _test_title = test_title;
    _test_case = test_case;
}

bool SimpleUnitTest::run(void)
{
    std::cout << TITLE_FORMAT << _test_title << ": ";

    if (_test_case() == true)
    {
        std::cout << SUCCESS_FORMAT << "OK!" << RESET_FORMAT << std::endl;
        return true;
    }

    std::cout << ERROR_FORMAT << "FAIL." << RESET_FORMAT << std::endl;

    return false;
}

SimpleTest::SimpleTest()
{
    _success_count = 0;
    _error_count = 0;
}

void SimpleTest::run_tests(void)
{
    std::cout << RESET_FORMAT << "Number of tests: " << vec_unit_tests.size() << std::endl;
    
    for (unsigned int test_n = 0; test_n < vec_unit_tests.size(); ++test_n)
    {
        if (vec_unit_tests[test_n].run() == true)
            _success_count += 1;
        else
            _error_count += 1;
    }

    std::cout << RESET_FORMAT << "Finished, success count: " << _success_count << ", failures: " << _error_count << std::endl;
}
