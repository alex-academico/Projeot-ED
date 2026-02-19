#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Venda {
    char nota_fiscal[50];
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

// Fila - Processo de espera para clientes
typedef struct Cliente {
    char nome[100];
    struct Cliente *prox;
} Cliente;

typedef struct {
    Cliente *frente;
    Cliente *tras;
} FilaAtendimento;

// Pilha - Histórico de Ações
typedef struct Log {
    char acao[100];
    struct Log *prox;
} Log;

typedef struct {
    Log *topo;
} PilhaHistorico;

// --- Inicializações ---

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

// --- Funções Auxiliares (Pilha e Fila) ---

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
        printf("Todos os clientes foram atendidos!\n");
        return;
    }
    Cliente *temp = f->frente;
    printf("Atendendo cliente: %s\n", temp->nome);
    f->frente = f->frente->prox;
    if (f->frente == NULL) f->tras = NULL;
    free(temp);
}

// --- Funcionalidades do Sistema ---

void cadastrar_vendedor(ListaConcessionaria *c, PilhaHistorico *p) {
    char nome[100], cpf[12];
    int mat;
    printf("\n--- Cadastro de Vendedor ---\n");
    printf("Nome: "); scanf(" %[^\n]", nome);
    printf("CPF: "); scanf("%s", cpf);
    printf("Matrícula: "); scanf("%d", &mat);

    // Verifica duplicidade de matrícula
    if (c->fim != NULL) {
        Vendedor *atual = c->fim->prox;
        do {
            if (atual->matricula == mat) {
                printf("Erro: Matrícula já existente!\n");
                return;
            }
            atual = atual->prox;
        } while (atual != c->fim->prox);
    }

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
    printf("Vendedor cadastrado com sucesso!\n");
}

