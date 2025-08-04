#ifndef POS_H
#define POS_H

typedef enum
{
    POS_STT_IDLE,
    POS_STT_BEGIN_TRANS,
    POS_STT_READ_CARD,
    POS_STT_AUTH_SEND,
    POS_STT_AUTH_RECV,
    POS_STT_RECEIPT,
    POS_STT_QUIT,
} posstt_e;

#endif // POS_H
