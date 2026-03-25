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

/**
 * This function presumes that "out" does have at least two bytes of space and 
 * is correctly aligned for the user intention of use. It need to be used with
 * caution as it may lead to UB
 */
static void ascii_to_asciihex(char chr, char *out)
{
	int asciiint = chr;

	if (out == NULL)
		return;

	strcpy(out, "00");

	for (int ri = 1; ri >= 0; --ri)
	{
		char sbyte = asciiint % 16;

		if (sbyte > 10)
			sbyte += 55;
		else
			sbyte += 48;

		asciiint /= 16;
		out[ri] = sbyte;
	}
}

static void cpy_as_hex(const char *src, char *dest, size_t dest_len)
{
	if (src == NULL || strlen(src) == 0 || dest == NULL)
		return;

	for (size_t src_i = 0; src_i < strlen(src) && src_i * 2 < dest_len; ++src_i)
		ascii_to_asciihex(src[src_i], dest + (src_i * 2));
}

iso8583msg_ret_e iso8583_buildmsg(
        iso8583msg_st msgfields,
        size_t buf_len,
        char *buf_out
        )
{
	char *msgtype_ptr;
	char *bitmask_ptr;
	char *msg_ptr;

	if (buf_len < 512)
		return MSG_BUFFER_TOO_SHORT;

	memset(buf_out, 0, buf_len);
	msgtype_ptr = buf_out;
    bitmask_ptr = buf_out + MSGTYPE_LEN * 2;
    msg_ptr = buf_out + MSGTYPE_LEN * 2 + BITMASK_LEN;

	if (strlen((char*)msgfields.msgtype) == 0)
		return MSG_TYPE_NOT_PRESENT;

	cpy_as_hex(msgfields.msgtype, msgtype_ptr, buf_len);

    if (strlen(msgfields.de002_pan) > 0)
    {
        strcpy(msg_ptr, msgfields.de002_pan); msg_ptr += DE002_LEN * 2;
        bitmask_ptr[0] |= DE002_ON;
    }
    if (strlen(msgfields.de003_proccode) > 0)
    {
        strcpy(msg_ptr, msgfields.de003_proccode); msg_ptr += DE003_LEN * 2;
        bitmask_ptr[0] |= DE003_ON;
    }
    if (strlen(msgfields.de004_amount) > 0)
    {
        strcpy(msg_ptr, msgfields.de004_amount); msg_ptr += DE004_LEN * 2;
        bitmask_ptr[0] |= DE004_ON;
    }
    if (strlen(msgfields.de007_datetime) > 0)
    {
        strcpy(msg_ptr, msgfields.de007_datetime); msg_ptr += DE007_LEN * 2;
        bitmask_ptr[0] |= DE007_ON;
    }
    if (strlen(msgfields.de011_stan) > 0)
    {
        strcpy(msg_ptr, msgfields.de011_stan); msg_ptr += DE011_LEN * 2;
        bitmask_ptr[1] |= DE011_ON;
    }
    if (strlen(msgfields.de012_loctime) > 0)
    {
        strcpy(msg_ptr, msgfields.de012_loctime); msg_ptr += DE012_LEN * 2;
        bitmask_ptr[1] |= DE012_ON;
    }
    if (strlen(msgfields.de037_refnumber) > 0)
    {
        strcpy(msg_ptr, msgfields.de037_refnumber); msg_ptr += DE037_LEN * 2;
        bitmask_ptr[4] |= DE037_ON;
    }
    if (strlen(msgfields.de041_cardid) > 0)
    {
        strcpy(msg_ptr, msgfields.de041_cardid); msg_ptr += DE041_LEN * 2;
        bitmask_ptr[5] |= DE041_ON;
    }
    if (strlen(msgfields.de049_currency) > 0)
    {
        strcpy(msg_ptr, msgfields.de049_currency); msg_ptr += DE049_LEN * 2;
        bitmask_ptr[6] |= DE049_ON;
    }
    if (strlen(msgfields.de128_authcode) > 0)
    {
        strcpy(msg_ptr, msgfields.de128_authcode); msg_ptr += DE128_LEN * 2;
        bitmask_ptr[15] |= DE128_ON;
    }

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

    memcpy(out->msgtype, parse, MSGTYPE_LEN); parse += MSGTYPE_LEN + BITMASK_LEN;

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
