#ifndef TEST_CASE_HPP
#define TEST_CASE_HPP

#include <functional>
#include <string>

class Test_case
{
    public:
        std::string test_name;
        std::function<T> test_function;
};

#endif
