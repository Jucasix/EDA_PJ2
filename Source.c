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
    struct ga* proximo;
} GA;

// Função para ler a matriz do ficheiro e criar vértices com base nos números
void loadGraphFromFile(GR** grafos, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        exit(1);
    }

    char line[1000];
    int row = 0;
    int col = 0;
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ";");
        col = 0;
        while (token != NULL) {
            // Criar um novo vértice na lista ligada GR
            GR* novoVertice = (GR*)malloc(sizeof(GR));
            if (novoVertice == NULL) {
                printf("Erro: Não foi possível alocar memória para o novo vértice.\n");
                exit(1);
            }

            // Atribuir valores ao vértice
            novoVertice->vertice = row * 10 + col; // Criar o vertice 00, 01, ..., 44
            novoVertice->dado = atoi(token); // Atribuir o número como dado
            novoVertice->proximo = NULL;
            novoVertice->arestas = NULL;

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

// Função para inserir uma aresta no início da lista de adjacência de um vértice
GA* inserirAresta(GA* lista, int verticed, int aresta) {
    GA* novaAresta = (GA*)malloc(sizeof(GA));
    if (novaAresta == NULL) {
        printf("Erro: Não foi possível alocar memória para a nova aresta.\n");
        return lista;
    }

    // Preencher os dados da nova aresta
    novaAresta->verticed = verticed;
    novaAresta->aresta = aresta;
    novaAresta->proximo = lista; // A nova aresta aponta para o início da lista existente

    return novaAresta; // Retorna o apontador para a nova lista de adjacência
}

//Função para criar as arestas consoante a decisão do utilizador
void criarArestas(GR* grafos, int opcao) {
    GR* verticeAtual = grafos;

    // Percorre cada vértice na lista ligada
    while (verticeAtual != NULL) {
        GA* listaArestas = NULL; // Inicializa a lista de adjacências para o vértice atual
        int numAresta = 1; // Inicializa o número da aresta

        // Determina os índices do vértice atual na matriz
        int linha = verticeAtual->vertice / 5;
        int coluna = verticeAtual->vertice % 5;

        // Verifica a opção selecionada pelo usuário e adiciona as arestas apropriadas
        if (opcao == 1) { // Ligações diagonais
            // Adiciona a aresta superior esquerda
            if (linha > 0 && coluna > 0) {
                int verticeDestino = (linha - 1) * 5 + coluna - 1;
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++);
            }
            // Adiciona a aresta superior direita
            if (linha > 0 && coluna < 4) {
                int verticeDestino = (linha - 1) * 5 + coluna + 1;
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++);
            }
            // Adiciona a aresta inferior esquerda
            if (linha < 4 && coluna > 0) {
                int verticeDestino = (linha + 1) * 5 + coluna - 1;
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++);
            }
            // Adiciona a aresta inferior direita
            if (linha < 4 && coluna < 4) {
                int verticeDestino = (linha + 1) * 5 + coluna + 1;
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++);
            }
        }
        else if (opcao == 2) { // Ligações verticais
            // Adiciona a aresta acima
            if (linha > 0) {
                int verticeDestino = (linha - 1) * 5 + coluna;
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++);
            }
            // Adiciona a aresta abaixo
            if (linha < 4) {
                int verticeDestino = (linha + 1) * 5 + coluna;
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++);
            }
        }
        else if (opcao == 3) { // Ligações horizontais
            // Adiciona a aresta à esquerda
            if (coluna > 0) {
                int verticeDestino = linha * 5 + coluna - 1;
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++);
            }
            // Adiciona a aresta à direita
            if (coluna < 4) {
                int verticeDestino = linha * 5 + coluna + 1;
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++);
            }
        }

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
        printf("Vertice %d (Dado: %d):\n", verticeAtual->vertice, verticeAtual->dado);

        // Imprime as arestas associadas ao vértice, se houver
        GA* arestaAtual = verticeAtual->arestas;
        if (arestaAtual == NULL) {
            printf("  Sem arestas\n");
        }
        else {
            printf("  Arestas: ");
            while (arestaAtual != NULL) {
                printf("(%d, %d) ", arestaAtual->aresta, arestaAtual->verticed);
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

    // Ler os dados da matriz do arquivo txt
    loadGraphFromFile(&grafos, "matriz.txt");

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


    printGraph(grafos);

  
    return 0;
}



