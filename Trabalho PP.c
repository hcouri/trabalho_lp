#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define tam_heap 12
bool heap[tam_heap] = {false};

typedef struct Node{
    int id;
    int end_inicial;
    int blocos_livres;
    struct Node* prox;
}Node;

Node* lista_areas_livres = NULL;
Node* lista_alocacoes = NULL;
int contador_id = 1;



void ImprimeHeap(){
    for(int i = 0; i < tam_heap; i++)
        printf("%d ", heap[i]);
    puts(""); // estética
}

void Inicializa_Lista(){
    lista_areas_livres = (Node*)malloc(sizeof(Node));
    lista_areas_livres->end_inicial = 0;
    lista_areas_livres->blocos_livres = tam_heap; // tam_heap == 12
    lista_areas_livres->prox = NULL;
}

void first(int valor, int* id_alocacao){
    Node* No_atual = lista_areas_livres;
    Node* No_anterior = NULL;

    while(No_atual != NULL){
        if(No_atual->blocos_livres >= valor){
            *id_alocacao = contador_id++;
            for(int i = 0; i < valor; i++)
                heap[No_atual->end_inicial + i] = true;

            int inicio_alocacao = No_atual->end_inicial;

            if(No_atual->blocos_livres == valor){
                if(No_anterior == NULL)
                    lista_areas_livres = No_atual->prox;
                else
                    No_anterior->prox = No_atual->prox;
                free(No_atual);
            }
            else{
                No_atual->blocos_livres -= valor;
                No_atual->end_inicial += valor;
            }

            Node* nova_alocacao = (Node*)malloc(sizeof(Node));
            nova_alocacao->id = *id_alocacao;
            nova_alocacao->end_inicial = inicio_alocacao;
            nova_alocacao->blocos_livres = valor;
            nova_alocacao->prox = lista_alocacoes;
            lista_alocacoes = nova_alocacao;

            break;
        }
        No_anterior = No_atual;
        No_atual = No_atual->prox;
    }
    
    // Se não encontrou nenhum bloco com espaco suficiente
    if(No_atual == NULL){
        printf("Erro: Nao ha bloco livre suficientemente grande.\n");
        return;
    }
}

void best(int valor, int* id_alocacao){
    Node* No_atual = lista_areas_livres;
    Node* No_anterior = NULL;
    Node* No_melhor = NULL;
    Node* No_melhor_anterior = NULL;

    // Encontra o menor bloco que seja suficiente
    while(No_atual != NULL){
        if(No_atual->blocos_livres >= valor){
            if(No_melhor == NULL || No_atual->blocos_livres < No_melhor->blocos_livres){ //im = j... (mesma ideia do selection sort, por exemplo).
                No_melhor = No_atual;
                No_melhor_anterior = No_anterior;
            }
        }
        No_anterior = No_atual;
        No_atual = No_atual->prox;
    }

    // Se não encontrou nenhum bloco com espaco suficiente
    if(No_melhor == NULL){
        printf("Erro: Nao ha bloco livre suficientemente grande.\n");
        return;
    }

    // Realiza a alocação no melhor bloco encontrado
    *id_alocacao = contador_id++;
    for(int i = 0; i < valor; i++)
        heap[No_melhor->end_inicial + i] = true;

    int inicio_alocacao = No_melhor->end_inicial;

    if(No_melhor->blocos_livres == valor){
        // Remove o bloco da lista de áreas livres
        if(No_melhor_anterior == NULL)
            lista_areas_livres = No_melhor->prox;
        else
            No_melhor_anterior->prox = No_melhor->prox;
        free(No_melhor);
    }
    else{
        // Ajusta o bloco restante
        No_melhor->end_inicial += valor;
        No_melhor->blocos_livres -= valor;
    }

    // Adiciona a alocacão à lista de alocacões
    Node* nova_alocacao = (Node*)malloc(sizeof(Node));
    nova_alocacao->id = *id_alocacao;
    nova_alocacao->end_inicial = inicio_alocacao;
    nova_alocacao->blocos_livres = valor;
    nova_alocacao->prox = lista_alocacoes;
    lista_alocacoes = nova_alocacao;
}

void worst(int valor, int* id_alocacao){
    Node* No_atual = lista_areas_livres;
    Node* No_anterior = NULL;
    Node* No_pior = NULL;
    Node* No_pior_anterior = NULL;

    // Encontra o maior bloco que seja suficiente
    while(No_atual != NULL){
        if(No_atual->blocos_livres >= valor){
            if(No_pior == NULL || No_atual->blocos_livres > No_pior->blocos_livres){ // troca o sinal
                No_pior = No_atual;
                No_pior_anterior = No_anterior;
            }
        }
        No_anterior = No_atual;
        No_atual = No_atual->prox;
    }

    // Se não encontrou nenhum bloco com espaco suficiente
    if(No_pior == NULL){
        printf("Erro: Nao ha bloco livre suficientemente grande.\n");
        return;
    }

    // Realiza a alocação no pior bloco encontrado
    *id_alocacao = contador_id++;
    for(int i = 0; i < valor; i++)
        heap[No_pior->end_inicial + i] = true;

    int inicio_alocacao = No_pior->end_inicial;

    if(No_pior->blocos_livres == valor){
        // Remove o bloco da lista de áreas livres
        if(No_pior_anterior == NULL)
            lista_areas_livres = No_pior->prox;
        else
            No_pior_anterior->prox = No_pior->prox;
        free(No_pior);
    }
    else{
        // Ajusta o bloco restante
        No_pior->end_inicial += valor;
        No_pior->blocos_livres -= valor;
    }

    // Adiciona a alocacão à lista de alocacões
    Node* nova_alocacao = (Node*)malloc(sizeof(Node));
    nova_alocacao->id = *id_alocacao;
    nova_alocacao->end_inicial = inicio_alocacao;
    nova_alocacao->blocos_livres = valor;
    nova_alocacao->prox = lista_alocacoes;
    lista_alocacoes = nova_alocacao;
}

