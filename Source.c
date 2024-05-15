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

// Estrutura para armazenar o número total de linhas e colunas
typedef struct {
    int numLinhas;
    int numColunas;
} DimensoesMatriz;

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

void inserirAresta(GA** listaArestas, int verticeDestino, int dadoVerticeDestino, int numeroAresta) {
    GA* novaAresta = (GA*)malloc(sizeof(GA));
    if (novaAresta == NULL) {
        printf("Erro: Não foi possível alocar memória para a nova aresta.\n");
        return;
    }

    // Preencher os dados da nova aresta
    novaAresta->verticed = verticeDestino;
    novaAresta->dado_verticed = dadoVerticeDestino;
    novaAresta->aresta = numeroAresta;
    novaAresta->proximo = *listaArestas; // A nova aresta aponta para o início da lista existente
    *listaArestas = novaAresta; // Atualiza o ponteiro da lista para apontar para a nova aresta
}


// Função para calcular o número total de linhas e colunas com base no maior número de vértice
DimensoesMatriz calcularLinhasColunas(GR* grafos) {
    DimensoesMatriz dimensoes = { 0, 0 };
    int maiorVertice = 0;
    GR* verticeAtual = grafos;

    // Encontrar o maior número de vértice
    while (verticeAtual != NULL) {
        if (verticeAtual->vertice > maiorVertice) {
            maiorVertice = verticeAtual->vertice;
        }
        verticeAtual = verticeAtual->proximo;
    }

    // Calcular o número total de linhas e colunas
    dimensoes.numLinhas = maiorVertice / 10;
    dimensoes.numColunas = maiorVertice % 10;

    return dimensoes;
}

// Função para criar as arestas com base no tamanho da matriz
void criarArestas(GR* grafos, int opcao) {
    DimensoesMatriz dimensoes = calcularLinhasColunas(grafos);
    int numLinhas = dimensoes.numLinhas;
    int numColunas = dimensoes.numColunas;

    // Percorrer todas as linhas e colunas da matriz
    for (int linha = 1; linha <= numLinhas; linha++) {
        for (int coluna = 1; coluna <= numColunas; coluna++) {
            // Determinar o número do vértice atual
            int verticeAtualNum = (linha * 10) + coluna;

            // Encontrar o vértice atual na lista
            GR* verticeAtualPtr = encontrarVertice(grafos, verticeAtualNum);
            if (verticeAtualPtr == NULL) continue;  // Se o vértice atual não existir, pula para o próximo

            int numeroAresta = 1; // Inicializar o número da aresta para cada vértice

            // Verificar se as arestas diagonais podem ser adicionadas
            if (opcao == 1) { // Ligações diagonais
                // Adicionar a aresta superior esquerda
                if (linha > 1 && coluna > 1) {
                    int verticeDestino = ((linha - 1) * 10) + (coluna - 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestino, verticeDestinoPtr->dado, numeroAresta++);
                    }
                }
                // Adicionar a aresta superior direita
                if (linha > 1 && coluna < numColunas) {
                    int verticeDestino = ((linha - 1) * 10) + (coluna + 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestino, verticeDestinoPtr->dado, numeroAresta++);
                    }
                }
                // Adicionar a aresta inferior esquerda
                if (linha < numLinhas && coluna > 1) {
                    int verticeDestino = ((linha + 1) * 10) + (coluna - 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestino, verticeDestinoPtr->dado, numeroAresta++);
                    }
                }
                // Adicionar a aresta inferior direita
                if (linha < numLinhas && coluna < numColunas) {
                    int verticeDestino = ((linha + 1) * 10) + (coluna + 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestino, verticeDestinoPtr->dado, numeroAresta++);
                    }
                }
            }
        }
    }
}


// Função para imprimir o grafo com as arestas e números associados
void imprimirGrafo(GR* grafos) {
    GR* verticeAtual = grafos;
    while (verticeAtual != NULL) {
        printf("Vertice: %d (D: %d):\n", verticeAtual->vertice, verticeAtual->dado);
        GA* arestaAtual = verticeAtual->arestas;
        printf("  Arestas:");
        while (arestaAtual != NULL) {
            printf(" (A-> %d, V-> %d, DV-> %d)", arestaAtual->aresta, arestaAtual->verticed, arestaAtual->dado_verticed);
            arestaAtual = arestaAtual->proximo;
        }
        printf("\n");
        verticeAtual = verticeAtual->proximo;
    }
}


void printGraph(GR* grafos) {
    GR* atual = grafos;

    printf("Lista de vertices:\n");
    // Percorre a lista de vértices
    while (atual != NULL) {
        printf("V: %02d, D: %d\n", atual->vertice, atual->dado);

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
