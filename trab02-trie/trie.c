#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

struct trieNode{
    int ehFDP; // flag pra verificar se é fim de uma palavra ou não
    struct trieNode* character[TAM_ALFABETO]; // vetor de nós de Trie
};


Trie* criaTrie(){ // aloco um ponteiro para um novo nó, inicializo ele como não sendo fim de uma palavra e preencho cada um dos campos que representam os caracteres com NULL
    Trie* no = (Trie*) malloc(sizeof(Trie));
    *no = (Trie) malloc(sizeof(struct trieNode));
    if(no != NULL){
     (*no)->ehFDP = 0;
        for(int i = 0; i < TAM_ALFABETO; i++)
         (*no)->character[i] = NULL;
    }

    return no;
}

int insereTrie(Trie* tr, char *str){
    if(tr == NULL)
        return 0;

    Trie noAtual = *tr;
    for(int i = 0; i < strlen(str); i++){
        int indice = str[i] - 'a'; // pego a posição no alfabeto do caractere atual a ser inserido
        if(noAtual->character[indice] == NULL){ // se esse caractere já estiver inserido, ou seja, o prefixo da palavra até esse ponto existir eu não entro aqui
            // se não foi inserido, eu entro para criar esse novo caractere
            noAtual->character[indice] = (Trie) malloc(sizeof(struct trieNode));
            noAtual->character[indice]->ehFDP = 0;
            for(int j = 0; j < TAM_ALFABETO; j++)
                noAtual->character[indice]->character[j] = NULL;   
        }

        // aqui eu avanço o noAtual pra continuar alocando novas árvores para cada caractere inserido
        noAtual = noAtual->character[indice];
    }

    // eu marco o nó a frente como fim da palavra
    noAtual->ehFDP = 1;
    return 1;
}

int validaPalavra(char *str){
    for(int i = 0; i < strlen(str); i++){
        if(str[i] < 'a' || str[i] >= 'a' + TAM_ALFABETO)
            return 0;
    }

    return 1;
}


int buscaTrie(Trie* tr, char *str){
    if(tr == NULL || !validaPalavra(str))
        return 0;

    Trie noAtual = *tr;
    for(int i = 0; i < strlen(str); i++){
        int indice = str[i] - 'a';
        noAtual = noAtual->character[indice];
        
        // se eu cheguei no fim de um caminho da Trie antes de chegar no fim da string passada
        // essa string não existe na Trie
        if(noAtual == NULL)
            return 0;
    }

    return noAtual->ehFDP;
}

int arvoreComFilhos(Trie* tr){
    // se o ponteiro pra essa árvore é nulo, não tem como o nó ter filhos
    if(tr == NULL)
        return 0;

    for(int i = 0; i < TAM_ALFABETO; i++){
        if((*tr)->character[i] != NULL) // achou qualquer ponteiro apontando pra algo retorna true
            return 1;
    }

    return 0;
}

int removeTrie(Trie* tr, char *str){
    if(tr == NULL || !validaPalavra(str))
        return 0;


    // enquanto nao tiver chegado ao fim da string faz a consulta
    if(strlen(str) > 0){
        int indice = str[0] - 'a';
        if(tr != NULL && (*tr)->character[indice] != NULL && removeTrie(&(*tr)->character[indice], str + 1) && (*tr)->ehFDP == 0){
            if(!arvoreComFilhos(tr)){
                free(*tr);
                tr = NULL;
                return 1;
            }
            else return 0;
        }
    }
    else if((*tr)->ehFDP){
        if(!arvoreComFilhos(tr)){
            free(*tr);
            tr = NULL;
            return 1;
        }
        else{
            (*tr)->ehFDP = 0;
            return 0;
        }
    }

    return 0;
}

void imprimeAux(Trie* tr, char *str, int pos){
    if(tr == NULL)
        return;
    
    if((*tr)->ehFDP){
        for(int i = 0; i < pos; i++)
            printf("%c", str[i]);

        printf("\n");
    }
    for(int i = 0; i < TAM_ALFABETO; i++){
        if((*tr)->character[i] != NULL)
            str[i] = i + 'a';
            imprimeAux(&((*tr)->character[i]), str, pos+1);
    }
}

void imprimeTrie(Trie* tr){
    char str[26];
    imprimeAux(tr, str, 0);

}