#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct gr {
    int vertice;
    int dado;
    struct gr* proximo;
    struct ga* arestas;
} GR;

typedef struct ga {
    int aresta;
    int verticed;
    int dado_verticed; //  campo para armazenar o dado do v�rtice de destino
    struct ga* proximo;
} GA;

// Fun��o para criar um novo v�rtice com base no n�mero e adicionar � lista de v�rtices
GR* criarVertice(int numero, int dado) {
    // Criar um novo v�rtice na lista ligada GR
    GR* novoVertice = (GR*)malloc(sizeof(GR));
    if (novoVertice == NULL) {
        printf("Erro: N�o foi poss�vel alocar mem�ria para o novo v�rtice.\n");
        exit(1);
    }

    // Atribuir valores ao v�rtice
    novoVertice->vertice = numero; // Criar o v�rtice com o n�mero recebido
    novoVertice->dado = dado; // Atribuir o n�mero como dado (opcional)
    novoVertice->proximo = NULL;
    novoVertice->arestas = NULL;

    return novoVertice;
}

// Fun��o para ler a matriz do ficheiro e criar v�rtices com base nos n�meros
void lerFicheiro(GR** grafos, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        exit(1);
    }

    char line[1000];
    int row = 1; // Iniciar contagem de linha em 1
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ";");
        int col = 1; // Iniciar contagem de coluna em 1
        while (token != NULL) {
            int numero = atoi(token);
            // Criar um novo v�rtice com o n�mero e adicionar � lista de v�rtices
            GR* novoVertice = criarVertice((row * 10) + col, numero);
            // Adicionar v�rtice � lista ligada GR
            if (*grafos == NULL) {
                *grafos = novoVertice;
            }
            else {
                // Encontrar o �ltimo n� na lista
                GR* atual = *grafos;
                while (atual->proximo != NULL) {
                    atual = atual->proximo;
                }
                // Inserir o novo v�rtice no final da lista
                atual->proximo = novoVertice;
            }

            col++;
            token = strtok(NULL, ";");
        }
        row++;
    }
    fclose(file);
}


// Fun��o para encontrar um v�rtice na lista de v�rtices com base no �ndice
GR* encontrarVertice(GR* grafos, int indice) {
    GR* verticeAtual = grafos;

    // Percorre a lista de v�rtices at� encontrar o v�rtice com o �ndice desejado
    while (verticeAtual != NULL) {
        if (verticeAtual->vertice == indice) {
            // Retorna o ponteiro para o v�rtice encontrado
            return verticeAtual;
        }
        verticeAtual = verticeAtual->proximo;
    }

    // Se o v�rtice n�o for encontrado, retorna NULL
    return NULL;
}

// Fun��o para inserir uma aresta no in�cio da lista de adjac�ncia de um v�rtice
GA* inserirAresta(GA* lista, int verticed, int aresta, int dado_verticed) {
    GA* novaAresta = (GA*)malloc(sizeof(GA));
    if (novaAresta == NULL) {
        printf("Erro: N�o foi poss�vel alocar mem�ria para a nova aresta.\n");
        return lista;
    }

    // Preencher os dados da nova aresta
    novaAresta->verticed = verticed;
    novaAresta->aresta = aresta;
    novaAresta->dado_verticed = dado_verticed; // Armazenar o dado do v�rtice de destino
    novaAresta->proximo = lista; // A nova aresta aponta para o in�cio da lista existente

    // Print dos dados da nova aresta
    printf("Nova aresta criada: verticed = %d, dado verticed = %d, aresta = %d\n", novaAresta->verticed, novaAresta->dado_verticed, novaAresta->aresta);

    return novaAresta; // Retorna o apontador para a nova lista de adjac�ncia
}

