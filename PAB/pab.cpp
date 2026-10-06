#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>

#include "pab.h"

int main()
{
    srand(time(NULL));

    char arq[100];
    // Tenta abrir a instancia na pasta atual ou na pasta PAB
    strcpy(arq, "i01.txt");
    FILE* f_teste = fopen(arq, "r");
    if (!f_teste)
        strcpy(arq, "..\\PAB\\i01.txt");
    else
        fclose(f_teste);

    printf("Lendo dados da instancia: %s ...\n", arq);
    ler_dados(arq);
    printf("Instancia carregada com sucesso! Navios: %d, Bercos: %d\n\n", num_nav, num_ber);

    // Opcional: testar impressao dos dados lidos (descomente para verificar)
    // testar_dados("");

    // ========================================================================
    // TESTE DA PROVA I: Criacao de uma solucao inicial para validar o modelo
    // (Seguindo o modelo estrutural de p1_alterada.cpp e pmm.cpp)
    // ========================================================================
    Solucao sol_teste;
    memset(&sol_teste, 0, sizeof(Solucao));

    // Exemplo de distribuicao simples: aloca cada navio sequencialmente em um berco compativel
    int berco_atual = 0;
    for (int i = 0; i < num_nav; i++)
    {
        // Procura um berco compativel para o navio i (onde t_i^k > 0)
        int tentativas = 0;
        while (mat_tempo_atend[berco_atual][i] == 0 && tentativas < num_ber)
        {
            berco_atual = (berco_atual + 1) % num_ber;
            tentativas++;
        }

        // Insere o navio no berco_atual
        sol_teste.mat_sol[berco_atual][sol_teste.vet_qtd[berco_atual]] = i;
        sol_teste.vet_qtd[berco_atual]++;

        // Alterna para o proximo berco para balancear
        berco_atual = (berco_atual + 1) % num_ber;
    }

    // Calcula a Funcao Objetivo da solucao de teste
    calcular_fo(sol_teste);

    // Escreve a solucao na tela
    strcpy(arq, "");
    escrever_sol(sol_teste, arq);

    // Escreve a solucao em arquivo
    strcpy(arq, "solucao_teste.txt");
    escrever_sol(sol_teste, arq);
    printf("Solucao de teste salva em: %s\n", arq);

    /*
    // ========================================================================
    // ESTRUTURA PARA A PROVA II (A ser descomentada ao implementar a Prova 2)
    // ========================================================================
    Solucao solA, solG, solAG;
    clock_t h;
    double tempoA, tempoG, tempoAG;

    // 1. Heuristica Construtiva Aleatoria
    h = clock();
    heu_con_ale(solA);
    calcular_fo(solA);
    tempoA = ((double)(clock() - h)) / CLOCKS_PER_SEC;

    // 2. Heuristica Construtiva Gulosa
    h = clock();
    heu_con_gul(solG);
    calcular_fo(solG);
    tempoG = ((double)(clock() - h)) / CLOCKS_PER_SEC;

    // 3. Heuristica Construtiva Aleatoria Gulosa
    h = clock();
    heu_con_ale_gul(solAG, 20.0); // 20% de aleatoriedade
    calcular_fo(solAG);
    tempoAG = ((double)(clock() - h)) / CLOCKS_PER_SEC;

    printf("\nResultados Construtivos:\n");
    printf("FO Aleatoria: %d \tTempo: %.5f s\n", solA.fo, tempoA);
    printf("FO Gulosa: %d \tTempo: %.5f s\n", solG.fo, tempoG);
    printf("FO Aleatoria Gulosa: %d \tTempo: %.5f s\n", solAG.fo, tempoAG);

    // 4. Teste de Geracao de Vizinha
    Solucao sol_viz = solG;
    gerar_vizinha(sol_viz);
    calcular_fo(sol_viz);
    printf("FO Vizinha da Gulosa: %d\n", sol_viz.fo);
    */

    return 0;
}

