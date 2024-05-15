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

// Estrutura para armazenar o n�mero total de linhas e colunas
typedef struct {
    int numLinhas;
    int numColunas;
} DimensoesMatriz;

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

void inserirAresta(GA** listaArestas, int verticeDestino, int dadoVerticeDestino, int numeroAresta) {
    GA* novaAresta = (GA*)malloc(sizeof(GA));
    if (novaAresta == NULL) {
        printf("Erro: N�o foi poss�vel alocar mem�ria para a nova aresta.\n");
        return;
    }

    // Preencher os dados da nova aresta
    novaAresta->verticed = verticeDestino;
    novaAresta->dado_verticed = dadoVerticeDestino;
    novaAresta->aresta = numeroAresta;
    novaAresta->proximo = *listaArestas; // A nova aresta aponta para o in�cio da lista existente
    *listaArestas = novaAresta; // Atualiza o ponteiro da lista para apontar para a nova aresta
}


// Fun��o para calcular o n�mero total de linhas e colunas com base no maior n�mero de v�rtice
DimensoesMatriz calcularLinhasColunas(GR* grafos) {
    DimensoesMatriz dimensoes = { 0, 0 };
    int maiorVertice = 0;
    GR* verticeAtual = grafos;

    // Encontrar o maior n�mero de v�rtice
    while (verticeAtual != NULL) {
        if (verticeAtual->vertice > maiorVertice) {
            maiorVertice = verticeAtual->vertice;
        }
        verticeAtual = verticeAtual->proximo;
    }

    // Calcular o n�mero total de linhas e colunas
    dimensoes.numLinhas = maiorVertice / 10;
    dimensoes.numColunas = maiorVertice % 10;

    return dimensoes;
}

// Fun��o para criar as arestas com base no tamanho da matriz
void criarArestas(GR* grafos, int opcao) {
    DimensoesMatriz dimensoes = calcularLinhasColunas(grafos);
    int numLinhas = dimensoes.numLinhas;
    int numColunas = dimensoes.numColunas;

    // Percorrer todas as linhas e colunas da matriz
    for (int linha = 1; linha <= numLinhas; linha++) {
        for (int coluna = 1; coluna <= numColunas; coluna++) {
            // Determinar o n�mero do v�rtice atual
            int verticeAtualNum = (linha * 10) + coluna;

            // Encontrar o v�rtice atual na lista
            GR* verticeAtualPtr = encontrarVertice(grafos, verticeAtualNum);
            if (verticeAtualPtr == NULL) continue;  // Se o v�rtice atual n�o existir, pula para o pr�ximo

            int numeroAresta = 1; // Inicializar o n�mero da aresta para cada v�rtice

            // Verificar se as arestas diagonais podem ser adicionadas
            if (opcao == 1) { // Liga��es diagonais
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


// Fun��o para imprimir o grafo com as arestas e n�meros associados
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
    // Percorre a lista de v�rtices
    while (atual != NULL) {
        printf("V: %02d, D: %d\n", atual->vertice, atual->dado);

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
