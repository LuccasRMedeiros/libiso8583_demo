#ifndef __UNIT_TEST_AUX_HPP__
#define __UNIT_TEST_AUX_HPP__

#include <string>

#define TITLE_FORMAT    "\e[1;34m"
#define SUCCESS_FORMAT  "\e[1;32m"
#define ERROR_FORMAT    "\e[1;31m"
#define MESSAGE_FORMAT	"\e[1;33m"
#define RESET_FORMAT    "\e[0m"

void sig_intercept(int signal);
std::string byte_to_hex(unsigned char *buffer, size_t len);

#endif // __UNIT_TEST_AUX_HPP__
