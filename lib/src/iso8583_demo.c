#include "iso8583_demo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// These are for internal use
// From 1 to 8      ##12345678 *0
#define BITMAP_1 0b10000000 // Tells if the secondary bitmap is on
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

iso8583msg_ret_e iso8583_buildmsg(
        iso8583msg_st msgfields,
        size_t buf_len,
        unsigned char *buf_out,
		size_t *out_len
        )
{
	unsigned char *msgtype_ptr;
	unsigned char *bitmask_ptr;
	unsigned char *msg_short_ptr;
	unsigned char *msg_long_ptr;
	size_t msg_len = 0;

	if (buf_len < 512)
		return MSG_BUFFER_TOO_SHORT;

	if (buf_out == NULL)
		return MSG_CANNOT_BE_NULL;

	if (out_len == NULL)
		return MSG_OUT_LEN_CANNOT_BE_NULL;

	if (strlen((char*)msgfields.msgtype) == 0)
		return MSG_TYPE_NOT_PRESENT;

	memset(buf_out, 0, buf_len);
	msgtype_ptr = buf_out;
    bitmask_ptr = buf_out + MSGTYPE_LEN;
    msg_short_ptr = buf_out + MSGTYPE_LEN + BITMASK_SHORT_LEN;
	msg_long_ptr = buf_out + MSGTYPE_LEN + BITMASK_LONG_LEN;

	memcpy(msgtype_ptr, msgfields.msgtype, MSGTYPE_LEN);

    if (strlen(msgfields.de002_pan) > 0)
    {
        memcpy(msg_short_ptr + msg_len, msgfields.de002_pan, DE002_LEN);
        bitmask_ptr[0] |= DE002_ON;
		msg_len += DE002_LEN;
    }
    if (strlen(msgfields.de003_proccode) > 0)
    {
        memcpy(msg_short_ptr + msg_len, msgfields.de003_proccode, DE003_LEN);
        bitmask_ptr[0] |= DE003_ON;
		msg_len += DE003_LEN;
    }
    if (strlen(msgfields.de004_amount) > 0)
    {
        memcpy(msg_short_ptr + msg_len, msgfields.de004_amount, DE004_LEN);
        bitmask_ptr[0] |= DE004_ON;
		msg_len += DE004_LEN;
    }
    if (strlen(msgfields.de007_datetime) > 0)
    {
        memcpy(msg_short_ptr + msg_len, msgfields.de007_datetime, DE007_LEN);
        bitmask_ptr[0] |= DE007_ON;
		msg_len += DE007_LEN;
    }
    if (strlen(msgfields.de011_stan) > 0)
    {
        memcpy(msg_short_ptr + msg_len, msgfields.de011_stan, DE011_LEN);
        bitmask_ptr[1] |= DE011_ON;
		msg_len += DE011_LEN;
    }
    if (strlen(msgfields.de012_loctime) > 0)
    {
        memcpy(msg_short_ptr + msg_len, msgfields.de012_loctime, DE012_LEN);
        bitmask_ptr[1] |= DE012_ON;
		msg_len += DE012_LEN;
    }
    if (strlen(msgfields.de037_refnumber) > 0)
    {
        memcpy(msg_short_ptr + msg_len, msgfields.de037_refnumber, DE037_LEN);
        bitmask_ptr[4] |= DE037_ON;
		msg_len += DE037_LEN;
    }
    if (strlen(msgfields.de041_cardid) > 0)
    {
        memcpy(msg_short_ptr + msg_len, msgfields.de041_cardid, DE041_LEN);
        bitmask_ptr[5] |= DE041_ON;
		msg_len += DE041_LEN;
    }
    if (strlen(msgfields.de049_currency) > 0)
    {
        memcpy(msg_short_ptr + msg_len, msgfields.de049_currency, DE049_LEN);
        bitmask_ptr[6] |= DE049_ON;
		msg_len += DE049_LEN;
    }
    if (strlen(msgfields.de128_authcode) > 0)
    {
		memmove(msg_long_ptr, msg_short_ptr, msg_len);
		memset(bitmask_ptr + BITMASK_SHORT_LEN, 0, BITMASK_SHORT_LEN);
        memcpy(msg_long_ptr + msg_len, msgfields.de128_authcode, DE128_LEN);
		bitmask_ptr[0] |= BITMAP_1;
        bitmask_ptr[15] |= DE128_ON;
		msg_len += DE128_LEN + BITMASK_LONG_LEN;
    }
	else
		msg_len += BITMASK_SHORT_LEN;

	msg_len += MSGTYPE_LEN;
	*out_len = msg_len;

    return MSG_OK;
}

iso8583msg_ret_e iso8583_parsemsg(const char *msg, iso8583msg_st *out)
{
    if (msg == NULL)
        return MSG_CANNOT_BE_NULL;

    if (out == NULL)
        return MSG_STRUCT_OUT_CANNOT_BE_NULL;

    char *parse = (char*)msg;
    char *bitmask = parse + MSGTYPE_LEN;

    memcpy(out->msgtype, parse, MSGTYPE_LEN); parse += MSGTYPE_LEN + BITMASK_SHORT_LEN;

    if (bitmask[0] & DE002_ON)
    {
        strncpy(out->de002_pan, parse, DE002_LEN);
        parse += DE002_LEN;
    }
    if (bitmask[0] & DE003_ON)
    {
        strncpy(out->de003_proccode, parse, DE003_LEN);
        parse += DE003_LEN;
    }
    if (bitmask[0] & DE004_ON)
    {
        strncpy(out->de004_amount, parse, DE004_LEN);
        parse += DE004_LEN;
    }
    if (bitmask[0] & DE007_ON)
    {
        strncpy(out->de007_datetime, parse, DE007_LEN);
        parse += DE007_LEN;
    }
    if (bitmask[1] & DE011_LEN)
    {
        strncpy(out->de011_stan, parse, DE011_LEN);
        parse += DE011_LEN;
    }
    if (bitmask[1] & DE012_LEN)
    {
        strncpy(out->de012_loctime, parse, DE012_LEN);
        parse += DE012_LEN;
    }
    if (bitmask[4] & DE037_ON)
    {
        strncpy(out->de037_refnumber, parse, DE037_LEN);
        parse += DE037_LEN;
    }
    if (bitmask[4] & DE041_ON)
    {
        strncpy(out->de041_cardid, parse, DE041_LEN);
        parse += DE041_LEN;
    }
    if (bitmask[6] & DE049_ON)
    {
        strncpy(out->de049_currency, parse, DE049_LEN);
        parse += DE049_LEN;
    }
    if (bitmask[15] & DE128_ON)
    {
        strncpy(out->de128_authcode, parse, DE128_ON);
        parse += DE128_ON;
    }

    return MSG_OK;
}