// Fun��o para criar as arestas consoante a decis�o do utilizador
void criarArestas(GR* grafos, int opcao) {
    GR* verticeAtual = grafos;

    printf("Vertice atual: %d\n", verticeAtual->vertice);
    // Percorre cada v�rtice na lista ligada
    while (verticeAtual != NULL) {
        printf("Vertice atual: %d\n", verticeAtual->vertice);
        GA* listaArestas = NULL; // Inicializa a lista de adjac�ncias para o v�rtice atual
        int numAresta = 1; // Inicializa o n�mero da aresta

        // Determina os �ndices do v�rtice atual na matriz
        int linha = (verticeAtual->vertice - 1) / 10;
        int coluna = (verticeAtual->vertice) % 10;

        // Verifica a op��o selecionada pelo usu�rio e adiciona as arestas apropriadas
        if (opcao == 1) { // Liga��es diagonais
            // Adiciona a aresta superior esquerda
            if (linha > 1 && coluna > 1) {
                int verticeDestino = ((linha - 1) * 10) + coluna - 1;
                GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++, verticeDestinoPtr->dado);
            }
            // Adiciona a aresta superior direita
            if (linha > 1 && coluna < 5) {
                int verticeDestino = ((linha - 1) * 10) + coluna + 1;
                GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++, verticeDestinoPtr->dado);
            }
            // Adiciona a aresta inferior esquerda
            if (linha < 5 && coluna > 1) {
                int verticeDestino = ((linha + 1) * 10) + coluna - 1;
                GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++, verticeDestinoPtr->dado);
            }
            // Adiciona a aresta inferior direita
            if (linha < 5 && coluna < 5) {
                int verticeDestino = ((linha + 1) * 10) + coluna + 1;
                GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++, verticeDestinoPtr->dado);
            }

            /*/ Ajusta para v�rtices de canto terem apenas uma aresta
            if ((linha == 1 && coluna == 1) ||
                (linha == 1 && coluna == 5) ||
                (linha == 5 && coluna == 1) ||
                (linha == 5 && coluna == 5)) {
                while (listaArestas->proximo != NULL) {
                    GA* temp = listaArestas->proximo;
                    free(listaArestas);
                    listaArestas = temp;
                }
            }
            // Ajusta para v�rtices das bordas terem apenas duas arestas
            else if (linha == 1 || linha == 5 || coluna == 1 || coluna == 5) {
                if (listaArestas != NULL && listaArestas->proximo != NULL) {
                    while (listaArestas->proximo->proximo != NULL) {
                        GA* temp = listaArestas->proximo;
                        free(temp->proximo);
                        listaArestas->proximo = NULL;
                    }
                }
            }*/
        }

        //printf("Vertice atual: %d\n", verticeAtual->vertice);
        // Associa a lista de adjac�ncias ao v�rtice atual
        verticeAtual->arestas = listaArestas;

        // Move para o pr�ximo v�rtice na lista
        verticeAtual = verticeAtual->proximo;
    }
}


// Fun��o para imprimir o grafo com as arestas e n�meros associados
void imprimirGrafo(GR* grafos) {
    GR* verticeAtual = grafos;

    // Percorre cada v�rtice na lista ligada
    while (verticeAtual != NULL) {
        printf("Vertice %02d (Dado: %d):\n", verticeAtual->vertice, verticeAtual->dado);

        // Imprime as arestas associadas ao v�rtice, se houver
        GA* arestaAtual = verticeAtual->arestas;
        if (arestaAtual == NULL) {
            printf("  Sem arestas\n");
        }
        else {
            printf("  Arestas: ");
            while (arestaAtual != NULL) {
                printf("(%02d, verticed: %d) ", arestaAtual->verticed / 10, arestaAtual->dado_verticed);
                arestaAtual = arestaAtual->proximo;
            }
            printf("\n");
        }

        // Move para o pr�ximo v�rtice na lista
        verticeAtual = verticeAtual->proximo;
    }
}



void printGraph(GR* grafos) {
    GR* atual = grafos;

    printf("Lista de vertices:\n");
    // Percorre a lista de v�rtices
    while (atual != NULL) {
        printf("Vertice: %02d, Dado: %d\n", atual->vertice, atual->dado);

        // Move para o pr�ximo v�rtice na lista
        atual = atual->proximo;
    }
}

// Fun��o para exibir o menu e ler a escolha do usu�rio
int menu() {
    int escolha;

    printf("\n------ Menu ------\n");
    printf("0 - Sair\n");
    printf("1 - Criar ligacoes de vertices\n");
    printf("2 - Mostrar vertices e ligacoes\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &escolha);

    return escolha;
}

int main() {
    GR* grafos = NULL;

    // Ler os dados da matriz do ficheiro txt
    lerFicheiro(&grafos, "matriz.txt");

    printGraph(grafos);

    int opcao;

    do {
        opcao = menu();

        switch (opcao) {
        case 0:
            printf("Saindo...\n");
            break;
        case 1:
            printf("Opcao 1 selecionada: Criar ligacoes de vertices\n");
            printf("Escolha o tipo de ligacoes:\n");
            printf("1 - Ligacoes diagonais\n");
            printf("2 - Ligacoes verticais\n");
            printf("3 - Ligacoes horizontais\n");

            int escolha;
            printf("Digite o numero da opcao desejada: ");
            scanf("%d", &escolha);

            // Aqui voc� pode chamar a fun��o para criar liga��es de v�rtices
            criarArestas(grafos, escolha);
            break;
        case 2:
            printf("Opcao 1 selecionada: Mostrar vertices e ligacoes\n");
            printf("Lista de vertices e respetivas arestas:\n");
            imprimirGrafo(grafos);
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}



