#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

//Estruturas-

typedef struct Venda {
    int nota_fiscal;
    float valor;
    char marca[50];
    int ano;
    char modelo[50];
    struct Venda *prox;
    struct Venda *ant;
} Venda;

typedef struct Vendedor {
    char nome[100];
    char cpf[12];
    int matricula;
    Venda *inicio_vendas;
    Venda *fim_vendas;
    int qtd_vendas;
    struct Vendedor *prox;
} Vendedor;

typedef struct {
    Vendedor *fim;
    int qtd_vendedores;
} ListaConcessionaria;


//Fila(FIFO)- Processo de espera para clientes
typedef struct Cliente {
    char nome[100];
    struct Cliente *prox;
} Cliente;

typedef struct {
    Cliente *frente;
    Cliente *tras;
} FilaAtendimento;

//Pilha(LIFO)- Histórico de Ações
typedef struct Log {
    char acao[100];
    struct Log *prox;
} Log;

typedef struct {
    Log *topo;
} PilhaHistorico;

//Inicializações

void inicializa_concessionaria(ListaConcessionaria *l)
    { l->fim = NULL; l->qtd_vendedores = 0; }
void inicializa_fila(FilaAtendimento *f)
    { f->frente = f->tras = NULL; }
void inicializa_pilha(PilhaHistorico *p)
    { p->topo = NULL; }

//Funções Auxiliares(Pilha e Fila)

void empilhar_log(PilhaHistorico *p, char mensagem[]) {
    Log *novo = (Log*)malloc(sizeof(Log));
    if(novo) {
        strcpy(novo->acao, mensagem);
        novo->prox = p->topo;
        p->topo = novo;
    }
}

void enfileirar_cliente(FilaAtendimento *f, char nome[]) {
    Cliente *novo = (Cliente*)malloc(sizeof(Cliente));
    if(novo) {
        strcpy(novo->nome, nome);
        novo->prox = NULL;
        if (f->tras == NULL) f->frente = novo;
        else f->tras->prox = novo;
        f->tras = novo;
        printf("Cliente %s entrou na fila.\n", nome);
    }
}

void chamar_proximo(FilaAtendimento *f) {
    if (f->frente == NULL) {
        printf("Fila vazia!\n");
        return;
    }
    Cliente *temp = f->frente;
    printf("Atendendo cliente: %s\n", temp->nome);
    f->frente = f->frente->prox;
    if (f->frente == NULL) f->tras = NULL;
    free(temp);
}

//Funcionalidade do Sistema

void cadastrar_vendedor(ListaConcessionaria *c, PilhaHistorico *p) {
    char nome[100], cpf[12];
    int mat;
    printf("Nome: "); scanf(" %[^\n]", nome);
    printf("CPF: "); scanf("%s", cpf);
    printf("Matrícula: "); scanf("%d", &mat);

    Vendedor *novo = (Vendedor*)malloc(sizeof(Vendedor));
    strcpy(novo->nome, nome);
    strcpy(novo->cpf, cpf);
    novo->matricula = mat;
    novo->inicio_vendas = novo->fim_vendas = NULL;
    novo->qtd_vendas = 0;

    if (c->fim == NULL) {
        novo->prox = novo;
        c->fim = novo;
    } else {
        novo->prox = c->fim->prox;
        c->fim->prox = novo;
        c->fim = novo;
    }
    c->qtd_vendedores++;
    empilhar_log(p, "Novo vendedor cadastrado");
}

void registrar_venda(ListaConcessionaria *c, PilhaHistorico *p) {
    if (!c->fim) return;
    int mat;
    printf("Matrícula do vendedor: "); scanf("%d", &mat);
    
    Vendedor *v = c->fim->prox;
    int achou = 0;
    do {
        if (v->matricula == mat) { achou = 1; break; }
        v = v->prox;
    } while (v != c->fim->prox);

    if (achou) {
        Venda *nova = (Venda*)malloc(sizeof(Venda));
        printf("Valor: "); scanf("%f", &nova->valor);
        printf("Marca: "); scanf(" %[^\n]", nova->marca);
        nova->prox = NULL;
        nova->ant = v->fim_vendas;

        if (v->inicio_vendas == NULL) v->inicio_vendas = nova;
        else v->fim_vendas->prox = nova;
        v->fim_vendas = nova;
        v->qtd_vendas++;
        empilhar_log(p, "Venda realizada com sucesso");
    }
}

// ARRAY:Uso de Array Estático para Metas
void relatorio_metas() {
    float metas[4] = {100000.0, 150000.0, 200000.0, 250000.0}; // Array fixo
        printf("\n--- Metas Trimestrais (Uso de Array) ---\n");
    for(int i=0; i<4; i++) 
        printf("Trimestre %d: R$ %.2f\n", i+1, metas[i]);
}

void exibir_historico(PilhaHistorico *p) {
    Log *atual = p->topo;
    printf("\n--- Histórico de Ações (Pilha - LIFO) ---\n");
    while(atual) {
        printf("[%s]\n", atual->acao);
        atual = atual->prox;
    }
}

//Limpeza de memória
void limpar_tudo(ListaConcessionaria *c, FilaAtendimento *f, PilhaHistorico *p) {
    // Implementar loops de free para cada estrutura antes de fechar
    printf("Limpando memória e saindo...\n");
}

int main() {
    ListaConcessionaria conc;
    FilaAtendimento fila;
    PilhaHistorico pilha;
    
    inicializa_concessionaria(&conc);
    inicializa_fila(&fila);
    inicializa_pilha(&pilha);

    int opt;
    char aux_nome[100];

    do {
        printf("\n1. Vendedor | 2. Venda | 3. Chegada Cliente (Fila) | 4. Atender (Fila) | 5. Pilha Logs | 6. Metas (Array) | 0. Sair\n");
        printf("Opção: "); scanf("%d", &opt);

        switch(opt) {
            case 1: cadastrar_vendedor(&conc, &pilha); break;
            case 2: registrar_venda(&conc, &pilha); break;
            case 3: printf("Nome Cliente: "); scanf(" %[^\n]", aux_nome); enfileirar_cliente(&fila, aux_nome); break;
            case 4: chamar_proximo(&fila); break;
            case 5: exibir_historico(&pilha); break;
            case 6: relatorio_metas(); break;
        }
    } while(opt != 0);

    limpar_tudo(&conc, &fila, &pilha);
    return 0;
}
