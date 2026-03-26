#include "unit_test_aux.hpp"
#include <iostream>
#include <signal.h>

void sig_intercept(int signal)
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
	case SIGINT:
        std::cout << ERROR_FORMAT << "SIGINT" << RESET_FORMAT << std::endl;
		break;
	case SIGTERM:
        std::cout << ERROR_FORMAT << "SIGTERM" << RESET_FORMAT << std::endl;
		break;
	default:
		std::cout << ERROR_FORMAT << "(" << signal << ") Finishing test execution" << RESET_FORMAT << std::endl;
    }
	
	exit(signal);
}

std::string byte_to_hex(unsigned char *buffer, size_t len)
{
	// Will transform each byte to its hexadecimal reference
	std::string ret = std::string(len * 2, '\0');

	for (size_t i = 0; i < len; ++i)
	{
		unsigned char byte = buffer[i];

		for (int ri = 1; ri >= 0; --ri)
		{
			char hdchr = byte % 16;

			if (hdchr > 9)
				hdchr += 55;
			else
				hdchr += 48;

			byte /= 16;
			ret[ri + (i * 2)] = hdchr;
		}
	}

	return ret;
}
