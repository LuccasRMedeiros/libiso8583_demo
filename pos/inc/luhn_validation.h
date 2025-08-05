#ifndef LUHN_VALIDATION_H
#define LUHN_VALIDATION_H

#define PAN_MAX_LEN 19

typedef enum
{
    LUHN_VALIDATION_OK,
    LUHN_VALIDATION_NULL_PAN,
    LUHN_VALIDATION_DIGIT_NOT_CHECK,
} luhn_validation_e;

luhn_validation_e luhn_validatepan(char *pan);

#endif
