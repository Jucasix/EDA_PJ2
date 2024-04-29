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

// Fun��o para ler a matriz do ficheiro e criar v�rtices com base nos n�meros
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
            // Criar um novo v�rtice na lista ligada GR
            GR* novoVertice = (GR*)malloc(sizeof(GR));
            if (novoVertice == NULL) {
                printf("Erro: N�o foi poss�vel alocar mem�ria para o novo v�rtice.\n");
                exit(1);
            }

            // Atribuir valores ao v�rtice
            novoVertice->vertice = row * 10 + col; // Criar o vertice 00, 01, ..., 44
            novoVertice->dado = atoi(token); // Atribuir o n�mero como dado
            novoVertice->proximo = NULL;
            novoVertice->arestas = NULL;

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

// Fun��o para inserir uma aresta no in�cio da lista de adjac�ncia de um v�rtice
GA* inserirAresta(GA* lista, int verticed, int aresta) {
    GA* novaAresta = (GA*)malloc(sizeof(GA));
    if (novaAresta == NULL) {
        printf("Erro: N�o foi poss�vel alocar mem�ria para a nova aresta.\n");
        return lista;
    }

    // Preencher os dados da nova aresta
    novaAresta->verticed = verticed;
    novaAresta->aresta = aresta;
    novaAresta->proximo = lista; // A nova aresta aponta para o in�cio da lista existente

    return novaAresta; // Retorna o apontador para a nova lista de adjac�ncia
}

//Fun��o para criar as arestas consoante a decis�o do utilizador
void criarArestas(GR* grafos, int opcao) {
    GR* verticeAtual = grafos;

    // Percorre cada v�rtice na lista ligada
    while (verticeAtual != NULL) {
        GA* listaArestas = NULL; // Inicializa a lista de adjac�ncias para o v�rtice atual
        int numAresta = 1; // Inicializa o n�mero da aresta

        // Determina os �ndices do v�rtice atual na matriz
        int linha = verticeAtual->vertice / 5;
        int coluna = verticeAtual->vertice % 5;

        // Verifica a op��o selecionada pelo usu�rio e adiciona as arestas apropriadas
        if (opcao == 1) { // Liga��es diagonais
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
        else if (opcao == 2) { // Liga��es verticais
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
        else if (opcao == 3) { // Liga��es horizontais
            // Adiciona a aresta � esquerda
            if (coluna > 0) {
                int verticeDestino = linha * 5 + coluna - 1;
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++);
            }
            // Adiciona a aresta � direita
            if (coluna < 4) {
                int verticeDestino = linha * 5 + coluna + 1;
                listaArestas = inserirAresta(listaArestas, verticeDestino, numAresta++);
            }
        }

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
        printf("Vertice %d (Dado: %d):\n", verticeAtual->vertice, verticeAtual->dado);

        // Imprime as arestas associadas ao v�rtice, se houver
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


    printGraph(grafos);

  
    return 0;
}