void remover_vendedor(ListaConcessionaria *c, PilhaHistorico *p) {
    if (c->fim == NULL) {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    int mat;
    printf("\n--- Remover Vendedor ---\n");
    printf("Matrícula do vendedor a ser removido: "); scanf("%d", &mat);

    Vendedor *atual = c->fim->prox;
    Vendedor *anterior = c->fim;
    int achou = 0;

    do {
        if (atual->matricula == mat) {
            achou = 1;
            break;
        }
        anterior = atual;
        atual = atual->prox;
    } while (atual != c->fim->prox);

    if (!achou) {
        printf("Vendedor não encontrado.\n");
        return;
    }

    // Liberar vendas (lista duplamente ligada)
    Venda *v = atual->inicio_vendas;
    while (v != NULL) {
        Venda *temp = v;
        v = v->prox;
        free(temp);
    }

    // Atualizar lista circular
    if (c->qtd_vendedores == 1) {
        c->fim = NULL;
    } else {
        anterior->prox = atual->prox;
        if (atual == c->fim) {
            c->fim = anterior;
        }
    }

    free(atual);
    c->qtd_vendedores--;
    empilhar_log(p, "Vendedor removido");
    printf("Vendedor removido com sucesso.\n");
}

void listar_vendedores(ListaConcessionaria *c) {
    if (c->fim == NULL) {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Vendedores ---\n");
    Vendedor *atual = c->fim->prox;
    do {
        printf("Nome: %s | CPF: %s | Matrícula: %d | Vendas: %d\n", 
               atual->nome, atual->cpf, atual->matricula, atual->qtd_vendas);
        atual = atual->prox;
    } while (atual != c->fim->prox);
}

void registrar_venda(ListaConcessionaria *c, PilhaHistorico *p) {
    if (!c->fim) {
        printf("Nenhum vendedor cadastrado para registrar venda.\n");
        return;
    }
    
    int mat;
    printf("\n--- Registro de Venda ---\n");
    printf("Matrícula do vendedor: "); 
    scanf("%d", &mat);
    scanf("%*c");
    
    Vendedor *v = c->fim->prox;
    int achou = 0;
    do {
        if (v->matricula == mat) { achou = 1; break; }
        v = v->prox;
    } while (v != c->fim->prox);

    if (achou) {
        Venda *nova = (Venda*)malloc(sizeof(Venda));
        printf("Nota Fiscal: "); scanf("%[^\n]", nova->nota_fiscal);
        printf("Valor: "); scanf("%f", &nova->valor);
        scanf("%*c");
        printf("Marca: "); scanf(" %[^\n]", nova->marca);
        printf("Modelo: "); scanf(" %[^\n]", nova->modelo);
        printf("Ano: "); scanf("%d", &nova->ano);
        
        nova->prox = NULL;
        nova->ant = v->fim_vendas;

        if (v->inicio_vendas == NULL) v->inicio_vendas = nova;
        else v->fim_vendas->prox = nova;
        
        v->fim_vendas = nova;
        v->qtd_vendas++;
        empilhar_log(p, "Venda realizada com sucesso");
        printf("Venda registrada com sucesso para o vendedor %s.\n", v->nome);
    } else {
        printf("Vendedor não encontrado.\n");
    }
}

void exibir_total_vendas_por_vendedor(ListaConcessionaria *c) {
    if (c->fim == NULL) {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    printf("\n--- Total de Vendas por Vendedor ---\n");
    Vendedor *atual = c->fim->prox;
    do {
        float total = 0.0;
        Venda *v = atual->inicio_vendas;
        while (v != NULL) {
            total += v->valor;
            v = v->prox;
        }
        printf("Vendedor: %s (Matrícula: %d)\n", atual->nome, atual->matricula);
        printf("Quantidade: %d | Total arrecadado: R$ %.2f\n\n", atual->qtd_vendas, total);
        atual = atual->prox;
    } while (atual != c->fim->prox);
}

void exibir_total_vendas_concessionaria(ListaConcessionaria *c) {
    if (c->fim == NULL) {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    float total_geral = 0.0;
    int qtd_total = 0;

    Vendedor *atual = c->fim->prox;
    do {
        Venda *v = atual->inicio_vendas;
        while (v != NULL) {
            total_geral += v->valor;
            qtd_total++;
            v = v->prox;
        }
        atual = atual->prox;
    } while (atual != c->fim->prox);

    printf("\n--- Desempenho Global da Concessionária ---\n");
    printf("Total de veículos vendidos: %d\n", qtd_total);
    printf("Faturamento total: R$ %.2f\n", total_geral);
}

void exibir_vendedor_mais_lucrativo(ListaConcessionaria *c) {
    if (c->fim == NULL) {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    Vendedor *mais_lucrativo = NULL;
    float maior_total = -1.0;

    Vendedor *atual = c->fim->prox;
    do {
        float total = 0.0;
        Venda *v = atual->inicio_vendas;
        while (v != NULL) {
            total += v->valor;
            v = v->prox;
        }

        if (total > maior_total) {
            maior_total = total;
            mais_lucrativo = atual;
        }
        atual = atual->prox;
    } while (atual != c->fim->prox);

    if(mais_lucrativo) {
        printf("\n--- Vendedor Destaque (Mais Lucrativo) ---\n");
        printf("Nome: %s | Matrícula: %d\n", mais_lucrativo->nome, mais_lucrativo->matricula);
        printf("Faturamento Gerado: R$ %.2f\n", maior_total);
    }
}

void exibir_vendas_por_marca(ListaConcessionaria *c) {
    if (c->fim == NULL) {
        printf("Nenhum vendedor cadastrado.\n");
        return;
    }

    char marca_busca[50];
    
    printf("\n--- Pesquisar Vendas por Marca ---\n");
    printf("Digite a marca desejada: "); scanf(" %[^\n]", marca_busca);

    int encontrou = 0;
    Vendedor *atual = c->fim->prox;
    do {
        Venda *v = atual->inicio_vendas;
        while (v != NULL) {
            if (strcasecmp(v->marca, marca_busca) == 0) {
                printf("\nVend. %s (Matrícula: %d)\n", atual->nome, atual->matricula);
                printf("  -> NF: %s | Modelo: %s (%d) | Valor: R$ %.2f\n", 
                       v->nota_fiscal, v->modelo, v->ano, v->valor);
                encontrou = 1;
            }
            v = v->prox;
        }
        atual = atual->prox;
    } while (atual != c->fim->prox);

    if (!encontrou) {
        printf("Nenhum veículo da marca '%s' foi vendido.\n", marca_busca);
    }
}

// ARRAY: Uso de Array Estático para Metas
void relatorio_metas() {
    float metas[4] = {100000.0, 150000.0, 200000.0, 250000.0}; 
    printf("\n--- Metas Trimestrais ---\n");
    for(int i=0; i<4; i++) 
        printf("Trimestre %d: R$ %.2f\n", i+1, metas[i]);
}

void exibir_historico(PilhaHistorico *p) {
    Log *atual = p->topo;
    printf("\n--- Histórico de Ações ---\n");
    if(!atual) printf("Nenhuma ação registrada.\n");
    while(atual) {
        printf(" -> [%s]\n", atual->acao);
        atual = atual->prox;
    }
}

// Limpeza de memória abrangente (Evita Memory Leaks)
void limpar_tudo(ListaConcessionaria *c, FilaAtendimento *f, PilhaHistorico *p) {
    printf("\nLimpando memória...\n");

    // 1. Limpar Lista de Vendedores e Vendas
    if (c->fim != NULL) {
        Vendedor *atual = c->fim->prox;
        while (atual != c->fim) {
            Vendedor *temp = atual;
            atual = atual->prox;
            
            // Limpa as vendas do vendedor temp
            Venda *v = temp->inicio_vendas;
            while (v != NULL) {
                Venda *temp_v = v;
                v = v->prox;
                free(temp_v);
            }
            free(temp);
        }
        // Limpa as vendas do último vendedor e o próprio
        Venda *v = c->fim->inicio_vendas;
        while (v != NULL) {
            Venda *temp_v = v;
            v = v->prox;
            free(temp_v);
        }
        free(c->fim);
        c->fim = NULL;
    }

    // 2. Limpar Fila de Clientes
    while (f->frente != NULL) {
        Cliente *temp_c = f->frente;
        f->frente = f->frente->prox;
        free(temp_c);
    }

    // 3. Limpar Pilha de Histórico
    while (p->topo != NULL) {
        Log *temp_l = p->topo;
        p->topo = p->topo->prox;
        free(temp_l);
    }

    printf("Memória totalmente liberada. Encerrando o sistema.\n");
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
        printf("\n=== Sistema de Gerenciamento da Concessionária ===\n");
        printf(" 1. Cadastrar Vendedor       |  8. Vendas por Marca\n");
        printf(" 2. Remover Vendedor         |  9. Ordem de chegada\n");
        printf(" 3. Listar Vendedores        | 10. Atender Cliente\n");
        printf(" 4. Registrar Venda          | 11. Exibir Histórico\n");
        printf(" 5. Total Vendas (Por Vend.) | 12. Metas\n");
        printf(" 6. Total Vendas (Conces.)   |  0. Sair\n");
        printf(" 7. Vendedor Mais Lucrativo  | \n");
        printf("Escolha uma opção: "); 
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
            case 9: 
                printf("Nome Cliente: "); scanf(" %[^\n]", aux_nome); 
                enfileirar_cliente(&fila, aux_nome); 
                break;
            case 10: chamar_proximo(&fila); break;
            case 11: exibir_historico(&pilha); break;
            case 12: relatorio_metas(); break;
            case 0: break;
            default: printf("Opção inválida!\n");
        }
    } while(opt != 0);

    limpar_tudo(&conc, &fila, &pilha);
    return 0;
}


