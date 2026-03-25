#ifndef UNIT_TEST_CLASS_HPP
#define UNIT_TEST_CLASS_HPP

#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include <csignal>
#include <iostream>

#define TITLE_FORMAT    "\e[1;34m"
#define SUCCESS_FORMAT  "\e[1;32m"
#define ERROR_FORMAT    "\e[1;31m"
#define MESSAGE_FORMAT	"\e[1;33m"
#define RESET_FORMAT    "\e[0m"

extern int rec_signal;

//void sig_intercept(int signal);

namespace SimpleTestTemplates
{
	bool assert_nosig(void);

	template<typename T> bool assert_eq(const T &result, const T &expects)
	{
		if (!assert_nosig())
			return false;

		if (result != expects)
		{
			std::cout << MESSAGE_FORMAT << "\nAssertion failed." << std::endl;
			std::cout << MESSAGE_FORMAT << "The expected result was: " << ERROR_FORMAT << expects << MESSAGE_FORMAT "; but it got: " <<  ERROR_FORMAT << result << RESET_FORMAT << std::endl;
			return false;
		}
	
		return true;
	}

	template<typename T> bool assert_ne(const T &result, const T &nexpects)
	{
		if (!assert_nosig())
			return false;

		if (result == nexpects)
		{
			std::cout << MESSAGE_FORMAT << "\nAssertion failed." << std::endl;
			std::cout << MESSAGE_FORMAT << "The result could not be: " << ERROR_FORMAT << nexpects << RESET_FORMAT << std::endl;
			return false;
		}

		return true;
	}
}

namespace SimpleTestClasses
{
	class SimpleUnitTest
	{
	    private:
	        std::function<bool(void)> _test_case;
	        std::string _test_title;
	
			SimpleUnitTest(std::string test_title, std::function<bool(void)> test_case);
	        bool run(void);
	
		public:
			friend class SimpleTest;
	};
	
	class SimpleTest 
	{
	    private:
	        unsigned int _success_count;
	        unsigned int _error_count;
	        std::vector<SimpleUnitTest> _vec_unit_tests;
	
	    public:
			SimpleTest(void);
			void add_test(std::string test_title, std::function<bool(void)> test_case);
	        int run_tests(void);
	};
}

#endif
