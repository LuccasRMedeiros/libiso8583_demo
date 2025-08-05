#ifndef STT_MACHINE_H
#define STT_MACHINE_H

#define STT_MACHINE_MAX 6

typedef enum
{
    STT_IDLE,
    STT_READ_CARD,
    STT_ISO_SEND,
    STT_WAIT_RESP,
    STT_RECEIPT,    // Estados extras para demonstração
    STT_QUIT,       // 
} stt_machine_e;

typedef void (*stt_machine_callback_fn)(void);

typedef enum
{
    STT_CBK_OK = 0,
    STT_CBK_REDEFINITION = 1,
    STT_CBK_INVALID_CALLBACK = -1,
    STT_CBK_INVALID_STATE = -2,
} stt_machine_cbk_e;

stt_machine_cbk_e stt_machine_initcallback(
        stt_machine_callback_fn cbk,
        stt_machine_e sttin
        );
int stt_machine(stt_machine_e *state);

#endif
