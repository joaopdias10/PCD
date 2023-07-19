#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct info{
    char *nome;
    char *banco;
    char *agencia;
    char *conta;
    char *chave;
    char *cpf;
}info;

typedef struct chave{
    int flag;
    struct info *dados;
    struct chave *pai;
    struct chave *filhos[26];
}chave;

typedef struct cpf{
    int flag;
    struct info *dados;
    struct cpf *pai;
    struct cpf *filhos[10];
}cpf;


void insere(chave *ch, cpf *cp, char *name, char *bank, char *agency, char *account, char *key, char *CPF);


//Parte da chave
chave *criach();
void buscach(chave *ch, char *key);
void removech(chave *ch, cpf *cp, char *key, int k);

//Parte do cpf
cpf *criacpf();
void buscacp(cpf *cp, char *CPF);
void removecp(cpf *cp, chave *ch, char *CPF, int k);
