#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Estruturas Obrigatórias

typedef struct Venda {
    char nota_fiscal[50];
    float valor;
    char marca[50];
    int ano;
    char modelo[50];
    struct Venda *prox;
    struct Venda *ant; // Lista Duplamente Encadeada 
} Venda;

typedef struct Vendedor {
    char nome[100];
    char cpf[15];
    char matricula[13]; 
    Venda *inicio_vendas;
    Venda *fim_vendas;
    int qtd_vendas;
    struct Vendedor *prox; // Lista Circular
} Vendedor;

typedef struct {
    Vendedor *fim;
    int qtd_vendedores;
} ListaConcessionaria;

// Fila (FIFO) - Atendimento de Clientes 
typedef struct Cliente {
    char nome[100];
    struct Cliente *prox;
} Cliente;

typedef struct {
    Cliente *frente;
    Cliente *tras;
} FilaAtendimento;

// Pilha (LIFO) - Histórico de Ações 
typedef struct Log {
    char acao[100];
    struct Log *prox;
} Log;

typedef struct {
    Log *topo;
} PilhaHistorico;

// Inicializações 

void inicializa_concessionaria(ListaConcessionaria *l) { 
    l->fim = NULL; 
    l->qtd_vendedores = 0; 
}

void inicializa_fila(FilaAtendimento *f) { 
    f->frente = f->tras = NULL; 
}

void inicializa_pilha(PilhaHistorico *p) { 
    p->topo = NULL; 
}

// Funções Auxiliares (Pilha e Fila) 

void empilhar_log(PilhaHistorico *p, char mensagem[]) {
    Log *novo = (Log*)malloc(sizeof(Log)); // Alocação dinâmica
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
        printf("Todos os clientes foram atendidos!\n");
        return;
    }
    Cliente *temp = f->frente;
    printf("Atendendo cliente: %s\n", temp->nome);
    f->frente = f->frente->prox;
    if (f->frente == NULL) f->tras = NULL;
    free(temp); // Liberação de memória
}

