#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

struct trieNode{
    int ehFDP; // flag pra verificar se � fim de uma palavra ou n�o
    Trie character[TAM_ALFABETO]; // vetor de n�s de Trie
};


Trie* criaTrie(){ // aloco um ponteiro para um novo n�, inicializo ele como n�o sendo fim de uma palavra e preencho cada um dos campos que representam os caracteres com NULL
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
        int indice = str[i] - 'a'; // pego a posi��o no alfabeto do caractere atual a ser inserido
        if(noAtual->character[indice] == NULL){ // se esse caractere j� estiver inserido, ou seja, o prefixo da palavra at� esse ponto existir eu n�o entro aqui
            // se n�o foi inserido, eu entro para criar esse novo caractere
            noAtual->character[indice] = (Trie) malloc(sizeof(struct trieNode));
            noAtual->character[indice]->ehFDP = 0;
            for(int j = 0; j < TAM_ALFABETO; j++)
                noAtual->character[indice]->character[j] = NULL;
        }

        // aqui eu avan�o o noAtual pra continuar alocando novas �rvores para cada caractere inserido
        noAtual = noAtual->character[indice];
    }

    // eu marco o n� a frente como fim da palavra
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
        // essa string n�o existe na Trie
        if(noAtual == NULL)
            return 0;
    }

    return noAtual->ehFDP;
}

int arvoreComFilhos(Trie* tr){
    // se o ponteiro pra essa �rvore � nulo, n�o tem como o n� ter filhos
    if(tr == NULL)
        return 0;

    for(int i = 0; i < TAM_ALFABETO; i++){
        if((*tr)->character[i] != NULL) // achou qualquer ponteiro apontando pra algo retorna true
            return 1;
    }

    return 0;
}

/*int removeTrie(Trie* tr, char *str){
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
}*/

int removeTrie(Trie* tr, char* str){
    // se a arvore nao existe ou se eu to tentando remover algo que não é uma palavra (com caracteres fora do alfabeto ou com tamanho 0) eu retorno falha
    if(*tr == NULL || strlen(str) == 0 || !validaPalavra(str))
        return 0;

    Trie noAtual = *tr, noPrevio;

    for(int i = 0; i < strlen(str); i++){
        // se eu não achei a palavra na árvore eu retorno falhao
        if(noAtual->character[str[i] - 'a'] == NULL)
            return 0;
        else{
            noPrevio = noAtual;
            noAtual = noAtual->character[str[i] - 'a'];
        }
    }

    // se o no atual não é fim de palavra o que eu tenho é a tentativa da remoção de um prefixo e não um membro da árvore. Falha
    if(!(noAtual->ehFDP))
        return 0;

    // se eu tenho uma árvore com filhos eu só preciso desmarcar a flag de FDP (fim de palavra) porque o resto é prefixo de outro membro da árvore
    if(arvoreComFilhos(&noAtual)){
        noAtual->ehFDP = 0;
        return 1;
    }
    else{
        // libero o fim da palavra e associo o ponteiro que apontava pra ela no ramo imediatamente superior a NULL
        free(noAtual);
        noPrevio->character[str[strlen(str) - 1] - 'a'] = NULL;
        return 1;
    }

    return 0;
}


void imprimeAux(Trie* tr, char *str, int pos){
    if(*tr == NULL)
        return;

    if((*tr)->ehFDP){
        for(int i = 0; i < pos; i++)
            printf("%c", str[i]);

        printf("\n");
    }
    for(int i = 0; i < TAM_ALFABETO; i++){
        if((*tr)->character[i] != NULL){
            str[pos] = i + 'a';
            imprimeAux(&((*tr)->character[i]), str, pos+1);
        }
    }
}

void imprimeTrie(Trie* tr){
    char str[26] = "";
    imprimeAux(tr, str, 0);

}

void autocompletarAux(Trie* tr, char *str, int pos, char* prefixo){
    if(*tr == NULL || tr == NULL)
        return;

    // chegou no fim de uma palavra com esse prefixo, imprime ele e depois os caracteres que vieram depois dele e tão guardados em str
    if((*tr)->ehFDP){
        printf("%s", prefixo);
        for(int i = 0; i < pos; i++)
            printf("%c", str[i]);

        printf("\n");
    }
    // vai chamando recursivamente onde quer que existam caracteres depois daquele prefixo
    // os caracteres achados a cada chamada são guardados em str
    for(int i = 0; i < TAM_ALFABETO; i++){
        if((*tr)->character[i] != NULL){
            str[pos] = i + 'a';
            autocompletarAux(&((*tr)->character[i]), str, pos+1, prefixo);
        }
    }
}

void autocompletarTrie(Trie* tr, char *prefixo){
    int i = 0;
    while(1){
        // se eu avancei na árvore até chegar no fim do prefixo eu chamo a função auxiliar responsável pelo autocomplete
        if(strlen(prefixo) == i){
            char str[26]= "";
            autocompletarAux(tr, str, 0, prefixo);
            return;
        }
        // if que verifica se o prefixo está na árvore, olhando caractere a caractere nas subárvores
        if((*tr)->character[prefixo[i] - 'a'] == NULL){
            return;
        }
        // avança a árvore pra quando começar a autocompletar não ter que refazer o percorrimento desde o início dela, mas sim do último caractere do prefixo
        tr = &((*tr)->character[prefixo[i] - 'a']);
        i++;
    }
}

void liberaTrie(Trie* tr){
    if(tr == NULL || *tr == NULL)
        return;
    
    for(int i = 0; i < 26; i++){
        liberaTrie(&((*tr)->character[i]));
    }

    free(*tr);
    *tr = NULL;
}