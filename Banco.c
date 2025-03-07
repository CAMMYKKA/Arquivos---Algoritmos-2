#include <stdio.h>
#include <string.h>

struct banco{
    int n_conta;
    char titular[50];
    char segtitular[50];
    float saldo;
};

int cadastro(struct banco banco[], int qtdcontas, int n_conta, int saldo, char* titular, char* segtitular){

    strcpy(banco[qtdcontas].titular, titular);
    strcpy(banco[qtdcontas].segtitular, segtitular);
    banco[qtdcontas].saldo = saldo;
    banco[qtdcontas].n_conta = n_conta;
    return qtdcontas + 1;
}

int cadastroArquivo(struct banco banco[], int *qtdcontas, int max) {
    int i = *qtdcontas; 
    FILE *arquivo = fopen("contas.txt", "r"); 
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de contas!\n");
        return *qtdcontas;
    }
    struct banco temp;
    while (i < max && fscanf(arquivo, "%d\n", &banco[i].n_conta) == 1) {
        
        fgets(banco[i].titular, sizeof(banco[i].titular), arquivo);
        banco[i].titular[strcspn(banco[i].titular, "\n")] = '\0';  

        fgets(banco[i].segtitular, sizeof(banco[i].segtitular), arquivo);
        banco[i].segtitular[strcspn(banco[i].segtitular, "\n")] = '\0';  

        fscanf(arquivo, "%f\n", &banco[i].saldo);
        
        i++; 
    }

    fclose(arquivo);

    *qtdcontas = i;
    return *qtdcontas;
}

void salvar_contas(struct banco banco[], int qtdcontas) {
    FILE *arquivo = fopen("contas.bin", "wb");  
    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo de contas para escrita!\n");
        return;
    }

    fwrite(&qtdcontas, sizeof(int), 1, arquivo);
    fwrite(banco, sizeof(struct banco), qtdcontas, arquivo);
    fclose(arquivo);
    
}

void carregar_contas(struct banco banco[], int max, int *qtdcontas) {
    FILE *arquivo = fopen("contas.bin", "rb");  
    if (arquivo == NULL) {
        printf("\nNenhum arquivo de contas encontrado, iniciando vazio.");
        return;  
    }
    
    if (*qtdcontas > max) {
        printf("O número de contas no arquivo excede o limite máximo!\n");
        fclose(arquivo);
        return; 
    }
    
    fread(qtdcontas, sizeof(int), 1, arquivo);
    fread(banco, sizeof(struct banco), *qtdcontas, arquivo);
    

    fclose(arquivo);
    printf("Contas carregadas com sucesso!\n");
}

int excluir_contas(struct banco banco[], int qtdcontas, int num, int escolha){

    if(num == 1){
        for(int i = (escolha - 1); i < qtdcontas; i++){
            banco[i] = banco[i + 1];
        }
        printf("\n\nConta excluida com sucesso!");
        return qtdcontas - 1;
    }

    if(num == 2){
        printf("Cancelando...");
        return qtdcontas;
    }
    if(num != 1 && num != 2){
        printf("Opcao invalida, digite novamente");
        scanf("%d" , &num);
    }
}

void consulta_conta(struct banco banco[], int qtdcontas){
    int escolha = 0;
    int i = 0;

    if(qtdcontas == 0){
        printf("Nenhuma conta cadastrada!");
        return;
    }
    for(int i = 0; i < qtdcontas; i++){
        printf("Titular: %d. %s\n [Numero da conta: %d]\n\n", 
            i + 1, banco[i].titular,banco[i].n_conta);
        }
        
    printf("Escolha a conta a ser consultada:\n");
    scanf("%d" , &escolha);
    
    if(escolha < 1 || escolha > qtdcontas){
        printf("Invalido.\n");
        return qtdcontas;
    }else{
        for(int i = (escolha - 1); i < escolha;i++){
            printf("Titular: %s | Segundo Titular: %s\n [Numero da conta: %d - Saldo: %.2f]\n\n", banco[i].titular, banco[i].segtitular, banco[i].n_conta, banco[i].saldo);
        }
    }
}

void ver_contas(struct banco banco[], int qtdcontas){
    if(qtdcontas == 0){
        printf("Nenhuma conta cadastrada!\n");
        return;
    }
    printf("Contas cadastradas:\n");
    for(int i = 0; i < qtdcontas; i++){
        printf("Titular: %d. %s  | Segundo Titular: %s\n [Numero da conta: %d | Saldo: %.2f]\n\n", 
        i + 1, banco[i].titular, banco[i].segtitular, banco[i].n_conta, banco[i].saldo);
    }
}

