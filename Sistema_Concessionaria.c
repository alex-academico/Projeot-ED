#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma venda
typedef struct Venda
{
    int nota_fiscal;
    float valor;
    char marca[50];
    int ano;
    char modelo[50];
    struct Venda *prox;
    struct Venda *ant;
} Venda;

// Estrutura para representar um vendedor
typedef struct Vendedor
{
    char nome[100];
    char cpf[12];
    int matricula;
    Venda *inicio_vendas;
    Venda *fim_vendas;
    int qtd_vendas;
    struct Vendedor *prox;
} Vendedor;

// Nó cabeça da lista circular de vendedores
typedef struct
{
    Vendedor *fim;
    int qtd_vendedores;
} ListaConcessionaria;


// Inicializa a lista de vendas de um vendedor
void inicializa_vendas(Vendedor *v)
{
    v->inicio_vendas = NULL;
    v->fim_vendas = NULL;
    v->qtd_vendas = 0;
}

// Inicializa a concessionária
void inicializa_concessionaria(ListaConcessionaria *concessionaria)
{
    concessionaria->fim = NULL;
    concessionaria->qtd_vendedores = 0;
}

// Função para criar um novo vendedor
Vendedor *criar_vendedor(char nome[], char cpf[], int matricula)
{
    Vendedor *novo = (Vendedor *)malloc(sizeof(Vendedor));
    if (novo == NULL)
        return NULL;

    strcpy(novo->nome, nome);
    strcpy(novo->cpf, cpf);
    novo->matricula = matricula;
    inicializa_vendas(novo);
    novo->prox = novo;
    return novo;
}

