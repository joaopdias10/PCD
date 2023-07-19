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

chave *criach(){ //Cria um "nodo" com 26 ponteiros para filhos
    chave *novo = (chave*)malloc(sizeof(chave));
    novo -> flag = 0;
    novo -> pai = NULL;
    novo -> dados = NULL;
    for(int i = 0;i < 26; i++){
        novo -> filhos[i] = NULL;
    }
    return novo;
}

cpf *criacpf(){ //Cria um "nodo" com 10 ponteiros para filhos
    cpf *novo = (cpf*)malloc(sizeof(cpf));
    novo -> flag = 0;
    novo -> pai = NULL;
    novo -> dados = NULL;
    for(int i = 0;i < 10; i++){
        novo -> filhos[i] = NULL;
    }
    return novo;
}

void insere(chave *ch, cpf *cp, char *name, char *bank, char *agency, char *account, char *key, char *CPF){ // Insere a string CPF em uma arvore Trie
    int i = 0, num = 0, letra = 0;
    cpf *aux = cp;
    chave *auxiliar = ch;
    info *dado = (info*)malloc(sizeof(info));

    for(; CPF[i] != '\0'; i++){
        num = CPF[i] - '0';
        if(aux -> filhos[num] == NULL){
            aux -> filhos[num] = criacpf();
            (aux -> filhos[num]) -> pai = aux; //Faz um ponteiro que aponta para o pai
        }
        aux = aux -> filhos[num];
    }

    for(i = 0; key[i] != '\0'; i++){
        letra = key[i] - 'a';
        if(auxiliar -> filhos[letra] == NULL){
            auxiliar -> filhos[letra] = criach();
            (auxiliar -> filhos[letra]) -> pai = auxiliar; //Faz um ponteiro que aponta para o pai, literalmente(Pai do filho é o pai)
        }
        auxiliar = auxiliar -> filhos[letra];
    }

    aux -> flag = 1;
    auxiliar -> flag = 1;
    dado -> nome = strdup(name);// Aloca e copia as strings para os dado->(char *)
    dado -> banco = strdup(bank);
    dado -> agencia = strdup(agency);
    dado -> conta = strdup(account);
    dado -> chave = strdup(key);
    dado -> cpf = strdup(CPF);
    aux -> dados = dado;//Talvez pudesse ter colocado direto no aux -> dados, mas agora ja foi e ta dando certo
    auxiliar -> dados = dado;
}

void buscach(chave *ch, char *key){ //Busca a chave inserida, bem parecido com a fun��o inserech digasse de passagem
    int i = 0, letra = 0;
    chave *aux = ch;
    info *dado;

    for(; key[i] != '\0'; i++){
        letra = key[i] - 'a';
        if(aux -> filhos[letra] == NULL){
            printf("Chave %s nao cadastrada.\n\n", key);
            return;
        }
        aux = aux -> filhos[letra];
    }
    if(aux -> flag == 0){ //Necessita verificar no caso, por exemplo, de procurar a chave "and" e tem cadastrado na arvore Trie "andarilho"
        printf("Chave %s nao cadastrada.\n\n", key);
        return;
    }
    dado = aux -> dados;//Fiz isso para diminuir a linha abaixo, ficaria longo "aux -> dados -> char *"
    printf("%s %s %s %s\n\n", dado -> nome, dado -> banco, dado -> agencia, dado -> conta);
}

void buscacp(cpf *cp, char *CPF){ //Busca o cpf inserido, bem parecido com a função inserecp
    int i = 0, num = 0;
    cpf *aux = cp;
    info *dado;

    for(; CPF[i] != '\0'; i++){
        num = CPF[i] - '0';
        if(aux -> filhos[num] == NULL){
            printf("CPF %s nao cadastrado.\n\n", CPF);
            return;
        }
        aux = aux -> filhos[num];
    }

    if(aux -> flag == 0){ //Não precisava, pois se o i chegar em 10 ja seria o suficiente pra provar que existe aquele cpf cadastrado
        printf("CPF %s nao cadastrado.\n\n", CPF); // pq não existe cpf igual e de tamanho diferente mas quis deixar padrao "FIFA"
        return;
    }

    dado = aux -> dados;//Fiz isso para diminuir a linha abaixo, ficaria longo "aux -> dados -> char *"
    printf("%s %s %s %s\n\n", dado -> nome, dado -> banco, dado -> agencia, dado -> conta);
}

