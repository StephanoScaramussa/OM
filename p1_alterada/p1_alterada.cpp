#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "p1_alterada.h"

#define MAX(X,Y) ((X > Y) ? X : Y)

int main()
{
    srand(time(NULL));
    char arq[50];
    strcpy(arq, "..//instancia.txt");
    ler_dados(arq);

    Solucao sol;
    sol.mat_sol[0][0] = 1;
    sol.mat_sol[0][1] = 3;
    sol.mat_sol[0][2] = 5;
    sol.vet_qtd[0] = 3;

    sol.mat_sol[1][0] = 2;
    sol.mat_sol[1][1] = 4;
    sol.vet_qtd[1] = 2;

    heu_con_gul(sol);    
    calcular_fo(sol);
    strcpy(arq, "");
    escrever(sol, arq);
    
    gerar_vizinha(sol);
    calcular_fo(sol);
    escrever(sol, arq);

    return 0;
}

void heu_con_gul(Solucao& s)
{
    memset(&s.vet_qtd, 0, sizeof(s.vet_qtd));
    int vei = 0;
    for (int i = 0; i < num_cli; i++)
    {
        s.mat_sol[vei][s.vet_qtd[vei]] = i + 1;
        s.vet_qtd[vei]++;
        vei++;
        if (vei == num_vei)
            vei = 0;
    }
}

void gerar_vizinha(Solucao& s)
{
    int op = rand() % 2;
    if (op == 0)
    {
        int vOr, vDs;
        do
            vOr = rand() % num_vei;
        while (s.vet_qtd[vOr] == 0);
        int pos = rand() % s.vet_qtd[vOr];
        int cli = s.mat_sol[vOr][pos];
        vDs = rand() % num_vei;
        remover_cli(s, vOr, pos);
        inserir_cli(s, vDs, cli);
    }
    else
    {
        int vOr, vDs;
        do
            vOr = rand() % num_vei;
        while (s.vet_qtd[vOr] == 0);
        do
            vDs = rand() % num_vei;
        while (s.vet_qtd[vDs] == 0);
        int pos_Or = rand() % s.vet_qtd[vOr];
        int pos_Ds = rand() % s.vet_qtd[vDs];
        int aux = s.mat_sol[vOr][pos_Or];
        s.mat_sol[vOr][pos_Or] = s.mat_sol[vDs][pos_Ds];
        s.mat_sol[vDs][pos_Ds] = aux;
    }   
}

void inserir_cli(Solucao& s, const int& vei, const int& cli)
{
    int pos = rand() % (s.vet_qtd[vei] + 1);
    for (int i = s.vet_qtd[vei]; i > pos; i--)
        s.mat_sol[vei][i] = s.mat_sol[vei][i - 1];
    s.mat_sol[vei][pos] = cli;
    s.vet_qtd[vei]++;
}

void remover_cli(Solucao& s, const int& vei, const int& pos)
{
    for (int i = pos; i < s.vet_qtd[vei]; i++)
        s.mat_sol[vei][i] = s.mat_sol[vei][i + 1];
    s.vet_qtd[vei]--;
}

void calcular_fo(Solucao& s)
{
    s.fo = 0;
    for (int k = 0; k < num_vei; k++)
    {
        if (s.vet_qtd[k] == 0)
            continue;
        int carga = vet_demandas[0] + vet_demandas[s.mat_sol[k][0]];
        s.fo += mat_custo[0][s.mat_sol[k][0]];
        for (int i = 1; i < s.vet_qtd[k]; i++)
        {
            s.fo += mat_custo[s.mat_sol[k][i-1]][s.mat_sol[k][i]];
            carga += vet_demandas[s.mat_sol[k][i]];
        }
        s.fo += mat_custo[s.mat_sol[k][s.vet_qtd[k]-1]][0];
        //if (carga > vet_cap_vei[k])
        //    s.fo += PESO_CAP * (carga - vet_cap_vei[k]);
        s.fo += PESO_CAP * MAX(0, carga - vet_cap_vei[k]);
    }
}

void escrever(Solucao&s, char* arq)
{
    FILE* f;
    if (strcmp(arq, "") == 0)
        f = stdout;
    else
        f = fopen(arq, "w");
    fprintf(f, "FO: %.2f\n", s.fo);
    for (int k = 0; k < num_vei; k++)
    {
        int carga = vet_demandas[0];
        fprintf(f, "V %d -> 0 -> ", k + 1);
        for (int i = 0; i < s.vet_qtd[k]; i++)
        {
            fprintf(f, "%d -> ", s.mat_sol[k][i]);
            carga += vet_demandas[s.mat_sol[k][i]];
        }
        fprintf(f, "0\tCarga: %d\n", carga);
    }
    if (strcmp(arq, "") != 0)
        fclose(f);
}

void ler_dados(char* arq)
{
    int aux;
    int vet_X[MAX_CLI];
    int vet_Y[MAX_CLI];
    FILE* f = fopen(arq, "r");
    fscanf(f, "%d %d", &num_cli, &num_vei);
    for (int i = 0; i <= num_cli; i++)
        fscanf(f, "%d %d %d %d", &aux, &vet_X[i], &vet_Y[i], &vet_demandas[i]);
    for (int k = 0; k < num_vei; k++)
        fscanf(f, "%d", &vet_cap_vei[k]);
    fclose(f);
    //---
    for (int i = 0; i <= num_cli; i++)
        for (int j = 0; j <= num_cli; j++)
            mat_custo[i][j] = sqrt(
                (vet_X[i] - vet_X[j]) * (vet_X[i] - vet_X[j]) +
                (vet_Y[i] - vet_Y[j]) * (vet_Y[i] - vet_Y[j])
            );
	// Teste
    printf("%d %d\n", num_cli, num_vei);
    for (int i = 0; i <= num_cli; i++)
    {
        for (int j = 0; j <= num_cli; j++)
            printf("%.2f ", mat_custo[i][j]);
        printf("\n");
    }
}