#include "iso8583_demo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// From 1 to 8      ##12345678 *0
#define DE002_ON 0b01000000
#define DE003_ON 0b00100000
#define DE004_ON 0b00010000
#define DE007_ON 0b00000010
// From 9 to 16     ##90123456 *1
#define DE011_ON 0b00100000
#define DE012_ON 0b00010000
// From 17 to 24    ##78901234 *2
// From 25 to 32    ##56789012 *3
// From 33 to 40    ##34567890 *4
#define DE037_ON 0b00001000 
// From 41 to 48    ##12345678 *5
#define DE041_ON 0b10000000 
// From 49 to 56    ##90123456 *6
#define DE049_ON 0b10000000 
// From 57 to 64    ##78901234 *7 
// From 65 to 72    ##56789012 *8 
// From 73 to 80    ##34567890 *9 
// From 81 to 88    ##12345678 *10 
// From 89 to 96    ##90123456 *11
// From 97 to 104   ##78901234 *12 
// From 105 to 112  ##56789012 *13 
// From 113 to 120  ##34567890 *14 
// From 121 to 128  ##12345678 *15 
#define DE128_ON 0b00000001


iso8583msg_ret_e iso8583_buildmsg(iso8583msg_st msgfields, size_t buf_len, char *buf_out)
{
    if (buf_len < 512)
        return MSG_BUFFER_TOO_SHORT;

    char *msg = buf_out;
    byte *bitmask = (byte*)buf_out + MSGTYPE_LEN;

    if (strlen((char*)msgfields.msgtype) == 0)
        return MSG_TYPE_NOT_PRESENT;

    memcpy(msg, msgfields.msgtype, MSGTYPE_LEN); msg += MSGTYPE_LEN + BITMASK_LEN;

    if (msgfields.de002_pan > 0)
    {
        sprintf(msg, "%019d", msgfields.de002_pan); msg += DE002_LEN;
        bitmask[0] |= DE002_ON;
    }
    if (msgfields.de003_proccode > 0)
    {
        sprintf(msg, "%06d", msgfields.de003_proccode); msg += DE003_LEN;
        bitmask[0] |= DE003_ON;
    }
    if (msgfields.de004_amount > 0)
    {
        sprintf(msg, "%012d", msgfields.de004_amount); msg += DE004_LEN;
        bitmask[0] |= DE004_ON;
    }
    if (msgfields.de007_datetime > 0)
    {
        sprintf(msg, "%010d", msgfields.de007_datetime); msg += DE007_LEN;
        bitmask[0] |= DE007_ON;
    }
    if (msgfields.de011_stan > 0)
    {
        sprintf(msg, "%06d", msgfields.de011_stan); msg += DE011_LEN;
        bitmask[1] |= DE011_ON;
    }
    if (msgfields.de012_loctime > 0)
    {
        sprintf(msg, "%06d", msgfields.de012_loctime); msg += DE012_LEN;
        bitmask[1] |= DE012_ON;
    }
    if (strlen(msgfields.de037_refnumber) > 0)
    {
        memcpy(msg, msgfields.de037_refnumber, DE037_LEN); msg += DE037_LEN;
        bitmask[4] |= DE037_ON;
    }
    if (strlen(msgfields.de041_cardid) > 0)
    {
        memcpy(msg, msgfields.de041_cardid, DE041_LEN); msg += DE041_LEN;
        bitmask[5] |= DE041_ON;
    }
    if (strlen(msgfields.de049_currency) > 0)
    {
        memcpy(msg, msgfields.de049_currency, DE049_LEN); msg += DE049_LEN;
        bitmask[6] |= DE049_ON;
    }
    if (strlen(msgfields.de128_authcode) > 0)
    {
        memcpy(msg, msgfields.de128_authcode, DE128_LEN); msg += DE128_LEN;
        bitmask[15] |= DE128_ON;
    }

    return MSG_OK;
}

iso8583msg_ret_e iso8583_parsemsg(const char *msg, iso8583msg_st *out)
{
    if (msg == NULL)
        return MSG_CANNOT_BE_NULL;

    if (out == NULL)
        return MSG_STRUCT_OUT_CANNOT_BE_NULL;

    char buf[32] = { '\0' };
    char *parse = (char*)msg;
    byte *bitmask = (byte*)msg + MSGTYPE_LEN;

    memcpy(out->msgtype, parse, MSGTYPE_LEN); parse += MSGTYPE_LEN + BITMASK_LEN;

    if (bitmask[0] & DE002_ON)
    {
        out->de002_pan = atoi(strncpy(buf, parse, DE002_LEN));
        parse += DE002_LEN;
    }
    if (bitmask[0] & DE003_ON)
    {
        out->de003_proccode = atoi(strncpy(buf, parse, DE003_LEN));
        parse += DE003_LEN;
    }
    if (bitmask[0] & DE004_ON)
    {
        out->de004_amount = atoi(strncpy(buf, parse, DE004_LEN));
        parse += DE004_LEN;
    }
    if (bitmask[0] & DE007_ON)
    {
        out->de007_datetime = atoi(strncpy(buf, parse, DE007_LEN));
        parse += DE007_LEN;
    }
    if (bitmask[1] & DE011_LEN)
    {
        out->de011_stan = atoi(strncpy(buf, parse, DE011_LEN));
        parse += DE011_LEN;
    }
    if (bitmask[1] & DE012_LEN)
    {
        out->de012_loctime = atoi(strncpy(buf, parse, DE012_LEN));
        parse += DE012_LEN;
    }
    if (bitmask[4] & DE037_ON)
    {
        memcpy(out->de037_refnumber, parse, DE037_LEN);
        parse += DE037_LEN;
    }
    if (bitmask[4] & DE041_ON)
    {
        memcpy(out->de041_cardid, parse, DE041_LEN);
        parse += DE041_LEN;
    }
    if (bitmask[6] & DE049_ON)
    {
        memcpy(out->de049_currency, parse, DE049_LEN);
        parse += DE049_LEN;
    }
    if (bitmask[15] & DE128_ON)
    {
        memcpy(out->de128_authcode, parse, DE128_ON);
        parse += DE128_ON;
    }

    return MSG_OK;
}