void liberar(int id_alocacao){
    Node* atual = lista_alocacoes;
    Node* anterior = NULL;
    bool encontrou = false;

    while(atual != NULL){
        if(atual->id == id_alocacao){
            encontrou = true;
            for(int i = 0; i < atual->blocos_livres; i++)
                heap[atual->end_inicial + i] = false;  // Libera os blocos alocados

            if(anterior == NULL)
                lista_alocacoes = atual->prox;
            else
                anterior->prox = atual->prox;

            // Cria um novo nó para a lista de áreas livres
            Node* novo_no = (Node*)malloc(sizeof(Node));
            novo_no->end_inicial = atual->end_inicial;
            novo_no->blocos_livres = atual->blocos_livres;
            novo_no->prox = NULL;

            // Inserindo na lista de áreas livres, mantendo a lista ordenada por end_inicial
            if(lista_areas_livres == NULL || lista_areas_livres->end_inicial > novo_no->end_inicial){
                novo_no->prox = lista_areas_livres;
                lista_areas_livres = novo_no;
            }
            else{
                Node* da_frente = lista_areas_livres;
                while(da_frente->prox != NULL && da_frente->prox->end_inicial < novo_no->end_inicial)
                    da_frente = da_frente->prox;
                novo_no->prox = da_frente->prox;
                da_frente->prox = novo_no;
            }

            // Unindo blocos livres adjacentes, se houver
            Node* da_frente = lista_areas_livres;
            Node* de_tras = NULL;
            while(da_frente != NULL){
                if(de_tras != NULL && de_tras->end_inicial + de_tras->blocos_livres == da_frente->end_inicial){
                    de_tras->blocos_livres += da_frente->blocos_livres;
                    de_tras->prox = da_frente->prox;
                    free(da_frente);
                    da_frente = de_tras->prox;
                }
                else{
                    de_tras = da_frente;
                    da_frente = da_frente->prox;
                }
            }

            free(atual);  // Libera a alocacao
            ImprimeHeap();
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }

    if(!encontrou)
        printf("Erro: Nao foi possivel liberar o bloco com ID %d.\n", id_alocacao);
}

void LiberaLista(Node* lista){
    Node* atual = lista;
    while(atual != NULL){
        Node* temp = atual;
        atual = atual->prox;
        free(temp);
    }
}

void LiberaRecursos(){
    LiberaLista(lista_areas_livres);
    LiberaLista(lista_alocacoes);
}

int main(){
    ImprimeHeap();
    Inicializa_Lista();

    while(1){
        char estrategia[5];
        printf("\nMODO(first, best, worst , liberar, sair): ");
        scanf("%s", estrategia);

        if(strcmp(estrategia, "first") == 0){
            char nome[10];
            int valor = 0;
            printf("Nome e valor: ");
            scanf("%s %d", nome, &valor);

            int id_alocacao = 0;
            first(valor, &id_alocacao);
            printf("Alocacao ID: %d\n", id_alocacao);
            ImprimeHeap();
        }

        else if(strcmp(estrategia, "best") == 0){
            char nome[10];
            int valor = 0;
            printf("Nome e valor: ");
            scanf("%s %d", nome, &valor);

            int id_alocacao = 0;
            best(valor, &id_alocacao);
            printf("Alocacao ID: %d\n", id_alocacao);
            ImprimeHeap();
        }

        else if(strcmp(estrategia, "worst") == 0){
            char nome[10];
            int valor = 0;
            printf("Nome e valor: ");
            scanf("%s %d", nome, &valor);

            int id_alocacao = 0;
            worst(valor, &id_alocacao);
            printf("Alocacao ID: %d\n", id_alocacao);
            ImprimeHeap();
        }

        else if(strcmp(estrategia, "liberar") == 0){
            int id_alocacao = 0;
            printf("ID da alocacao a ser liberada: ");
            scanf("%d", &id_alocacao);

            liberar(id_alocacao);
            ImprimeHeap();
        }

        else if(strcmp(estrategia, "sair") == 0){
            printf("Encerrando\n");
            break; //sai do while para liberar recursos
        }
        
        else
            printf("Opcao invalida\n");
    }

    LiberaRecursos(); //consegue liberar as duas listas pois sao globais, modificacoes em qualquer lugar terao impacto global e imediato
}