// ============================================================================
// PROVA I - QUESTAO 1: LEITURA DOS DADOS DA INSTANCIA
// ============================================================================
void ler_dados(char* arq)
{
    FILE* f = fopen(arq, "r");
    if (f == NULL)
    {
        printf("Erro: nao foi possivel abrir o arquivo %s para leitura.\n", arq);
        exit(1);
    }

    // 1. Leitura de |N| (navios) e |K| (bercos)
    fscanf(f, "%d %d", &num_nav, &num_ber);

    // 2. Leitura dos tempos de atendimento t_i^k de cada navio i em cada berco k
    //    Linhas: berco k (0 a num_ber-1); Colunas: navio i (0 a num_nav-1)
    for (int k = 0; k < num_ber; k++)
    {
        for (int i = 0; i < num_nav; i++)
        {
            fscanf(f, "%d", &mat_tempo_atend[k][i]);
        }
    }

    // 3. Leitura dos horarios de abertura (o^k) e fechamento (c^k) de cada berco k
    for (int k = 0; k < num_ber; k++)
    {
        fscanf(f, "%d %d", &vet_abertura_ber[k], &vet_fechamento_ber[k]);
    }

    // 4. Leitura dos tempos de chegada a_i de cada navio i
    for (int i = 0; i < num_nav; i++)
    {
        fscanf(f, "%d", &vet_tempo_chegada[i]);
    }

    // 5. Leitura dos tempos limite para saida b_i de cada navio i
    for (int i = 0; i < num_nav; i++)
    {
        fscanf(f, "%d", &vet_tempo_limite[i]);
    }

    fclose(f);
}

// ============================================================================
// METODO DE TESTE DOS DADOS LIDOS (Modelo PMM)
// Escreve os dados lidos para conferir integridade na tela ou em arquivo
// ============================================================================
void testar_dados(char* arq)
{
    FILE* f;
    if (strcmp(arq, "") == 0)
        f = stdout;
    else
        f = fopen(arq, "w");

    if (!f) return;

    fprintf(f, "%d %d\n", num_nav, num_ber);
    for (int k = 0; k < num_ber; k++)
    {
        for (int i = 0; i < num_nav; i++)
            fprintf(f, "%d ", mat_tempo_atend[k][i]);
        fprintf(f, "\n");
    }
    for (int k = 0; k < num_ber; k++)
        fprintf(f, "%d %d\n", vet_abertura_ber[k], vet_fechamento_ber[k]);
    for (int i = 0; i < num_nav; i++)
        fprintf(f, "%d ", vet_tempo_chegada[i]);
    fprintf(f, "\n");
    for (int i = 0; i < num_nav; i++)
        fprintf(f, "%d ", vet_tempo_limite[i]);
    fprintf(f, "\n");

    if (strcmp(arq, "") != 0)
        fclose(f);
}

