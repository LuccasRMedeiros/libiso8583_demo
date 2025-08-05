#include <iso8583_demo.h>
#include <stt_machine.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CONTINUE    1
#define STOP        0


int main(void)
{
    int flag_exec = CONTINUE;
    stt_machine_e state = STT_IDLE;
    
    while(flag_exec == CONTINUE)
    {
        stt_machine(&state);

        if (state == STT_QUIT)
            flag_exec = STOP;
    }

    return 0;
}