int main(void){

    int opcao_menu = 0;
    int qtdcontas = 0; 
    int max = 300;
    int escolha = 0, num;
    int i = 0;
    int n_conta;
    int tipo_conta;
    float saldo;
    char titular[50], segtitular[50];

    struct banco banco[max];
    carregar_contas(banco, max, &qtdcontas);

    do{
        printf("\n-------- B E M - V I N D O - A O - CAMBANK! --------\n");
        printf("\nPra onde iremos agora?\n");
        printf("\n1 - Cadastrar uma nova conta");
        printf("\n2 - Cadastrar a partir de Arquivo");
        printf("\n3 - Consultar conta");
        printf("\n4 - Excluir conta");
        printf("\n5 - Ver contas cadastradas");
        printf("\n6 - Salvar dados em arquivo");
        printf("\n7 - Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d" , &opcao_menu);

        switch(opcao_menu){
            case 1: 

                if(qtdcontas >= max){
                    printf("Limite de contas atingido!\n");
                    return qtdcontas;
                }
            
                printf("Digite o numero da conta: ");
                scanf("%d" , &n_conta);
            
                for(i = 0; i < qtdcontas; i++){
                    if(n_conta == banco[i].n_conta){
                        printf("Numero da conta ja existe! Cadastro cancelado.\n");
                        return qtdcontas;
                    }
                }

                getchar();
                printf("Digite o nome do titular: ");            
                fgets(titular, 50, stdin);
                titular[strcspn(titular, "\n")] = '\0';

                printf("Qual o tipo de conta?");
                printf("\n1 - Individual | 2 - Conjunta \n");
                scanf("%d" , &tipo_conta);

                while(tipo_conta != 1 && tipo_conta != 2){
                    printf("\nOpcao invalida! Digite novamente: ");
                    scanf("%d" , &tipo_conta);
                }

                if(tipo_conta == 1){
                    segtitular[0] = '\0';
                }else{
                    printf("Nome do segundo titular: ");
                    getchar();
                    fgets(segtitular, 50, stdin);
                            
                    if(strcmp(segtitular, titular) == 0){
                        printf("Os nomes não podem ser iguais! Cadastro cancelado!");
                        return qtdcontas;
                    }
                }

                printf("Digite o saldo da conta: ");
                scanf("%f" , &saldo);
                getchar();
                
                qtdcontas = cadastro(banco,qtdcontas, n_conta, saldo,titular, segtitular);

                printf("Conta cadastrada com sucesso!\n");
                break;

            case 2:
                qtdcontas = cadastroArquivo(banco, &qtdcontas, max);
                printf("Contas do arquivo cadastradas com sucesso!\n");
                break;
            case 3:
                consulta_conta(banco, qtdcontas);
                break;
            case 4:
                if(qtdcontas == 0){
                    printf("Nenhuma conta cadastrado!");
                    return qtdcontas;
                }
                
                for(int i = 0; i < qtdcontas; i++){
                    printf("\n%d. Titular: %s | Segundo titular: %s \n[numero da conta: %d | saldo: %.2f]" , i + 1, banco[i].titular,banco[i].segtitular, banco[i].n_conta, banco[i].saldo);
                }
                printf("\nSelecione a conta a ser excluida: ");
                scanf("%d" , &escolha);
            
                if(escolha < 1 || escolha > qtdcontas){
                    printf("Invalido.\n");
                    return qtdcontas;
                }
                
                printf("Confirmar exclusao da conta?");
                printf("\n1.Sim");
                printf("\n2.Nao");
                scanf("%d" , &num);
                qtdcontas = excluir_contas(banco, qtdcontas, num, escolha);
                break;

            case 5:
                ver_contas(banco, qtdcontas);
                break;
            case 6:
                salvar_contas(banco, qtdcontas);
                printf("Contas salvas com sucesso!\n");
                break;
            case 7:
            
                printf("Salvar dados antes de fechar?");
                printf("1. Sim | 2. Nao\n");
                scanf("%d" , &escolha);

                if(escolha == 1){
                    salvar_contas(banco, qtdcontas);
                    printf("Dados salvos! Saindo...");
                    return 0;
                }
                if(escolha == 2){
                    printf("Saindo sem salvar...");
                    return 0;
                }else if(escolha != 1 && escolha != 2){
                    printf("Escolha invalida! Digite novamente: ");
                    scanf("%d" , &escolha);
                }

                return 0;
                break;
            default:
                printf("\nOpcao invalida! Escolha de 1 a 7.\n");
        }
        
    }while(opcao_menu!=7);
}