// Insere um vendedor na lista circular
void inserir_vendedor(ListaConcessionaria *concessionaria)
{
    char nome[100], cpf[12];
    int matricula;

    printf("\n--- Cadastro de Vendedor ---\n");
    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("CPF (apenas números): ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    printf("Matrícula: ");
    scanf("%d", &matricula);
    // Verificar se matrícula já existe
    if (concessionaria->fim != NULL)
    {
        Vendedor *atual = concessionaria->fim->prox;
        do
        {
            if (atual->matricula == matricula)
            {
                printf("Erro: Matrícula já existente!\n");
                return;
            }
            atual = atual->prox;
        } while (atual != concessionaria->fim->prox);
    }

    Vendedor *novo = criar_vendedor(nome, cpf, matricula);
    if (novo == NULL)
    {
        printf("Erro ao alocar memória!\n");
        return;
    }

    if (concessionaria->fim == NULL)
    {
        concessionaria->fim = novo;
    }
    else
    {
        novo->prox = concessionaria->fim->prox;
        concessionaria->fim->prox = novo;
        concessionaria->fim = novo;
    }
    concessionaria->qtd_vendedores++;
    printf("Vendedor cadastrado com sucesso!\n");
}

// Remove um vendedor da lista circular
void remover_vendedor(ListaConcessionaria *concessionaria)
{
    if (concessionaria->fim == NULL)
    {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    int matricula;
    printf("\n--- Remover Vendedor ---\n");
    printf("Matrícula do vendedor a ser removido: ");
    scanf("%d", &matricula);

    Vendedor *atual = concessionaria->fim->prox, *anterior = concessionaria->fim;

    // Encontrar o vendedor
    do
    {
        if (atual->matricula == matricula)
            break;
        anterior = atual;
        atual = atual->prox;
    } while (atual != concessionaria->fim->prox);

    if (atual->matricula != matricula)
    {
        printf("Vendedor não encontrado.\n");
        return;
    }

    // Liberar vendas
    Venda *v = atual->inicio_vendas;
    while (v != NULL)
    {
        Venda *temp = v;
        v = v->prox;
        free(temp);
    }

    // Atualizar lista
    if (concessionaria->qtd_vendedores == 1)
    {
        concessionaria->fim = NULL;
    }
    else
    {
        anterior->prox = atual->prox;
        if (atual == concessionaria->fim)
        {
            concessionaria->fim = anterior;
        }
    }

    free(atual);
    concessionaria->qtd_vendedores--;
    printf("Vendedor removido com sucesso.\n");
}

// Cadastra uma nova venda para um vendedor
void cadastrar_venda(ListaConcessionaria *concessionaria)
{
    if (concessionaria->fim == NULL)
    {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    int matricula, nota_fiscal, ano;
    float valor;
    char marca[50], modelo[50];

    printf("\n--- Cadastro de Venda ---\n");
    printf("Matrícula do vendedor: ");
    scanf("%d", &matricula);

    Vendedor *v = concessionaria->fim->prox;
    do
    {
        if (v->matricula == matricula)
            break;
        v = v->prox;
    } while (v != concessionaria->fim->prox);

    if (v->matricula != matricula)
    {
        printf("Vendedor não encontrado.\n");
        return;
    }

    printf("Número da nota fiscal: ");
    scanf("%d", &nota_fiscal);

    printf("Valor da venda: ");
    scanf("%f", &valor);

    printf("Marca do veículo: ");
    fgets(marca, sizeof(marca), stdin);
    marca[strcspn(marca, "\n")] = '\0';

    printf("Ano do veículo: ");
    scanf("%d", &ano);

    printf("Modelo do veículo: ");
    fgets(modelo, sizeof(modelo), stdin);
    modelo[strcspn(modelo, "\n")] = '\0';

    Venda *nova = (Venda *)malloc(sizeof(Venda));
    if (nova == NULL)
    {
        printf("Erro ao alocar memória!\n");
        return;
    }

    nova->nota_fiscal = nota_fiscal;
    nova->valor = valor;
    strcpy(nova->marca, marca);
    nova->ano = ano;
    strcpy(nova->modelo, modelo);
    if (v->inicio_vendas == NULL)
    {
        nova->prox = NULL;
        nova->ant = NULL;
        v->inicio_vendas = nova;
    }
    else
    {
        nova->prox = NULL;
        nova->ant = v->fim_vendas;
        v->fim_vendas->prox = nova;
        v->fim_vendas = nova;
    }
    v->qtd_vendas++;

    printf("Venda registrada com sucesso para %s.\n", v->nome);
}

// Exibe o total de vendas de cada vendedor
void exibir_total_vendas_por_vendedor(ListaConcessionaria *concessionaria)
{
    if (concessionaria->fim == NULL)
    {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    printf("\n--- Total de Vendas por Vendedor ---\n");

    Vendedor *atual = concessionaria->fim->prox;
    do
    {
        float total = 0.0;
        Venda *v = atual->inicio_vendas;

        while (v != NULL)
        {
            total += v->valor;
            v = v->prox;
        }

        printf("Vendedor: %s (Matrícula: %d)\n", atual->nome, atual->matricula);
        printf("Quantidade de vendas: %d\n", atual->qtd_vendas);
        printf("Total vendido: R$ %.2f\n\n", total);

        atual = atual->prox;
    } while (atual != concessionaria->fim->prox);
}

// Exibe o valor total de todas as vendas da concessionária
void exibir_total_vendas_concessionaria(ListaConcessionaria *concessionaria)
{
    if (concessionaria->fim == NULL)
    {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    float total = 0.0;
    int qtd_total = 0;

    Vendedor *atual = concessionaria->fim->prox;
    do
    {
        Venda *v = atual->inicio_vendas;
        while (v != NULL)
        {
            total += v->valor;
            qtd_total++;
            v = v->prox;
        }
        atual = atual->prox;
    } while (atual != concessionaria->fim->prox);

    printf("\n--- Total de Vendas da Concessionária ---\n");
    printf("Quantidade total de vendas: %d\n", qtd_total);
    printf("Valor total vendido: R$ %.2f\n", total);
}

// Exibe o vendedor que deu mais lucro à concessionária
void exibir_vendedor_mais_lucrativo(ListaConcessionaria *concessionaria)
{
    if (concessionaria->fim == NULL)
    {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    Vendedor *mais_lucrativo = NULL;
    float maior_total = -1.0;

    Vendedor *atual = concessionaria->fim->prox;
    do
    {
        float total = 0.0;
        Venda *v = atual->inicio_vendas;

        while (v != NULL)
        {
            total += v->valor;
            v = v->prox;
        }

        if (total > maior_total)
        {
            maior_total = total;
            mais_lucrativo = atual;
        }

        atual = atual->prox;
    } while (atual != concessionaria->fim->prox);

    printf("\n--- Vendedor Mais Lucrativo ---\n");
    printf("Nome: %s\n", mais_lucrativo->nome);
    printf("CPF: %s\n", mais_lucrativo->cpf);
    printf("Matrícula: %d\n", mais_lucrativo->matricula);
    printf("Quantidade de vendas: %d\n", mais_lucrativo->qtd_vendas);
    printf("Total vendido: R$ %.2f\n", maior_total);
}

// Exibe todas as vendas de veículo de marca específica
void exibir_vendas_por_marca(ListaConcessionaria *concessionaria)
{
    if (concessionaria->fim == NULL)
    {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    char marca[50];
    printf("\n--- Vendas por Marca ---\n");
    printf("Digite a marca desejada: ");
    fgets(marca, sizeof(marca), stdin);
    marca[strcspn(marca, "\n")] = '\0';

    int encontrou = 0;

    Vendedor *atual = concessionaria->fim->prox;
    do
    {
        Venda *v = atual->inicio_vendas;
        while (v != NULL)
        {
            if (strcasecmp(v->marca, marca) == 0)
            {
                printf("\nVendedor: %s (Matrícula: %d)\n", atual->nome, atual->matricula);
                printf("Nota Fiscal: %d\n", v->nota_fiscal);
                printf("Valor: R$ %.2f\n", v->valor);
                printf("Modelo: %s %d\n", v->modelo, v->ano);
                encontrou = 1;
            }
            v = v->prox;
        }
        atual = atual->prox;
    } while (atual != concessionaria->fim->prox);

    if (!encontrou)
    {
        printf("Nenhuma venda encontrada para a marca %s.\n", marca);
    }
}

// Função para exibir todos os vendedores
void listar_vendedores(ListaConcessionaria *concessionaria)
{
    if (concessionaria->fim == NULL)
    {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Vendedores ---\n");

    Vendedor *atual = concessionaria->fim->prox;
    do
    {
        printf("\nNome: %s\n", atual->nome);
        printf("CPF: %s\n", atual->cpf);
        printf("Matrícula: %d\n", atual->matricula);
        printf("Quantidade de vendas: %d\n", atual->qtd_vendas);

        atual = atual->prox;
    } while (atual != concessionaria->fim->prox);
}

// Função para exibir o menu
void exibir_menu()
{
    printf("\n=== Sistema de Gerenciamento da Concessionária ===\n");
    printf("1. Cadastrar vendedor\n");
    printf("2. Remover vendedor\n");
    printf("3. Listar vendedores\n");
    printf("4. Cadastrar venda\n");
    printf("5. Total de vendas por vendedor\n");
    printf("6. Total de vendas da concessionária\n");
    printf("7. Vendedor mais lucrativo\n");
    printf("8. Vendas por marca\n");
    printf("9. Sair\n");
    printf("Escolha uma opção: ");
}

// Função para destruir todas as listas
void destroi_lista(ListaConcessionaria *concessionaria)
{
    if (concessionaria->fim == NULL)
        return;

    Vendedor *atual = concessionaria->fim->prox;
    while (atual != concessionaria->fim)
    {
        Vendedor *temp = atual;
        atual = atual->prox;

        // Liberar vendas
        Venda *v = temp->inicio_vendas;
        while (v != NULL)
        {
            Venda *temp_v = v;
            v = v->prox;
            free(temp_v);
        }

        free(temp);
    }

    // Liberar último vendedor
    if (concessionaria->fim != NULL)
    {
        Venda *v = concessionaria->fim->inicio_vendas;
        while (v != NULL)
        {
            Venda *temp_v = v;
            v = v->prox;
            free(temp_v);
        }
        free(concessionaria->fim);
    }

    concessionaria->fim = NULL;
    concessionaria->qtd_vendedores = 0;
}

int main()
{
    ListaConcessionaria concessionaria;
    inicializa_concessionaria(&concessionaria);
    int opcao;

    do
    {
        exibir_menu();
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            inserir_vendedor(&concessionaria);
            break;
        case 2:
            remover_vendedor(&concessionaria);
            break;
        case 3:
            listar_vendedores(&concessionaria);
            break;
        case 4:
            cadastrar_venda(&concessionaria);
            break;
        case 5:
            exibir_total_vendas_por_vendedor(&concessionaria);
            break;
        case 6:
            exibir_total_vendas_concessionaria(&concessionaria);
            break;
        case 7:
            exibir_vendedor_mais_lucrativo(&concessionaria);
            break;
        case 8:
            exibir_vendas_por_marca(&concessionaria);
            break;
        case 9:
            printf("Encerrando o sistema...\n");
            break;
        default:
            printf("Opção inválida!\n");
        }
    } while (opcao != 9);

    destroi_lista(&concessionaria);
    return 0;
}