void removech(chave *ch, cpf *cp, char *key, int k){ //Remove da arvore Trie, eu até podia fazer a busca retornar "nodo"
    int i = 0, letra = 0;                           // em questao mas preferi fazer uma busca dentro da remove
    chave *aux = ch;
    chave *dad;

    for(; key[i] != '\0'; i++){
        letra = key[i] - 'a';
        if(aux -> filhos[letra] == NULL){
            printf("Chave %s nao cadastrada.\n\n", key);
            return;
        }
        aux = aux -> filhos[letra];
    }
    if(aux -> flag == 0){
        printf("Chave %s nao cadastrada.\n\n", key);
        return;
    } //Até aqui funciona igual à busca


    if(k == 0){//Usado quanda o termo que deseja excluir da arvore é passado como chave e não cpf
        removecp(cp, ch, aux -> dados -> cpf, 1); //Chama a função removecp para remover da lista trie do cpf, com k = 1 para não chamar a função removech e ficar num loop
        printf("%s removido com sucesso.\n\n", aux -> dados -> nome); //Tive que printar aqui para excluir o aux -> dados antes de printar o nome
    }

    if(k == 2){//Usado na função finaliza
        removecp(cp, ch, aux -> dados -> cpf, 1);
    }

    aux -> flag = 0;
    aux -> dados = NULL;
    while(aux -> pai != NULL && aux -> flag == 0){//Apaga o "nodo" e seu caminho
        dad = aux -> pai;
        free(aux);
        aux = dad;

        if(aux != NULL){//Apaga o "caminho"
            i--;
            letra = key[i] - 'a';
            aux -> filhos[letra] = NULL;
        }


        for(letra = 0; letra < 26; letra++){//Verifica se aux tem mais de 1 filho
            if(aux -> filhos[letra] != NULL){
                return;
            }
        }
    }
}

void removecp(cpf *cp, chave *ch, char *CPF, int k){ //Remove da arvore Trie
    int i = 0, num = 0;
    cpf *aux = cp;
    cpf *dad;

    for(; CPF[i] != '\0'; i++){
        num = CPF[i] - '0';
        if(aux -> filhos[num] == NULL){
            printf("CPF %s nao cadastrado.\n\n", CPF);
            return;
        }
        aux = aux -> filhos[num];
    }

    if(aux -> flag == 0){
        printf("CPF %s nao cadastrado.\n\n", CPF);
        return;
    } //Até aqui funciona igual à busca


    if(k == 0){//Usado quanda o termo que deseja excluir da arvore é passado como cpf e não chave
        removech(ch, cp, aux -> dados -> chave, 1);//Chama a função removech para remover da lista trie da chaves, com k = 1
        printf("%s removido com sucesso.\n\n", aux -> dados -> nome); //Tive que printar aqui para excluir o aux -> dados antes de printar o nome
    }

    aux -> flag = 0;
    aux -> dados = NULL;
    while(aux -> pai != NULL && aux -> flag == 0){//Apaga o "nodo" e seu caminho
        dad = aux -> pai;
        free(aux);
        aux = dad;

        if(aux != NULL){//Apaga o "caminho"
            i--;
            num = CPF[i] - '0';
            aux -> filhos[num] = NULL;
        }

        for(num = 0; num < 10; num++){//Verifica se aux tem mais de 1 filho
            if(aux -> filhos[num] != NULL){
                return;
            }
        }
    }
}

