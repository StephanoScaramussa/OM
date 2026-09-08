#include "prcp.h"

int main(){
    char arq[50];
    strcpy(arq, "..\\inst1.txt");
    le_dados(arq);
}

void le_dados(char* arq){
    FILE* f = fopen(arq, "r");

}