// Funcionalidades de Remoção
void remover_cliente_fila(FilaAtendimento *f, PilhaHistorico *p) {
    if (f->frente == NULL) {
        printf("\nFila de espera vazia!\n");
        return;
    }
    
    char nome_busca[100];
    printf("Digite o nome do cliente que deseja remover da fila: ");
    scanf(" %[^\n]", nome_busca);

    Cliente *atual = f->frente;
    Cliente *anterior = NULL;
    int achou = 0;

    while (atual != NULL) {
        if (strcasecmp(atual->nome, nome_busca) == 0) {
            achou = 1;
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }

    if (!achou) {
        printf("Cliente %s nao encontrado na fila.\n", nome_busca);
        return;
    }

    if (anterior == NULL) { // Remover o primeiro da fila
        f->frente = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    if (atual == f->tras) { // Se era o último, atualiza o tras
        f->tras = anterior;
    }

    free(atual); // Liberação de memória 
    empilhar_log(p, "Cliente removido da fila por nome");
    printf("Cliente %s removido com sucesso.\n", nome_busca);
}

void remover_venda(ListaConcessionaria *c, PilhaHistorico *p) {
    if (c->fim == NULL) {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }
    char mat[13], nf[50];
    printf("\n--- Remover Venda ---\n");
    printf("Matricula do vendedor: "); scanf("%12s", mat);
    printf("Nota Fiscal da venda a remover: "); scanf(" %[^\n]", nf);

    Vendedor *v = c->fim->prox;
    int achou_vendedor = 0;
    do {
        if (strcmp(v->matricula, mat) == 0) {
            achou_vendedor = 1;
            break;
        }
        v = v->prox;
    } while (v != c->fim->prox);

    if (!achou_vendedor) {
        printf("Vendedor nao encontrado.\n");
        return;
    }

    Venda *atual = v->inicio_vendas;
    int achou_venda = 0;
    while (atual != NULL) {
        if (strcmp(atual->nota_fiscal, nf) == 0) {
            achou_venda = 1;
            break;
        }
        atual = atual->prox;
    }

    if (!achou_venda) {
        printf("Venda NF %s nao encontrada.\n", nf);
        return;
    }

    if (atual->ant == NULL) v->inicio_vendas = atual->prox;
    else atual->ant->prox = atual->prox;

    if (atual->prox == NULL) v->fim_vendas = atual->ant;
    else atual->prox->ant = atual->ant;

    free(atual);
    v->qtd_vendas--;
    empilhar_log(p, "Venda removida");
    printf("Venda removida com sucesso!\n");
}

// Funcionalidades de Cadastro e Busca

void cadastrar_vendedor(ListaConcessionaria *c, PilhaHistorico *p) {
    Vendedor *novo = (Vendedor*)malloc(sizeof(Vendedor));
    printf("\n--- Cadastro de Vendedor ---\n");
    printf("Nome: "); scanf(" %[^\n]", novo->nome);
    printf("CPF (11 digitos): "); scanf("%11s", novo->cpf);
    printf("Matricula (12 digitos): "); scanf("%12s", novo->matricula);

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
    printf("Vendedor cadastrado com sucesso!\n");
}

void remover_vendedor(ListaConcessionaria *c, PilhaHistorico *p) {
    if (c->fim == NULL) return;
    char mat[13];
    printf("Matricula para remover: "); scanf("%12s", mat);

    Vendedor *atual = c->fim->prox;
    Vendedor *anterior = c->fim;
    int achou = 0;

    do {
        if (strcmp(atual->matricula, mat) == 0) { achou = 1; break; }
        anterior = atual;
        atual = atual->prox;
    } while (atual != c->fim->prox);

    if (achou) {
        Venda *v = atual->inicio_vendas;
        while (v) { Venda *t = v; v = v->prox; free(t); }
        if (c->qtd_vendedores == 1) c->fim = NULL;
        else { anterior->prox = atual->prox; if (atual == c->fim) c->fim = anterior; }
        free(atual);
        c->qtd_vendedores--;
        empilhar_log(p, "Vendedor removido");
    }
}

void listar_vendedores(ListaConcessionaria *c) {
    if (!c->fim) return;
    Vendedor *atual = c->fim->prox;
    do {
        printf("Nome: %s | Matrícula: %s | Vendas: %d\n", atual->nome, atual->matricula, atual->qtd_vendas);
        atual = atual->prox;
    } while (atual != c->fim->prox);
}

void registrar_venda(ListaConcessionaria *c, PilhaHistorico *p) {
    if (!c->fim) return;
    char mat[13];
    printf("Matrícula do vendedor: "); scanf("%12s", mat);
    
    Vendedor *v = c->fim->prox;
    int achou = 0;
    do {
        if (strcmp(v->matricula, mat) == 0) { achou = 1; break; }
        v = v->prox;
    } while (v != c->fim->prox);

    if (achou) {
        Venda *nova = (Venda*)malloc(sizeof(Venda));
        printf("Nota Fiscal: "); scanf(" %[^\n]", nova->nota_fiscal);
        printf("Valor: "); scanf("%f", &nova->valor);
        printf("Marca: "); scanf(" %[^\n]", nova->marca);
        printf("Modelo: "); scanf(" %[^\n]", nova->modelo);
        printf("Ano: "); scanf("%d", &nova->ano);
        nova->prox = NULL; nova->ant = v->fim_vendas;
        if (!v->inicio_vendas) v->inicio_vendas = nova;
        else v->fim_vendas->prox = nova;
        v->fim_vendas = nova;
        v->qtd_vendas++;
        empilhar_log(p, "Venda realizada");
    }
}

void exibir_total_vendas_por_vendedor(ListaConcessionaria *c) {
    if (!c->fim) return;
    Vendedor *atual = c->fim->prox;
    do {
        float total = 0;
        Venda *v = atual->inicio_vendas;
        while (v) { total += v->valor; v = v->prox; }
        printf("Vendedor: %s | Total: R$ %.2f\n", atual->nome, total);
        atual = atual->prox;
    } while (atual != c->fim->prox);
}

void exibir_total_vendas_concessionaria(ListaConcessionaria *c) {
    if (!c->fim) return;
    float total = 0;
    Vendedor *atual = c->fim->prox;
    do {
        Venda *v = atual->inicio_vendas;
        while (v) { total += v->valor; v = v->prox; }
        atual = atual->prox;
    } while (atual != c->fim->prox);
    printf("Faturamento Total: R$ %.2f\n", total);
}

void exibir_vendedor_mais_lucrativo(ListaConcessionaria *c) {
    if (!c->fim) return;
    Vendedor *destaque = NULL; float maior = -1;
    Vendedor *atual = c->fim->prox;
    do {
        float total = 0;
        Venda *v = atual->inicio_vendas;
        while (v) { total += v->valor; v = v->prox; }
        if (total > maior) { maior = total; destaque = atual; }
        atual = atual->prox;
    } while (atual != c->fim->prox);
    if (destaque) printf("Destaque: %s com R$ %.2f\n", destaque->nome, maior);
}

void exibir_vendas_por_marca(ListaConcessionaria *c) {
    if (!c->fim) return;
    char marca[50]; printf("Marca: "); scanf(" %[^\n]", marca);
    Vendedor *atual = c->fim->prox;
    do {
        Venda *v = atual->inicio_vendas;
        while (v) {
            if (strcasecmp(v->marca, marca) == 0)
                printf("Vendedor: %s | Modelo: %s | Valor: %.2f\n", atual->nome, v->modelo, v->valor);
            v = v->prox;
        }
        atual = atual->prox;
    } while (atual != c->fim->prox);
}

// Array Estático para Metas
void relatorio_metas() {
    float metas[4] = {100000.0, 150000.0, 200000.0, 250000.0}; 
    printf("\n--- Metas Trimestrais ---\n");
    for(int i=0; i<4; i++) printf("Trimestre %d: R$ %.2f\n", i+1, metas[i]);
}

void exibir_historico(PilhaHistorico *p) {
    Log *atual = p->topo;
    while(atual) { printf(" -> [%s]\n", atual->acao); atual = atual->prox; }
}

void limpar_tudo(ListaConcessionaria *c, FilaAtendimento *f, PilhaHistorico *p) {
    // Implementar limpeza de todas as estruturas
    printf("\nLimpando memoria...\n");
}

int main() {
    ListaConcessionaria conc; FilaAtendimento fila; PilhaHistorico pilha;
    inicializa_concessionaria(&conc); inicializa_fila(&fila); inicializa_pilha(&pilha);
    int opt; char aux_nome[100];

    do {
        printf("\n=== Sistema de Gerenciamento da Concessionária ===\n");
        printf(" 1. Cadastrar Vendedor       |  8. Vendas por Marca\n");
        printf(" 2. Remover Vendedor         |  9. Chegada Cliente (Fila)\n");
        printf(" 3. Listar Vendedores        | 10. Atender Cliente (Fila)\n");
        printf(" 4. Registrar Venda          | 11. Exibir Historico (Pilha)\n");
        printf(" 5. Total Vendas (Por Vend.) | 12. Metas (Array)\n");
        printf(" 6. Total Vendas (Conces.)   | 13. Remover Venda\n");
        printf(" 7. Vendedor Mais Lucrativo  | 14. Remover Cliente da Fila\n");
        printf(" 0. Sair\nEscolha uma opcao: "); 
        scanf("%d", &opt);

        switch(opt) {
            case 1: cadastrar_vendedor(&conc, &pilha); break;
            case 2: remover_vendedor(&conc, &pilha); break;
            case 3: listar_vendedores(&conc); break;
            case 4: registrar_venda(&conc, &pilha); break;
            case 5: exibir_total_vendas_por_vendedor(&conc); break;
            case 6: exibir_total_vendas_concessionaria(&conc); break;
            case 7: exibir_vendedor_mais_lucrativo(&conc); break;
            case 8: exibir_vendas_por_marca(&conc); break;
            case 9: printf("Nome Cliente: "); scanf(" %[^\n]", aux_nome); enfileirar_cliente(&fila, aux_nome); break;
            case 10: chamar_proximo(&fila); break;
            case 11: exibir_historico(&pilha); break;
            case 12: relatorio_metas(); break;
            case 13: remover_venda(&conc, &pilha); break; 
            case 14: remover_cliente_fila(&fila, &pilha); break;
            case 0: break;
            default: printf("Opção inválida!\n");
        }
    } while(opt != 0);

    limpar_tudo(&conc, &fila, &pilha);
    return 0;
}
