#include <iso8583_demo.h>   
#include <stdio.h>

#define CONTINUE    1
#define STOP        0


int readcard(iso8583msg_st *out)
{
}

int setamount(char *stramount, iso8583msg_st *out)
{
    int ret = 0;
    int amount = atoi(stramount);

    if (amount <= 0)
        return -1;

    out->de004_amount = amount;

    return ret;
}

int main(void)
{
    int flag_exec = CONTINUE;
    iso8583msg_st transinfo;
    char buf[19] = { '\0' };

    while(flag_exec == CONTINUE)
    {
        printf("DEMO POS, DIGITE O VALOR DA COMPRA PARA COMEÇAR\n");
        fgets(buf, sizeof (buf), stdin);

        setamount(buf, &transinfo);
        printf("R$%f\n\n", (float)(transinfo.amount / 10));
        printf("1. Debito\n2. Crédito\n");
        fgets(buf, sizeof (buf), stdin);
    }


    return 0;
}
