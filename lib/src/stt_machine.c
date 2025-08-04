int stt_machine(void)
{
    stt_machine_e state = STT_IDLE;

    switch (pos_status)
    {
    case POS_STT_IDLE:
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
            pos_status = POS_STT_QUIT;
            break;
        }

        if (setamount(buf, &transinfo) == 0)
        {
            printf("O VALOR PRECISA SER MAIOR QUE 0\n");
            break;
        }

        pos_status = POS_STT_BEGIN_TRANS;
        break;

    case POS_STT_BEGIN_TRANS:
        printf("\nSelecione a forma de pagamento:\n1. Debito\n2. Crédito\n");
        fgets(buf, sizeof (buf), stdin);

        if (buf[0] == '1')
            strcpy(mod, "Debito");
        else if (buf[0] == '2')
            strcpy(mod, "Crédito");
        else
        {
            printf("Modo inválido\n");
            pos_status = POS_STT_IDLE;
            break;
        }

        pos_status = POS_STT_READ_CARD;
        break;

    case POS_STT_READ_CARD:
        printf("\nR$%.2f %s\n\n", (float)(transinfo.de004_amount / 100), mod);
        printf("Aproxime ou insira o cartão\n");
        readcard(&transinfo);

        pos_status = POS_STT_AUTH_SEND;

        break;

    case POS_STT_AUTH_SEND:
        printf("Processando...\n");
        // TODO: Finalizar a montagem da mensagem e enviar para o serviço de autorizador
        pos_status = POS_STT_AUTH_RECV;

        break;

    case POS_STT_AUTH_RECV:
        printf("Transação aprovada!\n");
        // TODO: Criar fluxo de recebmento de resposta do autorizador e exibir mensagens de acordo
        pos_status = POS_STT_RECEIPT;

        break;

    case POS_STT_RECEIPT:
        printf("Imprimindo via do estabelecimento\n");
        printf("Deseja imprimir via do cliente?\n 1. Sim\n 2. Não\n");
        fgets(buf, sizeof (buf), stdin);
        if (buf[0] == '1')
            printf("Imprimindo via do cliente\n");
        
        pos_status = POS_STT_IDLE;

        break;
        
    case POS_STT_QUIT:
        flag_exec = STOP;

        break;

    default:
        printf("ERRO INESPERADO\n");
        flag_exec = STOP;
        break;
    }
}
