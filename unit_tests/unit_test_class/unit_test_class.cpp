#include "unit_test_class.hpp"

int rec_signal = 0;

static void sig_intercept(int signal)
{
    std::cout << MESSAGE_FORMAT << "\nProgram crashed: ";

    switch(signal)
    {
    case SIGABRT:
        std::cout << ERROR_FORMAT << "SIGABRT" << RESET_FORMAT << std::endl;
        break;
    case SIGFPE:
        std::cout << ERROR_FORMAT << "SIGFPE" << RESET_FORMAT << std::endl;
        break;
    case SIGILL:
        std::cout << ERROR_FORMAT << "SIGILL" << RESET_FORMAT << std::endl;
        break;
    case SIGSEGV:
        std::cout << ERROR_FORMAT << "SIGSEGV" << RESET_FORMAT << std::endl;
        break;
	default:
		std::cout << ERROR_FORMAT << "(" << signal << ") Finishing test execution" << RESET_FORMAT << std::endl;
    }
	
	exit(signal);
}

namespace SimpleTestClasses
{
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
	    std::signal(SIGABRT, sig_intercept);
	    std::signal(SIGFPE, sig_intercept);
	    std::signal(SIGILL, sig_intercept);
	    std::signal(SIGINT, sig_intercept);
	    std::signal(SIGSEGV, sig_intercept);
	    std::signal(SIGTERM, sig_intercept);
	}
	
	void SimpleTest::add_test(std::string test_title, std::function<bool ()> test_case)
	{
		SimpleUnitTest new_test = SimpleUnitTest(test_title, test_case);
		_vec_unit_tests.push_back(new_test);
	}
	
	int SimpleTest::run_tests(void)
	{
	    std::cout << RESET_FORMAT << "Number of tests: " << _vec_unit_tests.size() << std::endl;
	    
	    for (unsigned int test_n = 0; test_n < _vec_unit_tests.size(); ++test_n)
	    {
	        if (_vec_unit_tests[test_n].run() == true)
	            _success_count += 1;
	        else
	            _error_count += 1;
	    }
	
	    std::cout << RESET_FORMAT << "Finished, success count: " << _success_count << ", failures: " << _error_count << std::endl;
		return _error_count;
	}
}

namespace SimpleTestTemplates
{

	bool assert_nosig(void)
	{
		if (rec_signal != 0)
		{
			rec_signal = 0;
			return false;
		}

		return true;
	}
}
