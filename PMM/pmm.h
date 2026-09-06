#define MAX_OBJ 500
#define MAX_MOC 50

const int PESO_CAP = 10;
const int PESO_DUP = 100;

typedef struct tSolucao{
    int vet_pesos[MAX_MOC];
    int vet_sol[MAX_OBJ];
    int fo;
}Solucao;

typedef struct tSolucaoBIN{
    int mat_sol[MAX_MOC][MAX_OBJ];
    int fo;
}SolucaoBIN;


int num_obj;
int num_moc;
int vet_val_obj[MAX_OBJ];
int vet_pes_obj[MAX_OBJ];
int vet_cap_moc[MAX_MOC];

int vet_ind_obj_ord[MAX_OBJ];

void heu_con_ale(Solucao& s);
void heu_con_gul(Solucao& s);
void heu_con_ale_gul(Solucao& s, const double per_ale);
void ordenar_objetos();


void calcular_FO(Solucao& s);
void escrever_sol(Solucao& s);
void calcular_FOBIN(SolucaoBIN& s);
void escrever_solBIN(SolucaoBIN& s);
void testar_dados(char* arq);
void ler_dados(char* arq);