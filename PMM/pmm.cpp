#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>

#include "pmm.h"

#define MAX(X,Y) ((X > Y) ? X : Y)

int main()
{
    //srand(time(NULL));
    char arq[50];
    strcpy(arq, "..\\pmm3.txt");
    ler_dados(arq);
    //strcpy(arq, "");
    //testar_dados(arq);

    ordenar_objetos();
    //for (int j = 0; j < num_obj; j++)
    //    printf("%d  ", vet_ind_obj_ord[j]);

    clock_t h;
    double tempoA, tempoG, tempoAG;
    Solucao solA, solG, solAG;
    
    h = clock();
    for (int r = 0; r < 10000; r++)
        heu_con_ale(solA);
    tempoA = ((double)(clock() - h)) / CLOCKS_PER_SEC;
    
    h = clock();
    for (int r = 0; r < 10000; r++)
        heu_con_gul(solG);
    tempoG = ((double)(clock() - h)) / CLOCKS_PER_SEC;
    
    h = clock();
    for (int r = 0; r < 10000; r++)
        heu_con_ale_gul(solAG, 10);
    tempoAG = ((double)(clock() - h)) / CLOCKS_PER_SEC;

    calcular_FO(solA);
    calcular_FO(solG);
    calcular_FO(solAG);
    printf("FOA: %d\tTempo: %.5f\n", solA.fo, tempoA);
    printf("FOG: %d\tTempo: %.5f\n", solG.fo, tempoG);
    printf("FOAG: %d\tTempo: %.5f\n", solAG.fo, tempoAG);
    

    /*
    SolucaoBIN solB;
    memset(&solB, 0, sizeof(SolucaoBIN));
    solB.mat_sol[0][0] = 1;
    solB.mat_sol[0][1] = 1;
    //solB.mat_sol[1][0] = 1;
    solB.mat_sol[1][2] = 1;

    clock_t h;
    h = clock();
    for (int r = 0; r < 1000; r++)
        calcular_FOBIN(solB);
    double tempo = ((double)(clock() - h)) / CLOCKS_PER_SEC;
    printf("FO: %d\tTempo: %.5f\n", solB.fo, tempo);
    

    Solucao sol;
    sol.vet_sol[0] = 0;
    sol.vet_sol[1] = 0;
    sol.vet_sol[2] = 1;
    sol.vet_sol[3] = -1;

    h = clock();
    for (int r = 0; r < 1000; r++)
        calcular_FO(sol);
    tempo = ((double)(clock() - h)) / CLOCKS_PER_SEC;
    printf("FO: %d\tTempo: %.5f\n", sol.fo, tempo);


    for (int i = 0; i < num_moc; i++)
        for (int j = 0; j < num_obj; j++)
            solB.mat_sol[i][j] = rand() % 2;
    calcular_FOBIN(solB);
    //escrever_solBIN(solB);
    
    for (int j = 0; j < num_obj; j++)
        sol.vet_sol[j] = rand() % (num_moc + 1) - 1;
    calcular_FO(sol);
    //escrever_sol(sol);
    */

    return 0;
}

void heu_con_ale(Solucao& s)
{
    for (int j = 0; j < num_obj; j++)
        s.vet_sol[j] = rand() % (num_moc + 1) - 1;
}

void heu_con_gul(Solucao& s)
{
    memset(&s.vet_pesos, 0, sizeof(s.vet_pesos));
    memset(&s.vet_sol, -1, sizeof(s.vet_sol));
    for (int j = 0; j < num_obj; j++)
    {
        int obj = vet_ind_obj_ord[j];
        for (int i = 0; i < num_moc; i++)
        {
            if (vet_pes_obj[obj] + s.vet_pesos[i] <= vet_cap_moc[i])
            {
                s.vet_sol[obj] = i;
                s.vet_pesos[i] += vet_pes_obj[obj];
                break;
            }
        }
    }
}

void heu_con_ale_gul(Solucao& s, const double per_ale)
{
    int vet_aux[MAX_OBJ];
    memcpy(&vet_aux, &vet_ind_obj_ord, sizeof(vet_ind_obj_ord));
    int qtde = MAX(1, (per_ale / 100) * num_obj);
    for (int i = 0; i < qtde; i++)
    {
        int pos = i + rand() % (num_obj - i);
        int aux = vet_aux[i];
        vet_aux[i] = vet_aux[pos];
        vet_aux[pos] = aux;
    }
    // codigo da gulosa
    memset(&s.vet_pesos, 0, sizeof(s.vet_pesos));
    memset(&s.vet_sol, -1, sizeof(s.vet_sol));
    for (int j = 0; j < num_obj; j++)
    {
        int obj = vet_aux[j]; // troquei o vetor ordenado pelo embaralhado
        for (int i = 0; i < num_moc; i++)
        {
            if (vet_pes_obj[obj] + s.vet_pesos[i] <= vet_cap_moc[i])
            {
                s.vet_sol[obj] = i;
                s.vet_pesos[i] += vet_pes_obj[obj];
                break;
            }
        }
    }
}

