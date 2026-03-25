#include "unit_test_class.hpp"
#include <cstring>

extern "C"
{
#include "iso8583_demo.h"
#include "luhn_validation.h"
#include "stt_machine.h"
#include "memory.h"
#include <stdio.h>
}

using namespace std;
using namespace SimpleTestTemplates;
using namespace SimpleTestClasses;

void initialize_tests(SimpleTest &test_libiso8583_demo)
{
    test_libiso8583_demo.add_test(
            "Test if memory_init initializes memory without failure",
            []() -> bool {
                memory_init();
				return assert_nosig();
            });

	test_libiso8583_demo.add_test(
			"Test if memory_read returns -1 when trying to read recently initiated memory",
			[]() -> bool {
				memory_dir_st *files;

				return assert_eq<int>(memory_read(&files), -1);
			});

	test_libiso8583_demo.add_test(
			"Test if memory_show_info shows recently initiated disk info without breaking the program",
			[]() -> bool {
				std::cout << RESET_FORMAT << std::endl;
				memory_show_info();
				return assert_nosig();
			});

    test_libiso8583_demo.add_test(
            "Test if memory_write write a file successfully",
            []() -> bool {
                char filename[] = "test.txt";
                char data[] = "Hello World!";

				return assert_eq<int>(memory_write(filename, data, 12), 0);
            });

	test_libiso8583_demo.add_test(
			"Test if memory_write returns -1 when receiving NULL parameters",
			[]() -> bool {
				return assert_eq<int>(memory_write(NULL, NULL, 0), -1);
			});

	test_libiso8583_demo.add_test(
			"Test if memory_write is safe to use when trying to write bigger data than the informed number of bytes",
			[]() -> bool {
				char filename[] = "test2.txt";
				char data[] = "Hello World! I am not that creative!";

				return assert_eq<int>(memory_write(filename, data, 12), 0);
			});

    test_libiso8583_demo.add_test(
            "Test if memory_read have a record of the file test.txt",
            []() -> bool {
                memory_dir_st *files;
				string filename;

                if (!assert_eq<int>(memory_read(&files), 0))
                    return false;

				filename = string(reinterpret_cast<const char*>(files[0].filename), 8);
				return assert_eq<string>(filename, "test.txt");
            });

    test_libiso8583_demo.add_test(
			"Test if memory_get_str_filename create a valid null terminated string from a array of bytes",
			[]() -> bool {
                char dir_filename[FILENAME_SIZE+1] = { '\0' };
                byte test_str[FILENAME_SIZE] = { 0xff };
                
                memcpy(test_str, "foobar", 6);

                if (!assert_eq<int>(memory_get_str_filename(test_str, dir_filename), 0))
                    return false;

                if (!assert_eq<string>(dir_filename, "foobar"))
                    return false;

                return true;
            });

	test_libiso8583_demo.add_test(
			"Test if memory_get_str_filename returns -1 when parameters are NULL",
			[]() -> bool {
				return assert_eq(memory_get_str_filename(NULL, NULL), -1);
			});

    test_libiso8583_demo.add_test(
            "Test if memory_read_file reads the content of \"test.txt\"",
            []() -> bool {
                byte *dy_file_content = NULL;
				string dy_file_string;
                
                if (!assert_eq<int>(memory_read_file("test.txt", &dy_file_content), 0))
                {
                    if (dy_file_content != NULL)
                        free(dy_file_content);

                    return false;
                }
	
				dy_file_string = string(reinterpret_cast<const char*>(dy_file_content), 12);
                if (!assert_eq<string>(dy_file_string, "Hello World!"))
                {
                    if (dy_file_content != NULL)
                        free(dy_file_content);

                    return false;
                }

                free(dy_file_content);
                return true;
            });

	test_libiso8583_demo.add_test(
			"Test if memory_show_info lists all information from disk without breaking the program",
			[]() -> bool {
				std::cout << RESET_FORMAT << std::endl;
				memory_show_info();
				return assert_nosig();
			});

    test_libiso8583_demo.add_test(
            "Test if memory_erase_block erases block",
            []() -> bool {
                memory_dir_st *files;

                if (!assert_eq<int>(memory_erase_block(1), 0))
                    return false;

                memory_read(&files);
                if (!assert_nosig())
                {
                    rec_signal = 0;
                    return false;
                }

                if (!assert_eq<int>(files[0].filename[0], 0xff))
                    return false;
                return true;
            });

	test_libiso8583_demo.add_test(
			"Test if memory_show_info still lists all information about disk without breaking the program",
			[]() -> bool {
				std::cout << RESET_FORMAT << std::endl;
				memory_show_info();
				return assert_nosig();
			});

	test_libiso8583_demo.add_test(
			"Test if iso8583_buildmsg builds a message accordingly to specifications",
			[]() -> bool {
				iso8583msg_st msg;
				char msgbuffer[512];
				string msgstr;
				
				strcpy(msg.msgtype, "0200");
				strcpy(msg.de002_pan, "4111111111111111");
				strcpy(msg.de003_proccode, "000000");
				strcpy(msg.de004_amount, "000000000100");
				strcpy(msg.de007_datetime, "1230102030");
				strcpy(msg.de011_stan, "123456");
				strcpy(msg.de012_loctime, "102030");
				strcpy(msg.de037_refnumber, "ABC123456789");
				strcpy(msg.de041_cardid, "TERM01");
				strcpy(msg.de049_currency, "840");
				strcpy(msg.de128_authcode, "008TESTDATA");
				iso8583_buildmsg(msg, sizeof (msgbuffer), msgbuffer);
				msgstr = string(msgbuffer);
				return assert_eq<string>(msgstr, "30323030F23000000880800000000000000131363431313131313131313131313131313130303030303030303030303030303030313030313233303130323033303132333435363130323033304142433132333435363738395445524D3020203834303030385445535444415441");
			});

	test_libiso8583_demo.add_test(
			"Test if iso8583_buildmsg returns \"MSG_BUFFER_TOO_SHORT\"(1) when size of buffer is to short to fit a message",
			[]() -> bool {
				iso8583msg_st msg;
				char msgbuffer[12];
				
				strcpy(msg.msgtype, "0200");
				strcpy(msg.de002_pan, "4111111111111111");
				strcpy(msg.de003_proccode, "000000");
				strcpy(msg.de004_amount, "000000000100");
				strcpy(msg.de007_datetime, "1230102030");
				strcpy(msg.de011_stan, "123456");
				strcpy(msg.de012_loctime, "102030");
				strcpy(msg.de037_refnumber, "ABC123456789");
				strcpy(msg.de041_cardid, "TERM01");
				strcpy(msg.de049_currency, "840");
				strcpy(msg.de128_authcode, "008TESTDATA");
				return assert_eq<iso8583msg_ret_e>(iso8583_buildmsg(msg, sizeof (msgbuffer), msgbuffer), MSG_BUFFER_TOO_SHORT);
			});

	test_libiso8583_demo.add_test(
			"Test if iso8583_buildmsg returns \"MSG_TYPE_NOT_PRESENT\"(2) when message type is not passed with the message structure",
			[]() -> bool {
				iso8583msg_st msg;
				char msgbuffer[512];
				
				memset(msg.msgtype, 0, MSGTYPE_LEN);
				strcpy(msg.de002_pan, "4111111111111111");
				strcpy(msg.de003_proccode, "000000");
				strcpy(msg.de004_amount, "000000000100");
				strcpy(msg.de007_datetime, "1230102030");
				strcpy(msg.de011_stan, "123456");
				strcpy(msg.de012_loctime, "102030");
				strcpy(msg.de037_refnumber, "ABC123456789");
				strcpy(msg.de041_cardid, "TERM01");
				strcpy(msg.de049_currency, "840");
				strcpy(msg.de128_authcode, "008TESTDATA");
				return assert_eq<iso8583msg_ret_e>(iso8583_buildmsg(msg, sizeof (msgbuffer), msgbuffer), MSG_TYPE_NOT_PRESENT);
			});

	test_libiso8583_demo.add_test(
			"Test if iso8583_buildmsg returns \"MSG_CANNOT_BE_NULL\"(3) when message without contents are passed",
			[]() -> bool {
				iso8583msg_st msg;
				char msgbuffer[512];
				
				memset(&msg, 0, sizeof (msg));
				return assert_eq<iso8583msg_ret_e>(iso8583_buildmsg(msg, sizeof (msgbuffer), msgbuffer), MSG_CANNOT_BE_NULL);
			});

	test_libiso8583_demo.add_test(
			"Test if iso8583_parsemsg parses the message accordingly to specifications",
			[]() -> bool {
				bool ret = true;
				iso8583msg_st parsemsg;
				char msg[] = "30323030F23000000880800000000000000131363431313131313131313131313131313130303030303030303030303030303030313030313233303130323033303132333435363130323033304142433132333435363738395445524D3020203834303030385445535444415441";

				iso8583_parsemsg(msg, &parsemsg);

				// The following ternary operations keep "ret" true until one assertion fails
				ret = assert_eq<string>(string(parsemsg.msgtype), "0200") && ret == true ? true : false;
				ret = assert_eq<string>(string(parsemsg.de002_pan), "4111111111111111") && ret == true ? true : false;
				ret = assert_eq<string>(string(parsemsg.de003_proccode), "000000") && ret == true ? true : false;
				ret = assert_eq<string>(string(parsemsg.de004_amount), "000000000100") && ret == true ? true : false;
				ret = assert_eq<string>(string(parsemsg.de007_datetime), "1230102030") && ret == true ? true : false;
				ret = assert_eq<string>(string(parsemsg.de011_stan), "123456") && ret == true ? true : false;
				ret = assert_eq<string>(string(parsemsg.de012_loctime), "102030") && ret == true ? true : false;
				ret = assert_eq<string>(string(parsemsg.de037_refnumber), "ABC123456789") && ret == true ? true : false;
				ret = assert_eq<string>(string(parsemsg.de041_cardid), "TERM01") && ret == true ? true : false;
				ret = assert_eq<string>(string(parsemsg.de049_currency), "840") && ret == true ? true : false;
				ret = assert_eq<string>(string(parsemsg.de128_authcode), "008TESTDATA") && ret == true ? true : false;
				return ret;
			});

	test_libiso8583_demo.add_test(
			"Test if iso8583_parsemsg returns \"MSG_CANNOT_BE_NULL\"(3) when null is passed as the first argument",
			[]() -> bool {
				iso8583msg_st out;

				return assert_eq<iso8583msg_ret_e>(iso8583_parsemsg(NULL, &out), MSG_CANNOT_BE_NULL);
			});

	test_libiso8583_demo.add_test(
			"Test if iso8583_parsemsg returns \"MSG_STRUCT_OUT_CANNOT_BE_NULL\"(4) when null is passed as the first argument",
			[]() -> bool {
				char msg[] = "30323030F23000000880800000000000000131363431313131313131313131313131313130303030303030303030303030303030313030313233303130323033303132333435363130323033304142433132333435363738395445524D3020203834303030385445535444415441";

				return assert_eq<iso8583msg_ret_e>(iso8583_parsemsg(msg, NULL), MSG_STRUCT_OUT_CANNOT_BE_NULL);
			});

	test_libiso8583_demo.add_test(
			"Test if luhn_validatepan validates valid pan",
			[]() -> bool {
				return assert_eq<luhn_validation_e>(luhn_validatepan("5424083933985817"), LUHN_VALIDATION_OK);
			});

	test_libiso8583_demo.add_test(
			"Test if luhn_validatepan invalidates invalid pan",
			[]() -> bool {
				return assert_eq<luhn_validation_e>(luhn_validatepan("1234123412341234"), LUHN_VALIDATION_DIGIT_NOT_CHECK);
			});

	test_libiso8583_demo.add_test(
			"Test if luhn_validatepan returns \"LUHN_VALIDATION_NULL_PAN\"(1) when null string is informed",
			[]() -> bool {
				return assert_eq<luhn_validation_e>(luhn_validatepan(NULL), LUHN_VALIDATION_NULL_PAN);
			});
}

int main(void)
{
	SimpleTest test_libiso8583_demo = SimpleTest();

    initialize_tests(test_libiso8583_demo);
    return test_libiso8583_demo.run_tests();
}
