#ifndef PAB_H
#define PAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>

#define MAX(X,Y) ((X > Y) ? X : Y)

// Limites maximos da especificacao do PAB (|N| <= 100 navios, |K| <= 20 bercos)
#define MAX_NAV 100
#define MAX_BER 20

// Constantes de penalizacao (calibracao de violacao de restricoes)
const int PESO_PRAZO     = 100;    // Penalidade por ultrapassar o horario limite do navio (bi)
const int PESO_FECH      = 100;    // Penalidade por ultrapassar o horario de fechamento do berco (ck)
const int PESO_INCOMP    = 10000;  // Penalidade se o berco nao puder atender o navio (t_i^k == 0)
const int PESO_NAO_ATEND = 10000;  // Penalidade se o navio nao for atendido
const int PESO_DUP       = 10000;  // Penalidade se o navio for atendido em mais de um berco

// ============================================================================
// ESTRUTURA DA SOLUCAO (PROVA I - Questao 2)
// Armazena todas as decisoes do problema: onde e quando cada navio atraca
// ============================================================================
typedef struct tSolucao {
    int mat_sol[MAX_BER][MAX_NAV];    // mat_sol[k][p]: navio que ocupa a posicao p na fila do berco k
    int vet_qtd[MAX_BER];             // quantidade de navios alocados ao berco k
    int vet_tempo_atracacao[MAX_NAV]; // horario de atracacao T_i^k de cada navio i (calculado na FO)
    int fo;                           // valor da funcao objetivo (tempo total de servico + penalidades)
} Solucao;

// ============================================================================
// VARIAVEIS GLOBAIS COM DADOS DA INSTANCIA (PROVA I - Questao 1)
// ============================================================================
int num_nav;                             // Numero total de navios (|N|)
int num_ber;                             // Numero total de bercos (|K|)
int mat_tempo_atend[MAX_BER][MAX_NAV];   // Tempo de atendimento t_i^k do navio i no berco k (0 = incompativel)
int vet_abertura_ber[MAX_BER];           // Horario de abertura o^k do berco k
int vet_fechamento_ber[MAX_BER];         // Horario de fechamento c^k do berco k
int vet_tempo_chegada[MAX_NAV];          // Horario de chegada a_i de cada navio i
int vet_tempo_limite[MAX_NAV];           // Horario limite para saida b_i de cada navio i

// Vetor auxiliar para ordenacao de navios (utilizado na heuristica gulosa da Prova 2)
int vet_ind_nav_ord[MAX_NAV];

// ============================================================================
// METODOS DA PROVA I
// ============================================================================
void ler_dados(char* arq);                     // Questao 1: Leitura de instancia
void testar_dados(char* arq);                  // Escrita/verificacao dos dados lidos (modelo pmm)
void escrever_sol(Solucao& s, char* arq);      // Questao 3: Escrita da solucao (tela se arq="" ou arquivo)
void calcular_fo(Solucao& s);                  // Questao 4: Calculo da Funcao Objetivo

// ============================================================================
// METODOS AUXILIARES E METODOS PREVISTOS PARA A PROVA II
// ============================================================================
void inserir_navio(Solucao& s, const int& berco, const int& navio, const int& pos);
void remover_navio(Solucao& s, const int& berco, const int& pos);
void ordenar_navios();

void gerar_vizinha(Solucao& s);                               // Prova 2 - Questao 1
void heu_con_ale(Solucao& s);                                 // Prova 2 - Questao 2
void heu_con_gul(Solucao& s);                                 // Prova 2 - Questao 3
void heu_con_ale_gul(Solucao& s, const double per_ale);       // Prova 2 - Questao 4

#endif