// ============================================================================
// PROVA I - QUESTAO 4: CALCULO DA FUNCAO OBJETIVO (FO)
// Minimizar: f(s) = sum_{i in N} (T_i^k - a_i + t_i^k) + penalidades
//
// O calculo do horario de atracacao T_i^k e deterministico dado a sequencia:
// - O primeiro navio do berco atraca em max(o^k, a_i).
// - Os navios seguintes atracam em max(fim_do_anterior, a_i).
//
// Restricoes avaliadas e penalizadas caso violadas:
// 1. Incompatibilidade: t_i^k == 0 (PESO_INCOMP)
// 2. Limite de saida do navio: T_i^k + t_i^k > b_i (PESO_PRAZO * atraso)
// 3. Fechamento do berco: T_i^k + t_i^k > c^k (PESO_FECH * atraso)
// 4. Navios nao atendidos (PESO_NAO_ATEND)
// 5. Navios duplicados (PESO_DUP)
// ============================================================================
void calcular_fo(Solucao& s)
{
    s.fo = 0;
    int vet_atend[MAX_NAV];
    memset(vet_atend, 0, sizeof(vet_atend));

    // Percorre cada berco k
    for (int k = 0; k < num_ber; k++)
    {
        // O berco so pode iniciar atendimentos a partir de seu horario de abertura o^k
        int tempo_livre = vet_abertura_ber[k];

        // Processa os navios alocados ao berco k na ordem da fila
        for (int p = 0; p < s.vet_qtd[k]; p++)
        {
            int nav = s.mat_sol[k][p];

            // Seguranca contra indices invalidos
            if (nav < 0 || nav >= num_nav)
                continue;

            vet_atend[nav]++;

            // Horario de atracacao T_i^k: o maior entre a chegada do navio e o berco estar livre
            int tempo_atracacao = MAX(tempo_livre, vet_tempo_chegada[nav]);
            s.vet_tempo_atracacao[nav] = tempo_atracacao;

            int tempo_atend = mat_tempo_atend[k][nav];
            int tempo_saida = tempo_atracacao + tempo_atend;

            // FO base: Tempo de servico = (tempo_atracacao - chegada + tempo_atendimento)
            s.fo += (tempo_atracacao - vet_tempo_chegada[nav] + tempo_atend);

            // Penalidade 1: Berco incompativel com o navio
            // if (tempo_atend == 0)
            // {
            //     s.fo += PESO_INCOMP;
            // }

            // Penalidade 2: Ultrapassou horario limite de partida do navio (b_i)
            if (tempo_saida > vet_tempo_limite[nav])
            {
                s.fo += PESO_PRAZO * (tempo_saida - vet_tempo_limite[nav]);
            }

            // Penalidade 3: Ultrapassou horario de fechamento do berco (c^k)
            if (tempo_saida > vet_fechamento_ber[k])
            {
                s.fo += PESO_FECH * (tempo_saida - vet_fechamento_ber[k]);
            }

            // O berco fica ocupado ate a saida deste navio
            tempo_livre = tempo_saida;
        }
    }

    // Penalidade 4 e 5: Verificar navios nao atendidos ou duplicados
    for (int i = 0; i < num_nav; i++)
    {
        if (vet_atend[i] == 0)
        {
            s.fo += PESO_NAO_ATEND;
        }
        else if (vet_atend[i] > 1)
        {
            s.fo += PESO_DUP * (vet_atend[i] - 1);
        }
    }
}

// ============================================================================
// PROVA I - QUESTAO 3: ESCRITA DA SOLUCAO (TELA E ARQUIVO)
// Se arq == "", escreve na tela (stdout). Caso contrario, salva no arquivo.
// ============================================================================
void escrever_sol(Solucao& s, char* arq)
{
    FILE* f;
    if (strcmp(arq, "") == 0)
        f = stdout;
    else
        f = fopen(arq, "w");

    if (!f)
    {
        printf("Erro ao abrir arquivo para escrita: %s\n", arq);
        return;
    }

    fprintf(f, "================================================================================\n");
    fprintf(f, "RELATORIO DA SOLUCAO - PROBLEMA DE ALOCACAO DE BERCOS (PAB)\n");
    fprintf(f, "================================================================================\n");
    fprintf(f, "Funcao Objetivo (FO Total com penalidades): %d\n\n", s.fo);

    int total_navios_alocados = 0;

    for (int k = 0; k < num_ber; k++)
    {
        fprintf(f, "Berco %2d [Abertura: %3d | Fechamento: %3d] - Qtd Navios: %d\n",
                k + 1, vet_abertura_ber[k], vet_fechamento_ber[k], s.vet_qtd[k]);

        if (s.vet_qtd[k] == 0)
        {
            fprintf(f, "   (Nenhum navio atendido neste berco)\n\n");
            continue;
        }

        fprintf(f, "   Fila | Navio | Chegada(a) | Atrac(T) | Atend(t) | Saida(T+t) | Limite(b) | Espera | Servico\n");
        fprintf(f, "   ------------------------------------------------------------------------------------------\n");

        for (int p = 0; p < s.vet_qtd[k]; p++)
        {
            int nav = s.mat_sol[k][p];
            int a = vet_tempo_chegada[nav];
            int t = mat_tempo_atend[k][nav];
            int T = s.vet_tempo_atracacao[nav];
            int saida = T + t;
            int b = vet_tempo_limite[nav];
            int espera = T - a;
            int servico = saida - a;

            fprintf(f, "   %4d |  %4d |     %6d |   %6d |   %6d |     %6d |   %6d | %6d | %7d\n",
                    p + 1, nav + 1, a, T, t, saida, b, espera, servico);
            total_navios_alocados++;
        }
        fprintf(f, "\n");
    }

    fprintf(f, "Total de navios alocados nos bercos: %d de %d\n", total_navios_alocados, num_nav);
    fprintf(f, "================================================================================\n\n");

    if (strcmp(arq, "") != 0)
        fclose(f);
}

