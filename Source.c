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
    int dado_verticed; //  campo para armazenar o dado do vértice de destino
    struct ga* proximo;
} GA;

// Função para criar um novo vértice com base no número e adicionar à lista de vértices
GR* criarVertice(int numero, int dado) {
    // Criar um novo vértice na lista ligada GR
    GR* novoVertice = (GR*)malloc(sizeof(GR));
    if (novoVertice == NULL) {
        printf("Erro: Não foi possível alocar memória para o novo vértice.\n");
        exit(1);
    }

    // Atribuir valores ao vértice
    novoVertice->vertice = numero; // Criar o vértice com o número recebido
    novoVertice->dado = dado; // Atribuir o número como dado (opcional)
    novoVertice->proximo = NULL;
    novoVertice->arestas = NULL;

    return novoVertice;
}

// Função para ler a matriz do ficheiro e criar vértices com base nos números
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
            // Criar um novo vértice com o número e adicionar à lista de vértices
            GR* novoVertice = criarVertice((row * 10) + col, numero);
            // Adicionar vértice à lista ligada GR
            if (*grafos == NULL) {
                *grafos = novoVertice;
            }
            else {
                // Encontrar o último nó na lista
                GR* atual = *grafos;
                while (atual->proximo != NULL) {
                    atual = atual->proximo;
                }
                // Inserir o novo vértice no final da lista
                atual->proximo = novoVertice;
            }

            col++;
            token = strtok(NULL, ";");
        }
        row++;
    }
    fclose(file);
}


// Função para encontrar um vértice na lista de vértices com base no índice
GR* encontrarVertice(GR* grafos, int indice) {
    GR* verticeAtual = grafos;

    // Percorre a lista de vértices até encontrar o vértice com o índice desejado
    while (verticeAtual != NULL) {
        if (verticeAtual->vertice == indice) {
            // Retorna o ponteiro para o vértice encontrado
            return verticeAtual;
        }
        verticeAtual = verticeAtual->proximo;
    }

    // Se o vértice não for encontrado, retorna NULL
    return NULL;
}

// Função para inserir uma aresta no início da lista de adjacência de um vértice
GA* inserirAresta(GA* lista, int verticed, int aresta, int dado_verticed) {
    GA* novaAresta = (GA*)malloc(sizeof(GA));
    if (novaAresta == NULL) {
        printf("Erro: Não foi possível alocar memória para a nova aresta.\n");
        return lista;
    }

    // Preencher os dados da nova aresta
    novaAresta->verticed = verticed;
    novaAresta->aresta = aresta;
    novaAresta->dado_verticed = dado_verticed; // Armazenar o dado do vértice de destino
    novaAresta->proximo = lista; // A nova aresta aponta para o início da lista existente

    // Print dos dados da nova aresta
    printf("Nova aresta criada: verticed = %d, dado verticed = %d, aresta = %d\n", novaAresta->verticed, novaAresta->dado_verticed, novaAresta->aresta);

    return novaAresta; // Retorna o apontador para a nova lista de adjacência
}

// Função para criar as arestas consoante a decisão do utilizador
void criarArestas(GR* grafos, int opcao) {
    GR* verticeAtual = grafos;

    printf("Vertice atual: %d\n", verticeAtual->vertice);
    // Percorre cada vértice na lista ligada
    while (verticeAtual != NULL) {
        printf("Vertice atual: %d\n", verticeAtual->vertice);
        GA* listaArestas = NULL; // Inicializa a lista de adjacências para o vértice atual
        int numAresta = 1; // Inicializa o número da aresta

        // Determina os índices do vértice atual na matriz
        int linha = (verticeAtual->vertice - 1) / 10;
        int coluna = (verticeAtual->vertice) % 10;

        // Verifica a opção selecionada pelo usuário e adiciona as arestas apropriadas
        if (opcao == 1) { // Ligações diagonais
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

            /*/ Ajusta para vértices de canto terem apenas uma aresta
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
            // Ajusta para vértices das bordas terem apenas duas arestas
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
        // Associa a lista de adjacências ao vértice atual
        verticeAtual->arestas = listaArestas;

        // Move para o próximo vértice na lista
        verticeAtual = verticeAtual->proximo;
    }
}


// Função para imprimir o grafo com as arestas e números associados
void imprimirGrafo(GR* grafos) {
    GR* verticeAtual = grafos;

    // Percorre cada vértice na lista ligada
    while (verticeAtual != NULL) {
        printf("Vertice %02d (Dado: %d):\n", verticeAtual->vertice, verticeAtual->dado);

        // Imprime as arestas associadas ao vértice, se houver
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

        // Move para o próximo vértice na lista
        verticeAtual = verticeAtual->proximo;
    }
}



void printGraph(GR* grafos) {
    GR* atual = grafos;

    printf("Lista de vertices:\n");
    // Percorre a lista de vértices
    while (atual != NULL) {
        printf("Vertice: %02d, Dado: %d\n", atual->vertice, atual->dado);

        // Move para o próximo vértice na lista
        atual = atual->proximo;
    }
}

// Função para exibir o menu e ler a escolha do usuário
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

            // Aqui você pode chamar a função para criar ligações de vértices
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