void ordenar_objetos()
{
    for (int j = 0; j < num_obj; j++)
        vet_ind_obj_ord[j] = j;
    int flag = 1;
    while (flag)
    {
        flag = 0;
        for (int j = 0; j < num_obj - 1; j++)
        {
            if ((double)vet_val_obj[vet_ind_obj_ord[j]] / vet_pes_obj[vet_ind_obj_ord[j]] <
                (double)vet_val_obj[vet_ind_obj_ord[j + 1]] / vet_pes_obj[vet_ind_obj_ord[j + 1]])
            {
                int aux = vet_ind_obj_ord[j];
                vet_ind_obj_ord[j] = vet_ind_obj_ord[j + 1];
                vet_ind_obj_ord[j + 1] = aux;
                flag = 1;
            }
        }
    }
}














void calcular_FO(Solucao& s)
{
    s.fo = 0;
    memset(&s.vet_pesos, 0, sizeof(s.vet_pesos));
    for (int j = 0; j < num_obj; j++)
    {
        int moc = s.vet_sol[j];
        if (moc == -1)
            continue;
        s.fo += vet_val_obj[j];
        s.vet_pesos[moc] += vet_pes_obj[j];
    }
    for (int i = 0; i < num_moc; i++)
        s.fo -= PESO_CAP * MAX(0, s.vet_pesos[i] - vet_cap_moc[i]);
}

void escrever_sol(Solucao& s)
{
    printf("\n\nFO: %d\n", s.fo);
    for (int j = 0; j < num_obj; j++)
        printf("%d ", s.vet_sol[j]);
}

void calcular_FOBIN(SolucaoBIN& s)
{
    s.fo = 0;
    for (int i = 0; i < num_moc; i++)
    {
        int peso = 0;
        for (int j = 0; j < num_obj; j++)
        {
            //if (s.mat_sol[i][j] == 1)
            //    s.fo += vet_val_obj[j];
            s.fo += vet_val_obj[j] * s.mat_sol[i][j];
            peso += vet_pes_obj[j] * s.mat_sol[i][j];
        }
        //if (peso > vet_cap_moc[i])
        //    s.fo -= PESO_CAP * (peso - vet_cap_moc[i]);
        s.fo -= PESO_CAP * MAX(0, peso - vet_cap_moc[i]);
    }
    //---
    for (int j = 0; j < num_obj; j++)
    {
        int qtd = 0;
        for (int i = 0; i < num_moc; i++)
            qtd += s.mat_sol[i][j];
        s.fo -= PESO_DUP * MAX(0, qtd - 1);
    }
}

void escrever_solBIN(SolucaoBIN& s)
{
    printf("\n\nFO: %d\n", s.fo);
    for (int i = 0; i < num_moc; i++)
    {
        for (int j = 0; j < num_obj; j++)
            printf("%d ", s.mat_sol[i][j]);
        printf("\n");
    }
}

void testar_dados(char* arq)
{
    FILE* f;
    if (strcmp(arq, "") == 0)
        f = stdout;
    else
        f = fopen(arq, "w");
    fprintf(f, "%d %d\n", num_obj, num_moc);
    for (int j = 0; j < num_obj; j++)
        fprintf(f, "%d ", vet_val_obj[j]);
    fprintf(f, "\n");
    for (int j = 0; j < num_obj; j++)
        fprintf(f, "%d ", vet_pes_obj[j]);
    fprintf(f, "\n");
    for (int i = 0; i < num_moc; i++)
        fprintf(f, "%d ", vet_cap_moc[i]);
    if (strcmp(arq, "") != 0)
        fclose(f);
}

void ler_dados(char* arq)
{
    FILE* f = fopen(arq, "r");
    fscanf(f, "%d %d", &num_obj, &num_moc);
    for (int j = 0; j < num_obj; j++)
        fscanf(f, "%d", &vet_val_obj[j]);
    for (int j = 0; j < num_obj; j++)
        fscanf(f, "%d", &vet_pes_obj[j]);
    for (int i = 0; i < num_moc; i++)
        fscanf(f, "%d", &vet_cap_moc[i]);
    fclose(f);
}