#ifndef ISO8583_DEMO_H
#define ISO8583_DEMO_H

#include <stddef.h>
#include <stdint.h>

#define MSGTYPE_LEN 4
#define BITMASK_LEN 16

#define DE002_LEN   19
#define DE003_LEN   6
#define DE004_LEN   12
#define DE007_LEN   10
#define DE011_LEN   6
#define DE012_LEN   6
#define DE037_LEN   12 
#define DE041_LEN   8 
#define DE049_LEN   3
#define DE128_LEN   64

typedef enum
{
    MSG_OK = 0,
    MSG_BUFFER_TOO_SHORT,
    MSG_TYPE_NOT_PRESENT,
    MSG_CANNOT_BE_NULL,
    MSG_STRUCT_OUT_CANNOT_BE_NULL,
} iso8583msg_ret_e;

typedef struct
{
    char    msgtype[MSGTYPE_LEN+1];
    char    de002_pan[DE002_LEN+1];
    char    de003_proccode[DE003_LEN+1];
    char    de004_amount[DE004_LEN+1];
    char    de007_datetime[DE007_LEN+1];
    char    de011_stan[DE011_LEN+1];
    char    de012_loctime[DE012_LEN+1];
    char    de037_refnumber[DE037_LEN+1];
    char    de041_cardid[DE041_LEN+1];
    char    de049_currency[DE049_LEN+1];
    char    de128_authcode[DE128_LEN+1];
} iso8583msg_st;

iso8583msg_ret_e iso8583_buildmsg(iso8583msg_st msgfields, size_t buf_len, char *buf_out);
iso8583msg_ret_e iso8583_parsemsg(const char *msg, iso8583msg_st *out);

#endif
