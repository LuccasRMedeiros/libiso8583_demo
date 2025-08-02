#ifndef ISO8583_DEMO_H
#define ISO8583_DEMO_H

#define MSGTYPE_LEN 4
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

typedef struct
{
    unsigned char   msgtype[MSGTYPE_LEN+1];
    unsigned int    de002_pan;
    unsigned int    de003_proccode;
    unsigned int    de004_amount;
    unsigned int    de007_datetime;
    unsigned int    de011_stan;
    unsigned int    de012_loctime;
    unsigned char   de037_refnumber[DE037_LEN+1];
    unsigned char   de041_cardid[DE041_LEN+1];
    unsigned char   de049_currency[DE049_LEN+1];
    unsigned char   de128_authcode[DE128_LEN+1];
} iso8583msg_st;

#endif
