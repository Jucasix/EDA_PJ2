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

// Fun��o para inserir uma aresta no in�cio da lista de adjac�ncia de um v�rtice
void inserirAresta(GA** listaArestas, int VerticeDestino, int aresta, int VerticeDestinoDado) {
    GA* novaAresta = (GA*)malloc(sizeof(GA));
    if (novaAresta == NULL) {
        printf("Erro: N�o foi poss�vel alocar mem�ria para a nova aresta.\n");
        return;
    }

    // Preencher os dados da nova aresta
    novaAresta->aresta = aresta;
    novaAresta->verticed = VerticeDestino;
    novaAresta->dado_verticed = VerticeDestinoDado;
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

    // Verificar se a matriz � v�lida
    if (numLinhas == 0 || numColunas == 0) {
        printf("Erro: Dimens�es da matriz inv�lidas.\n");
        return;
    }

    GR* verticeAtual = grafos;

    // Percorrer todas as linhas e colunas da matriz
    for (int linha = 1; linha <= numLinhas; linha++) {
        for (int coluna = 1; coluna <= numColunas; coluna++) {
            // Determinar o n�mero do v�rtice atual
            int verticeAtualNum = linha * 10 + coluna;

            // Encontrar o v�rtice atual na lista
            GR* verticeAtualPtr = encontrarVertice(grafos, verticeAtualNum);
            if (verticeAtualPtr == NULL) {
                printf("Erro: V�rtice %d n�o encontrado.\n", verticeAtualNum);
                continue;
            }

            int arestaCount = 1;

            // Verificar se as arestas diagonais podem ser adicionadas
            if (opcao == 1) { // Liga��es diagonais
                // Adicionar a aresta superior esquerda
                if (linha > 1 && coluna > 1) {
                    int verticeDestino = ((linha - 1) * 10) + (coluna - 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, arestaCount++, verticeDestinoPtr->dado);
                    }
                }
                // Adicionar a aresta superior direita
                if (linha > 1 && coluna < numColunas) {
                    int verticeDestino = ((linha - 1) * 10) + (coluna + 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, arestaCount++, verticeDestinoPtr->dado);
                    }
                }
                // Adicionar a aresta inferior esquerda
                if (linha < numLinhas && coluna > 1) {
                    int verticeDestino = ((linha + 1) * 10) + (coluna - 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, arestaCount++, verticeDestinoPtr->dado);
                    }
                }
                // Adicionar a aresta inferior direita
                if (linha < numLinhas && coluna < numColunas) {
                    int verticeDestino = ((linha + 1) * 10) + (coluna + 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, arestaCount++, verticeDestinoPtr->dado);
                    }
                }
            }
            else if (opcao == 2) { // Liga��es verticais
                // Adicionar a aresta superior
                if (linha > 1) {
                    int verticeDestino = ((linha - 1) * 10) + coluna;
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, arestaCount++, verticeDestinoPtr->dado);
                    }
                }
                // Adicionar a aresta inferior
                if (linha < numLinhas) {
                    int verticeDestino = ((linha + 1) * 10) + coluna;
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, arestaCount++, verticeDestinoPtr->dado);
                    }
                }
            }
            else if (opcao == 3) { // Liga��es horizontais
                // Adicionar a aresta esquerda
                if (coluna > 1) {
                    int verticeDestino = (linha * 10) + (coluna - 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, arestaCount++, verticeDestinoPtr->dado);
                    }
                }
                // Adicionar a aresta direita
                if (coluna < numColunas) {
                    int verticeDestino = (linha * 10) + (coluna + 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, arestaCount++, verticeDestinoPtr->dado);
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

// Fun��o para remover uma aresta espec�fica de um v�rtice
void removerAresta(GA** listaArestas, int verticeDestino) {
    GA* atual = *listaArestas;
    GA* anterior = NULL;

    while (atual != NULL) {
        if (atual->verticed == verticeDestino) {
            if (anterior == NULL) {
                *listaArestas = atual->proximo;
            }
            else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            printf("Aresta para o v�rtice %d removida com sucesso.\n", verticeDestino);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("Aresta para o v�rtice %d n�o encontrada.\n", verticeDestino);
}

// Fun��o para remover todas as arestas de um v�rtice
void removerTodasArestas(GA** listaArestas) {
    GA* atual = *listaArestas;
    while (atual != NULL) {
        GA* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    *listaArestas = NULL;
    printf("Todas as arestas foram removidas.\n");
}

// Fun��o para remover todas as arestas de todos os v�rtices do grafo
void removerTodasArestasGrafo(GR* grafos) {
    GR* verticeAtual = grafos;
    while (verticeAtual != NULL) {
        removerTodasArestas(&(verticeAtual->arestas));
        verticeAtual = verticeAtual->proximo;
    }
    printf("Todas as arestas do grafo foram removidas.\n");
}

// Fun��o para remover um v�rtice e suas arestas
void removerVertice(GR* grafos, int vertice) {
    // Verificar se a lista de v�rtices est� vazia
    if (grafos == NULL) {
        printf("Erro: A lista de v�rtices est� vazia.\n");
        return;
    }

    GR* anterior = NULL;
    GR* atual = grafos;

    // Procurar o v�rtice na lista
    while (atual != NULL) {
        if (atual->vertice == vertice) {
            // Se o v�rtice a ser removido for o primeiro da lista
            if (anterior == NULL) {
                grafos = atual->proximo;
            }
            else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    // Se o v�rtice n�o for encontrado, exibir uma mensagem de erro
    printf("Erro: O v�rtice %d n�o foi encontrado.\n", vertice);
}



void imprimirVertices(GR* grafos) {
    GR* atual = grafos;

    printf("Lista de vertices:\n");
    // Percorre a lista de v�rtices
    while (atual != NULL) {
        printf("V: %02d, D: %d\n", atual->vertice, atual->dado);

        // Move para o pr�ximo v�rtice na lista
        atual = atual->proximo;
    }
}


// Fun��o para exibir o menu de remo��o e ler a escolha do usu�rio
int menuRemover() {
    int escolha;

    printf("\n------ Menu Remover ------\n");
    printf("0 - Sair\n");
    printf("1 - Remover todas as arestas\n");
    printf("2 - Remover arestas de um vertice\n");
    printf("3 - Remover uma aresta de um vertice\n");
    printf("4 - Remover um vertice e suas arestas\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &escolha);

    return escolha;
}


// Fun��o para remover arestas baseada na escolha do usu�rio
void executarRemover(GR* grafos) {
    int escolhaRemover;
    do {
        escolhaRemover = menuRemover();

        switch (escolhaRemover) {
        case 0:
            printf("Saindo do menu remover...\n");
            break;
        case 1:
            printf("Removendo todas as arestas do grafo...\n");
            removerTodasArestasGrafo(grafos);
            break;
        case 2:
        {
            int vertice;
            printf("Digite o vertice para remover todas as suas arestas: ");
            scanf("%d", &vertice);
            GR* verticePtr = encontrarVertice(grafos, vertice);
            if (verticePtr != NULL) {
                removerTodasArestas(&(verticePtr->arestas));
            }
            else {
                printf("Vertice %d n�o encontrado.\n", vertice);
            }
            break;
        }
        case 3:
        {
            int vertice, verticeDestino;
            printf("Digite o vertice para remover uma aresta: ");
            scanf("%d", &vertice);
            printf("Digite o vertice de destino da aresta: ");
            scanf("%d", &verticeDestino);
            GR* verticePtr = encontrarVertice(grafos, vertice);
            if (verticePtr != NULL) {
                removerAresta(&(verticePtr->arestas), verticeDestino);
            }
            else {
                printf("Vertice %d n�o encontrado.\n", vertice);
            }
            break;
        }
        case 4: // Remover um v�rtice e suas arestas
            int vertice;
            printf("Digite o numero do vertice para remover: ");
            scanf("%d", &vertice);
            removerVertice(grafos, vertice);
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    } while (escolhaRemover != 0);
}



// Fun��o para exibir o menu e ler a escolha do usu�rio
int menu() {
    int escolha;

    printf("\n------ Menu ------\n");
    printf("0 - Sair\n");
    printf("1 - Criar ligacoes de vertices\n");
    printf("2 - Mostrar vertices e ligacoes\n");
    printf("3 - Remover vertices ou arestas\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &escolha);

    return escolha;
}

int main() {
    GR* grafos = NULL;

    // Ler os dados da matriz do ficheiro txt
    lerFicheiro(&grafos, "matriz.txt");

    imprimirVertices(grafos);

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
            printf("Opcao 2 selecionada: Mostrar vertices e ligacoes\n");
            printf("Lista de vertices e respetivas arestas:\n");
            imprimirGrafo(grafos);
            break;
        case 3:
            printf("Opcao 3 selecionada: Remover vertices ou arestas\n");
            executarRemover(grafos);
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}

