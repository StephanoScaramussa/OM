#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#define MAX_PONTOS 1000
#define POS 4

typedef struct tSolucao {
    int vet_pos_cand[POS][MAX_PONTOS];
    int fo;
}Solucao;

void le_dados(FILE*f);
void escreve_dados();
void calcula_fo();
void heu_con_ale();