// ============================================================================
// FUNCOES AUXILIARES PARA MANIPULACAO DE SOLUCAO (Modelo p1_alterada.cpp)
// Uteis para implementar gerar_vizinha e as heuristicas construtivas
// ============================================================================

// Insere o navio na posicao especificada do berco, deslocando os posteriores
void inserir_navio(Solucao& s, const int& berco, const int& navio, const int& pos)
{
    for (int i = s.vet_qtd[berco]; i > pos; i--)
        s.mat_sol[berco][i] = s.mat_sol[berco][i - 1];
    s.mat_sol[berco][pos] = navio;
    s.vet_qtd[berco]++;
}

// Remove o navio da posicao especificada do berco, deslocando os posteriores
void remover_navio(Solucao& s, const int& berco, const int& pos)
{
    for (int i = pos; i < s.vet_qtd[berco] - 1; i++)
        s.mat_sol[berco][i] = s.mat_sol[berco][i + 1];
    s.vet_qtd[berco]--;
}

// Ordena os indices dos navios em vet_ind_nav_ord por tempo de chegada crescente (Modelo pmm.cpp)
void ordenar_navios()
{
    for (int i = 0; i < num_nav; i++)
        vet_ind_nav_ord[i] = i;

    int flag = 1;
    while (flag)
    {
        flag = 0;
        for (int i = 0; i < num_nav - 1; i++)
        {
            // Quem chega mais cedo (menor tempo de chegada) vem primeiro
            if (vet_tempo_chegada[vet_ind_nav_ord[i]] > vet_tempo_chegada[vet_ind_nav_ord[i + 1]])
            {
                int aux = vet_ind_nav_ord[i];
                vet_ind_nav_ord[i] = vet_ind_nav_ord[i + 1];
                vet_ind_nav_ord[i + 1] = aux;
                flag = 1;
            }
        }
    }
}

