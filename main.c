#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Trie.h"

void finaliza(chave *ch, cpf *cp, int k){//Passa as inf pro blocos de notas
    cpf *aux = cp;
    info *dado;
    int num = 0;

    FILE *saida;
    saida = fopen("banco.txt", "w");
    fprintf(saida, "%d\n", k);

    for(int i = 0; i < k; i++){
        while(aux -> flag == 0){//Procura um cpf qualquer, naturalmente, a saida vai ficar em ordem crescente de cpf
            for(int j = 0; j < 10; j++){
                if(aux -> filhos[j] != NULL){
                    num = j;
                    aux = aux -> filhos[num];
                    break;
                }
            }
        }

        dado = aux -> dados;
        fprintf(saida, "%s %s %s ", dado -> nome, dado -> banco, dado -> agencia);
        fprintf(saida, "%s %s %s\n", dado -> conta, dado -> chave, dado -> cpf);
        removech(ch, cp, dado -> chave, 2);
        aux = cp;
    }
    fclose(saida);
}

int main(){
    int n = 0, j = 0;
    char i, nome[100], banco[100], agencia[100], conta[100], num[11], busca[100], aux[100];

    chave *ch = criach();
    cpf *cp = criacpf();

    FILE *entrada;
    entrada = fopen("banco.txt", "r");
    fscanf(entrada, "%d", &n);
    for(;j < n;j++){    //Pega as n informa��o do bloco de notas banco e coloca nas arvores Trie
        fscanf(entrada, "%s %s %s %s %s %s", &nome, &banco, &agencia, &conta, &aux, &num);
        char *key = (char*)malloc((strlen(aux) + 1)*sizeof(char)); //Tive que fazer isso pois o aux não estava cadastrando as chaves, as
        strcpy(key, aux);                                         //chaves apareciam não apareciam se eu simplesmente fizesse o fscanf
        insere(ch, cp, nome, banco, agencia, conta, key, num);
        free(key);
    }
    fclose(entrada);

    while(i != 'F'){
        scanf("%c", &i);

        if(i == 43){
            scanf("%s %s %s %s %s %s", &nome, &banco, &agencia, &conta, &aux, &num);
            char *key = (char*)malloc((strlen(aux) + 1)*sizeof(char));
            strcpy(key, aux);
            insere(ch, cp, nome, banco, agencia, conta, key, num);
            printf("%s cadastrado com sucesso.\n\n", nome);
            free(key);
            n++;
        }

        if(i == 45){
            scanf("%s", &busca);
            if(busca[0] >= 'a'){
                removech(ch, cp, busca, 0);//É deveras importante esse 0, pois com k == 0 faz a função chamar removecp com valor
            }                             //de k = 1, não gerarando um loop, evitando uma função chamar a outra infinitamente
            else{
                removecp(cp, ch, busca, 0);
            }
            n--;
        }

        if(i == 63){
            scanf("%s", &busca);
            if(busca[0] >= 'a'){
                buscach(ch, busca);
            }
            else{
                buscacp(cp, busca);
            }
        }
    }
    finaliza(ch, cp, n);
    free(ch);
    free(cp);
    return 0;
}

