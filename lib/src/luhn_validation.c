#include "luhn_validation.h"

#include <string.h>

#define PAN_LAST_DIGIT(pan) strlen(pan) - 1
#define PAYLOAD_LEN(pan) strlen(pan) - 2


luhn_validation_e luhn_validatepan(char *pan)
{
    unsigned char pan_check_digit = pan[PAN_LAST_DIGIT(pan)];
    unsigned char check_digit = '0';
    char arr[PAN_MAX_LEN] = { '\0' };
    int sum = 0;

    if (pan == NULL || strlen(pan) == 0)
        return LUHN_VALIDATION_NULL_PAN;

    for (int dg = PAYLOAD_LEN(pan); dg >= 0; --dg)
    {
        int doubling_res = 0;

        arr[dg] = pan[dg];
        --dg;

        if (dg < 0)
            break;

        doubling_res = (pan[dg] - '0') * 2;
        if (doubling_res > 9)
            doubling_res -= 9;

        arr[dg] = doubling_res + '0';
    }

    for (size_t arr_dg = 0; arr[arr_dg] != '\0'; ++arr_dg)
    {
        sum += arr[arr_dg] - '0';
    }

    check_digit += (10 - (sum % 10)) % 10;
    
    if (check_digit != pan_check_digit)
        return LUHN_VALIDATION_DIGIT_NOT_CHECK;
    return LUHN_VALIDATION_OK;
}
