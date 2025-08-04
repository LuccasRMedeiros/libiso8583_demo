#ifndef STT_MACHINE_H
#define STT_MACHINE_H

typedef enum
{
    STT_IDLE,
    STT_BEGIN_TRANS,
    STT_READ_CARD,
    STT_AUTH_SEND,
    STT_AUTH_RECV,
    STT_RECEIPT,
    STT_QUIT,
} stt_machine_e;

int stt_machine(void);

#endif
