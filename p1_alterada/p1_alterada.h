#define MAX_VEI 100
#define MAX_CLI 501

const int PESO_CAP = 100;

typedef struct tSolucao{
    int mat_sol[MAX_VEI][MAX_CLI];
    int vet_qtd[MAX_CLI];
    double fo;
}Solucao;

int num_vei;
int num_cli;
int vet_demandas[MAX_CLI];
int vet_cap_vei[MAX_VEI];
double mat_custo[MAX_CLI][MAX_CLI];


void heu_con_gul(Solucao& s);
void inserir_cli(Solucao& s, const int& vei, const int& cli);
void remover_cli(Solucao& s, const int& vei, const int& pos);
void gerar_vizinha(Solucao& s);
void calcular_fo(Solucao& s);
void escrever(Solucao&s, char* arq);
void ler_dados(char* arq);
