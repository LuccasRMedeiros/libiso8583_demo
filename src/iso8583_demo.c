#include "iso8583_demo.h"

#include <stdio.h>
#include <string.h>


void iso8583_buildmsg(iso8583msg_st msgfields, size_t buf_len, char *buf_out)
{
    if (buf_len < 512)
        return ;

    char *msg = buf_out;

    memcpy(msg, msgfields.msgtype, MSGTYPE_LEN); msg + MSGTYPE_LEN;
    sprintf(msg, "%019d", msgfields.de002_pan); msg + DE002_LEN;

}
