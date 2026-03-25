#include "luhn_validation.h"
#include <string.h>

luhn_validation_e luhn_validatepan(char *pan)
{
    unsigned int pan_check_digit;
    unsigned int check_digit;
    unsigned int arr[PAN_MAX_LEN] = { 0 };
    int sum = 0;

	if (pan == NULL || strlen(pan) == 0)
		return LUHN_VALIDATION_NULL_PAN;

	pan_check_digit = pan[strlen(pan) - 1] - '0';

    for (int dg = strlen(pan) - 2; dg >= 0; --dg)
    {
        arr[dg] = (pan[dg] - '0');
		
		if (dg % 2 == 0)
		{
			arr[dg] *= 2;
			
			if (arr[dg] > 9)
				arr[dg] -= 9;
		}
    }

    for (size_t arr_dg = 0; arr[arr_dg] != '\0'; ++arr_dg)
    {
        sum += arr[arr_dg];
    }

    check_digit = (10 - (sum % 10)) % 10;
    
    if (check_digit != pan_check_digit)
        return LUHN_VALIDATION_DIGIT_NOT_CHECK;
    return LUHN_VALIDATION_OK;
}