// ============================================================================
// DICAS E GUIA ESTRUTURAL PARA A PROVA II (A ser feito pelo aluno)
// ============================================================================
/*
COMO IMPLEMENTAR A PROVA II DA FORMA MAIS SIMPLES POSSIVEL:

-------------------------------------------------------------------------------
1. GERAR VIZINHA (gerar_vizinha(Solucao& s))
-------------------------------------------------------------------------------
Conceito (identico ao p1_alterada.cpp):
Com base em um sorteio rand() % 2, aplique um dos dois movimentos basicos:
- Movimento 0 (Realocacao / Shift):
  a) Sorteie um berco de origem `bOr` que tenha pelo menos 1 navio (s.vet_qtd[bOr] > 0).
  b) Sorteie a posicao `posOr` do navio a ser removido (rand() % s.vet_qtd[bOr]).
  c) Guarde o navio: `int nav = s.mat_sol[bOr][posOr];`
  d) Remova-o: `remover_navio(s, bOr, posOr);`
  e) Sorteie um berco de destino `bDs` (rand() % num_ber).
  f) Sorteie a posicao de insercao `posDs` (rand() % (s.vet_qtd[bDs] + 1)).
  g) Insira: `inserir_navio(s, bDs, nav, posDs);`

- Movimento 1 (Troca / Swap):
  a) Sorteie dois bercos `bOr` e `bDs` nao-vazios (podem ser o mesmo ou diferentes).
  b) Sorteie uma posicao em cada berco: `p1 = rand() % s.vet_qtd[bOr]` e `p2 = rand() % s.vet_qtd[bDs]`.
  c) Troque os navios:
     int aux = s.mat_sol[bOr][p1];
     s.mat_sol[bOr][p1] = s.mat_sol[bDs][p2];
     s.mat_sol[bDs][p2] = aux;

Ao final, chame: calcular_fo(s);

-------------------------------------------------------------------------------
2. HEURISTICA CONSTRUTIVA ALEATORIA (heu_con_ale(Solucao& s))
-------------------------------------------------------------------------------
Passo a passo mais simples:
1. Zere a solucao: memset(&s.vet_qtd, 0, sizeof(s.vet_qtd));
2. Crie um vetor temporario com todos os navios: `int aux[MAX_NAV];` preenchendo aux[i] = i;
3. Embaralhe o vetor aux (embaralhamento aleatorio Fisher-Yates ou swaps).
4. Para cada navio i no vetor embaralhado:
   - Sorteie um berco aleatorio: `int k = rand() % num_ber;`
     (Dica opcional: pode sortear repetidamente ate achar um berco com mat_tempo_atend[k][nav] > 0)
   - Insira o navio no final da fila desse berco:
     s.mat_sol[k][s.vet_qtd[k]] = nav;
     s.vet_qtd[k]++;
5. Chame: calcular_fo(s);

-------------------------------------------------------------------------------
3. HEURISTICA CONSTRUTIVA GULOSA (heu_con_gul(Solucao& s))
-------------------------------------------------------------------------------
Conceito (modelo pmm.cpp):
1. Ordene os navios por um criterio inteligente (criterio guloso).
   Exemplo ideal no PAB: ordenar por tempo de chegada `vet_tempo_chegada[i]` em ordem crescente.
   (Quem chega antes tem prioridade para ser atendido antes).
2. Zere a solucao: memset(&s.vet_qtd, 0, sizeof(s.vet_qtd));
3. Para cada navio `i` (seguindo a ordem de chegada):
   - Avalie todos os bercos `k` compativeis (mat_tempo_atend[k][i] > 0).
   - Escolha o berco `k_melhor` que oferece o menor tempo de saida ou menor aumento da FO.
     (Ou simplesmente o berco compativel com o menor tempo livre disponivel).
   - Insira o navio `i` em `k_melhor`:
     s.mat_sol[k_melhor][s.vet_qtd[k_melhor]] = i;
     s.vet_qtd[k_melhor]++;
4. Chame: calcular_fo(s);

-------------------------------------------------------------------------------
4. HEURISTICA CONSTRUTIVA ALEATORIA GULOSA (heu_con_ale_gul(Solucao& s, const double per_ale))
-------------------------------------------------------------------------------
Conceito (identico ao pmm.cpp):
1. Copie o vetor ordenado de navios para um vetor auxiliar.
2. Embaralhe os primeiros `qtde = MAX(1, (per_ale / 100) * num_nav)` elementos.
3. Execute o mesmo procedimento de alocacao gulosa utilizado na `heu_con_gul`.
4. Chame: calcular_fo(s);
-------------------------------------------------------------------------------
*/

void heu_con_ale(Solucao& s){
    memset(&s.vet_qtd, 0, sizeof(s.vet_qtd));
    for(int i=0; i<num_nav; i++){
        int berco;
        do{
            berco = rand() % num_ber;
        }while(mat_tempo_atend[berco][i]==0);
        s.mat_sol[berco][s.vet_qtd[berco]] = i;
        s.vet_qtd[berco]++;
    }
}

void heu_con_gul(Solucao& s){
    memset(&s.vet_qtd, 0, sizeof(s.vet_qtd));
    for(int i=0; i<num_nav; i++){
        int nav = vet_ind_nav_ord[i];
        int mlb = -1;
        int mlt = 999;
        for(int j=0; j<num_ber; j++){
            if(mat_tempo_atend[j][nav]==0) continue;
            if(mat_tempo_atend[j][nav]<mlt){
                mlb = j;
                mlt = mat_tempo_atend[j][nav];
            }
        }
        s.mat_sol[mlb][s.vet_qtd[mlb]] = nav;
        s.vet_qtd[mlb]++;
    }
}