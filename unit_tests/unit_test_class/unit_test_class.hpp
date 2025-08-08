#ifndef UNIT_TEST_CLASS_HPP
#define UNIT_TEST_CLASS_HPP

#include <functional>
#include <string>
#include <vector>


class SimpleUnitTest
{
    private:
        std::function<bool(void)> _test_case;
        std::string _test_title;

    public:
        SimpleUnitTest(std::string test_title, std::function<bool(void)> test_case);

        bool run(void);
};

class SimpleTest 
{
    private:
        unsigned int _success_count;
        unsigned int _error_count;

    public:
        SimpleTest();

        std::vector<SimpleUnitTest> vec_unit_tests;

        void run_tests(void);
};

extern int rec_signal;

void sig_intercept(int signal);

#endif
