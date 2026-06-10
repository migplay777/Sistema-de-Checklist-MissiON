#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define MAX_LISTAS 50
#define MAX_ITENS 100
#define MAX_NOME 100
#define MAX_TAMANHO 256

// Estruturas de dados
typedef struct {
    char nome[MAX_NOME];
    int quantidade;
    int concluido;
} Item;

typedef struct {
    char nome[MAX_NOME];
    Item itens[MAX_ITENS];
    int num_itens;
} Lista;

// Variáveis globais
Lista listas[MAX_LISTAS];
int num_listas = 0;
int listas_carregadas = 0;

// Prototipos das funções
void menu_principal();
void criar_lista();
void visualizar_listas();
void editar_lista();
void salvar_dados();
void carregar_dados();
void exportar_txt();
void limpar_buffer();
int encontrar_lista(char* nome);
void marcar_item(int indice_lista, int indice_item);
void adicionar_item(int indice_lista);
void editar_item(int indice_lista, int indice_item);
void remover_item(int indice_lista, int indice_item);
void excluir_lista(int indice_lista);
// Função para limpar buffer do teclado (ESSENCIAL)
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ETAPA 2 - MENU PRINCIPAL
int main() {
    setlocale(LC_ALL,"");
    printf("===SISTEMA DE CHECKLIST MISSION===");

    // Carrega dados salvos
    carregar_dados();

    int opcao;
    do {
        menu_principal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch(opcao) {
            case 1: criar_lista(); break;
            case 2: visualizar_listas(); break;
            case 3: editar_lista(); break;
            case 4: salvar_dados(); break;
            case 5: exportar_txt(); break;
            case 6: printf("Obrigado por usar o sistema!\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while(opcao != 6);

    return 0;
}

void menu_principal() {
    system("cls || clear");
    printf("\n=== SISTEMA DE CHECKLIST MISSION ===\n");
    printf("1. Criar nova lista\n");
    printf("2. Visualizar listas\n");
    printf("3. Editar lista\n");
    printf("4. Salvar dados\n");
    printf("5. Exportar para TXT\n");
    printf("6. Sair\n");
}

// ETAPA 3 - CADASTRO
void criar_lista() {
    if(num_listas >= MAX_LISTAS) {
        printf("Limite maximo de listas atingido!\n");
        return;
    }

    printf("\n--- CRIAR NOVA LISTA ---\n");
    printf("Nome da lista: ");
    fgets(listas[num_listas].nome, MAX_NOME, stdin);
    listas[num_listas].nome[strcspn(listas[num_listas].nome, "\n")] = 0;
    listas[num_listas].num_itens = 0;

    printf("Lista '%s' criada com sucesso!\n", listas[num_listas].nome);
    num_listas++;

    // Adicionar itens iniciais
    char adicionar;
    do {
        adicionar_item(num_listas - 1);
        printf("Adicionar mais um item? (s/n): ");
        scanf(" %c", &adicionar);
        limpar_buffer();
    } while(tolower(adicionar) == 's');
}

// ETAPA 3 - ADICIONAR ITENS
void adicionar_item(int indice_lista) {
    if(listas[indice_lista].num_itens >= MAX_ITENS) {
        printf("Limite maximo de itens atingido!\n");
        return;
    }

    printf("\n--- ADICIONAR ITEM ---\n");
    printf("Nome do item: ");
    fgets(listas[indice_lista].itens[listas[indice_lista].num_itens].nome,
          MAX_NOME, stdin);
    listas[indice_lista].itens[listas[indice_lista].num_itens].nome[strcspn(
        listas[indice_lista].itens[listas[indice_lista].num_itens].nome, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &listas[indice_lista].itens[listas[indice_lista].num_itens].quantidade);
    listas[indice_lista].itens[listas[indice_lista].num_itens].concluido = 0;

    listas[indice_lista].num_itens++;
    printf("Item adicionado com sucesso!\n");
}

// ETAPA 2 - VISUALIZAR
void visualizar_listas() {
    if(num_listas == 0) {
        printf("\nNenhuma lista criada!\n");
        return;
    }

    printf("\n--- LISTAS DISPONIVEIS ---\n");
    for(int i = 0; i < num_listas; i++) {
        int concluido = 0;
        for(int j = 0; j < listas[i].num_itens; j++) {
            if(listas[i].itens[j].concluido) concluido++;
        }
        printf("%d. %s (%d/%d concluido)\n", i+1, listas[i].nome,
               concluido, listas[i].num_itens);
    }

    int indice;
    printf("Digite o numero da lista para visualizar (0 para voltar): ");
    scanf("%d", &indice);
    limpar_buffer();

    if(indice < 1 || indice > num_listas) return;

    indice--;
    printf("\n--- LISTA: %s ---\n", listas[indice].nome);
    for(int i = 0; i < listas[indice].num_itens; i++) {
        char status = listas[indice].itens[i].concluido ? 'X' : ' ';
        printf("%d. [%c] %s (%d)\n", i+1, status,
               listas[indice].itens[i].nome, listas[indice].itens[i].quantidade);
    }

    printf("\nPressione Enter para continuar...");
    getchar();
}

// ETAPA 4 - SISTEMA DE CHECKLIST
void editar_lista() {
    if(num_listas == 0) {
        printf("Nenhuma lista para editar!\n");
        return;
    }

    visualizar_listas();
    int indice_lista;
    printf("Digite o numero da lista: ");
    scanf("%d", &indice_lista);
    limpar_buffer();

    if(indice_lista < 1 || indice_lista > num_listas) {
        printf("Lista nao encontrada!\n");
        return;
    }

    indice_lista--;

    int opcao;
    do {
        printf("\n--- EDITAR LISTA: %s ---\n", listas[indice_lista].nome);
        printf("1. Marcar/Desmarcar item\n");
        printf("2. Adicionar item\n");
        printf("3. Editar item\n");
        printf("4. Remover item\n");
        printf("5. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch(opcao) {
            case 1: {
                int indice_item;
                printf("Item (1-%d): ", listas[indice_lista].num_itens);
                scanf("%d", &indice_item);
                limpar_buffer();
                if(indice_item >= 1 && indice_item <= listas[indice_lista].num_itens) {
                    marcar_item(indice_lista, indice_item - 1);
                }
                break;
            }
            case 2: adicionar_item(indice_lista); break;
            case 3: {
                int indice_item;
                printf("Qual item editar (1-%d): ", listas[indice_lista].num_itens);
                scanf("%d", &indice_item);
                limpar_buffer();
                if(indice_item >= 1 && indice_item <= listas[indice_lista].num_itens) {
                    editar_item(indice_lista, indice_item - 1);
                }
                break;
            }
            case 4: {
                int indice_item;
                printf("Qual item remover (1-%d): ", listas[indice_lista].num_itens);
                scanf("%d", &indice_item);
                limpar_buffer();
                if(indice_item >= 1 && indice_item <= listas[indice_lista].num_itens) {
                    remover_item(indice_lista, indice_item - 1);
                }
                break;
            }
        }
    } while(opcao != 5);
}

// ETAPA 4 - MARCAR ITENS (CORRIGIDA)
void marcar_item(int indice_lista, int indice_item) {
    // Inverte o status do item
    listas[indice_lista].itens[indice_item].concluido =
        !listas[indice_lista].itens[indice_item].concluido;

    // ✅ CORRIGIDO: usa os parâmetros corretos
    char status = listas[indice_lista].itens[indice_item].concluido ? 'X' : ' ';

    printf("✅ Item '%s' marcado como [",
           listas[indice_lista].itens[indice_item].nome);
    printf("%c", status);
    printf("]\n");
}
// ETAPA 5 - EDIÇÃO
void editar_item(int indice_lista, int indice_item) {
    printf("\nNovo nome do item: ");
    fgets(listas[indice_lista].itens[indice_item].nome, MAX_NOME, stdin);
    listas[indice_lista].itens[indice_item].nome[strcspn(
        listas[indice_lista].itens[indice_item].nome, "\n")] = 0;

    printf("Nova quantidade: ");
    scanf("%d", &listas[indice_lista].itens[indice_item].quantidade);

    printf("Item editado com sucesso!\n");
}

void remover_item(int indice_lista, int indice_item) {
    for(int i = indice_item; i < listas[indice_lista].num_itens - 1; i++) {
        strcpy(listas[indice_lista].itens[i].nome, listas[indice_lista].itens[i+1].nome);
        listas[indice_lista].itens[i].quantidade = listas[indice_lista].itens[i+1].quantidade;
        listas[indice_lista].itens[i].concluido = listas[indice_lista].itens[i+1].concluido;
    }
    listas[indice_lista].num_itens--;
    printf("Item removido com sucesso!\n");
}

// ETAPA 6 - SALVAMENTO
void salvar_dados() {
    FILE *arquivo = fopen("checklist.dat", "wb");
    if(arquivo == NULL) {
        printf("Erro ao salvar!\n");
        return;
    }

    fwrite(&num_listas, sizeof(int), 1, arquivo);
    for(int i = 0; i < num_listas; i++) {
        fwrite(listas[i].nome, sizeof(char), MAX_NOME, arquivo);
        fwrite(&listas[i].num_itens, sizeof(int), 1, arquivo);
        for(int j = 0; j < listas[i].num_itens; j++) {
            fwrite(listas[i].itens[j].nome, sizeof(char), MAX_NOME, arquivo);
            fwrite(&listas[i].itens[j].quantidade, sizeof(int), 1, arquivo);
            fwrite(&listas[i].itens[j].concluido, sizeof(int), 1, arquivo);
        }
    }

    fclose(arquivo);
    printf("Dados salvos com sucesso em checklist.dat!\n");
}

void carregar_dados() {
    FILE *arquivo = fopen("checklist.dat", "rb");
    if(arquivo == NULL) {
        listas_carregadas = 0;
        return;
    }

    fread(&num_listas, sizeof(int), 1, arquivo);
    for(int i = 0; i < num_listas; i++) {
        fread(listas[i].nome, sizeof(char), MAX_NOME, arquivo);
        fread(&listas[i].num_itens, sizeof(int), 1, arquivo);
        for(int j = 0; j < listas[i].num_itens; j++) {
            fread(listas[i].itens[j].nome, sizeof(char), MAX_NOME, arquivo);
            fread(&listas[i].itens[j].quantidade, sizeof(int), 1, arquivo);
            fread(&listas[i].itens[j].concluido, sizeof(int), 1, arquivo);
        }
    }

    fclose(arquivo);
    listas_carregadas = 1;
    printf("Dados carregados com sucesso (%d listas)!\n", num_listas);
}

// ETAPA 7 - EXPORTAÇÃO (VERSÃO FINAL CORRIGIDA)
void exportar_txt() {
    if(num_listas == 0) {
        printf("\nNenhuma lista para exportar!\n");
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }

    char nome_arquivo[256];
    printf("\nNome do arquivo (ex: relatorio.txt): ");
    fgets(nome_arquivo, 256, stdin);
    nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;

    FILE *f = fopen(nome_arquivo, "w");
    if(!f) {
        printf("Erro ao criar arquivo!\n");
        printf("Pressione Enter...");
        getchar();
        return;
    }

    fprintf(f, "=== CHECKLIST MISSION ===\n");
    fprintf(f, "Data: %s\n\n", __DATE__);

    int lista_count = 0;
    for(lista_count = 0; lista_count < num_listas; lista_count++) {
        fprintf(f, "LISTA %d: %s\n", lista_count+1, listas[lista_count].nome);
        fprintf(f, "==============================\n");

        int itens_concluidos = 0;
        int item_count = 0;
        for(item_count = 0; item_count < listas[lista_count].num_itens; item_count++) {
            char marca = listas[lista_count].itens[item_count].concluido ? 'X' : ' ';
            fprintf(f, "[%c] %s (%d unid.)\n",
                   marca,
                   listas[lista_count].itens[item_count].nome,
                   listas[lista_count].itens[item_count].quantidade);

            if(listas[lista_count].itens[item_count].concluido) {
                itens_concluidos++;
            }
        }
        fprintf(f, "\nRESUMO: %d de %d itens concluidos\n\n",
                itens_concluidos, listas[lista_count].num_itens);
    }

    fclose(f);
    printf("\n✅ EXPORTADO para '%s'\n", nome_arquivo);
    printf("Pressione Enter para continuar...");
    getchar();
}
