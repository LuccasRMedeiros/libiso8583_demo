#include "stt_machine.h"
#include "iso8583_demo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CONTINUE    1
#define STOP        0

#define MILLISEC_TO_NANOSEC(ms) ms * 1000000

static stt_machine_callback_fn callbacks[STT_MACHINE_MAX] = { NULL };
static char posid[DE041_LEN+1] = "POS12345";

static void completefields(iso8583msg_st *out)
{
    time_t loctime;
    struct tm *loctime_ret;
    int transstan = 0;

    loctime = time(NULL);
    loctime_ret = localtime(&loctime);
    transstan = rand() % 999999;

    strftime(out->de007_datetime, DE007_LEN, "%m%d%H%M%S", loctime_ret);
    snprintf(out->de011_stan, DE011_LEN, "%u", transstan);
    strftime(out->de012_loctime, DE012_LEN, "%H%M%S", loctime_ret);
    strcpy(out->de041_cardid, posid);
    strcpy(out->de049_currency, "986");
    snprintf(
            out->de128_authcode,
            DE128_LEN,
            "%s%s%s",
            out->de002_pan,
            out->de003_proccode,
            out->de004_amount);
}

static int readcard(char *mod, iso8583msg_st *out)
{
    struct timespec slptime; 

    slptime.tv_sec = 1;
    slptime.tv_nsec = 0;

    printf("Aguarde, realizando a leitura...\n");
    printf("\e[41m "); fflush(stdout); nanosleep(&slptime, NULL);
    printf("\e[43m "); fflush(stdout); nanosleep(&slptime, NULL);
    printf("\e[42m "); fflush(stdout); nanosleep(&slptime, NULL);
    printf("\e[44m \e[0m\n");
    nanosleep(&slptime, NULL);
    printf("Leitura concluída\n");

    strcpy(out->de002_pan, "5186001700008785");
    strcpy(out->de003_proccode, "00");
    strcpy(out->de003_proccode + 2, mod);
    strcpy(out->de003_proccode + 4, mod);

    return 0;
}

static int setamount(char *stramount, iso8583msg_st *out)
{
    int amount = atoi(stramount);

    sprintf(out->de004_amount, "%u", amount);

    return amount;
}

stt_machine_cbk_e stt_machine_initcallback(
        stt_machine_callback_fn cbk,
        stt_machine_e sttin
        )
{
    stt_machine_cbk_e ret = STT_CBK_OK;

    if (cbk == NULL)
        return STT_CBK_INVALID_CALLBACK;

    if (sttin >= STT_MACHINE_MAX)
        return STT_CBK_INVALID_STATE;

    if (callbacks[sttin] != NULL)
        ret = STT_CBK_REDEFINITION;

    callbacks[sttin] = cbk;

    return ret;
}

int stt_machine(stt_machine_e *state_io)
{
    char buf[19+1] = { '\0' };
    char mod[2+1] = { '\0' };
    char modstr[9+1] = { '\0' };
    char clock[5+1] = { '\0' };
    char msg[512+1] = { '\0' };
    int amount = 0;
    iso8583msg_st transinfo;
    time_t loctime;
    struct tm *loctime_ret;

    switch (*state_io)
    {
    case STT_IDLE:
        // TODO: Mudar para função callback
        memset(buf, 0, sizeof (buf));
        loctime = time(NULL);
        loctime_ret = localtime(&loctime);
        strftime(clock, sizeof (clock), "%H:%M", loctime_ret);
        printf("\e[47m\e[30m   %s                       4G⠠⠾   \e[0m\n\n\n", clock);
        printf("             DEMO POS                \n");
        printf("DIGITE O VALOR DA COMPRA PARA COMEÇAR\n");
        printf("        \"Sair\" PARA ENCERRAR         \n");
        fgets(buf, sizeof (buf), stdin);

        if (strncmp(buf, "Sair", 4) == 0)
        {
            *state_io = STT_QUIT;
            break;
        }

        if ((amount = setamount(buf, &transinfo)) == 0)
        {
            printf("\nO VALOR PRECISA SER MAIOR QUE 0\n\n");
            break;
        }

        printf("\nSelecione a forma de pagamento:\n[1]. Debito\n2. Crédito\n");
        fgets(buf, sizeof (buf), stdin);

        if (buf[0] != '2')
        {
            strcpy(mod, "10");
            strcpy(modstr, "Débito");
            printf("\nDébito selecionado\n");
        }
        else // buf[0] == '2'
        {
            strcpy(mod, "20");
            strcpy(modstr, "Crédito");
            printf("\nCrédito selecionado\n");
        }

        *state_io = STT_READ_CARD;
        break;

    case STT_READ_CARD:
        printf("\nR$%.2f %s\n\n", (float)(amount / 100), mod);
        printf("Aproxime ou insira o cartão\n");
        readcard(mod, &transinfo);

        *state_io = STT_ISO_SEND;

        break;

    case STT_ISO_SEND:
        printf("Processando...\n");
        completefields(&transinfo);
        iso8583_buildmsg(transinfo, sizeof (msg), msg);
        printf("MESSAGE = %s\n", msg);
        // TODO: Finalizar a montagem da mensagem e enviar para o serviço de autorizador
        *state_io = STT_WAIT_RESP;

        break;

    case STT_WAIT_RESP:
        printf("Transação aprovada!\n");
        // TODO: Criar fluxo de recebmento de resposta do autorizador e exibir mensagens de acordo
        *state_io = STT_RECEIPT;

        break;

    case STT_RECEIPT:
        printf("Imprimindo via do estabelecimento\n");
        printf("Deseja imprimir via do cliente?\n 1. Sim\n 2. Não\n");
        fgets(buf, sizeof (buf), stdin);
        if (buf[0] == '1')
            printf("Imprimindo via do cliente\n");
        
        *state_io = STT_IDLE;

        break;
        
    default:
        fprintf(stderr, "ERRO: ESTADO INESPERADO\n");
        
        return -1;
    }

    return 0;
}
