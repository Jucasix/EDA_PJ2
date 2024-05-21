#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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

int escolha_tipo_ligacao = 0;
// Flags para rastrear se cada tipo de liga��o foi escolhido
bool diagonal_escolhida = false;
bool vertical_escolhida = false;
bool horizontal_escolhida = false;

// Fun��o para criar um novo v�rtice com base no n�mero e adicionar � lista de v�rtices
GR* criarVertice(int numero, int dado) {
    // Criar um novo v�rtice na lista ligada GR
    GR* novoVertice = (GR*)malloc(sizeof(GR));
    if (novoVertice == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para o novo vertice.\n");
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

// Fun��o para determinar na lista de arestas de um v�rtice o maior �ndice de aresta
int encontrarMaiorIndiceAresta(GR* grafos) {
    int maiorIndice = 0;
    GR* verticeAtual = grafos;

    while (verticeAtual != NULL) {
        GA* arestaAtual = verticeAtual->arestas;
        while (arestaAtual != NULL) {
            if (arestaAtual->aresta > maiorIndice) {
                maiorIndice = arestaAtual->aresta;
            }
            arestaAtual = arestaAtual->proximo;
        }
        verticeAtual = verticeAtual->proximo;
    }

    return maiorIndice;
}

// Fun��o para inserir uma aresta no in�cio da lista de adjac�ncia de um v�rtice
void inserirAresta(GA** listaArestas, int VerticeDestino, int aresta, int VerticeDestinoDado) {
    GA* novaAresta = (GA*)malloc(sizeof(GA));
    if (novaAresta == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para a nova aresta.\n");
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

    // Encontrar o maior �ndice de aresta existente
    int maiorIndice = encontrarMaiorIndiceAresta(grafos);
    int novoIndice = maiorIndice + 1;

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

            // Verificar se as arestas diagonais podem ser adicionadas
            if (opcao == 1) { // Liga��es diagonais
                // Adicionar a aresta superior esquerda
                if (linha > 1 && coluna > 1) {
                    int verticeDestino = ((linha - 1) * 10) + (coluna - 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, novoIndice++, verticeDestinoPtr->dado);
                    }
                }
                // Adicionar a aresta superior direita
                if (linha > 1 && coluna < numColunas) {
                    int verticeDestino = ((linha - 1) * 10) + (coluna + 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, novoIndice++, verticeDestinoPtr->dado);
                    }
                }
                // Adicionar a aresta inferior esquerda
                if (linha < numLinhas && coluna > 1) {
                    int verticeDestino = ((linha + 1) * 10) + (coluna - 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, novoIndice++, verticeDestinoPtr->dado);
                    }
                }
                // Adicionar a aresta inferior direita
                if (linha < numLinhas && coluna < numColunas) {
                    int verticeDestino = ((linha + 1) * 10) + (coluna + 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, novoIndice++, verticeDestinoPtr->dado);
                    }
                }
            }
            else if (opcao == 2) { // Liga��es verticais
                // Adicionar a aresta superior
                if (linha > 1) {
                    int verticeDestino = ((linha - 1) * 10) + coluna;
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, novoIndice++, verticeDestinoPtr->dado);
                    }
                }
                // Adicionar a aresta inferior
                if (linha < numLinhas) {
                    int verticeDestino = ((linha + 1) * 10) + coluna;
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, novoIndice++, verticeDestinoPtr->dado);
                    }
                }
            }
            else if (opcao == 3) { // Liga��es horizontais
                // Adicionar a aresta esquerda
                if (coluna > 1) {
                    int verticeDestino = (linha * 10) + (coluna - 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, novoIndice++, verticeDestinoPtr->dado);
                    }
                }
                // Adicionar a aresta direita
                if (coluna < numColunas) {
                    int verticeDestino = (linha * 10) + (coluna + 1);
                    GR* verticeDestinoPtr = encontrarVertice(grafos, verticeDestino);
                    if (verticeDestinoPtr != NULL) {
                        inserirAresta(&(verticeAtualPtr->arestas), verticeDestinoPtr->vertice, novoIndice++, verticeDestinoPtr->dado);
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
            printf("Aresta para o vertice %d removida com sucesso.\n", verticeDestino);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("Aresta para o vertice %d n�o encontrada.\n", verticeDestino);
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

// Fun��o auxiliar para remover todas as arestas com destino ao v�rtice especificado
void removerArestasComDestino(GR* grafos, int destino) {
    GR* atual = grafos;

    // Percorrer todos os v�rtices
    while (atual != NULL) {
        // Verificar se existem arestas com destino ao v�rtice especificado
        GA* anteriorAresta = NULL;
        GA* atualAresta = atual->arestas;
        while (atualAresta != NULL) {
            if (atualAresta->verticed == destino) {
                // Remover a aresta
                if (anteriorAresta == NULL) {
                    atual->arestas = atualAresta->proximo;
                }
                else {
                    anteriorAresta->proximo = atualAresta->proximo;
                }
                free(atualAresta);
                atualAresta = NULL; // Sair do loop
            }
            else {
                anteriorAresta = atualAresta;
                atualAresta = atualAresta->proximo;
            }
        }
        atual = atual->proximo;
    }
}

// Fun��o para remover um v�rtice e suas arestas
void removerVertice(GR* grafos, int vertice) {
    // Verificar se a lista de v�rtices est� vazia
    if (grafos == NULL) {
        printf("Erro: A lista de vertices esta vazia.\n");
        return;
    }

    GR* anterior = NULL;
    GR* atual = grafos;

    // Procurar o v�rtice na lista
    while (atual != NULL) {
        if (atual->vertice == vertice) {
            // Remover todas as arestas com destino a este v�rtice
            removerArestasComDestino(grafos, vertice);

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
    printf("Erro: O vertice %d nao foi encontrado.\n", vertice);
}

// Fun��o para imprimir vertices
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
    printf("0 - Voltar ao menu principal\n");
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
        int vertice, verticeDestino;

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
            printf("Digite o vertice para remover todas as suas arestas: ");
            scanf("%d", &vertice);
            GR* verticePtr = encontrarVertice(grafos, vertice);
            if (verticePtr != NULL) {
                removerTodasArestas(&(verticePtr->arestas));
            }
            else {
                printf("Vertice %d nao encontrado.\n", vertice);
            }
            break;
        }
        case 3:
        {
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

// Fun��o auxiliar para encontrar o n�mero de vertices existetes na lista
int contarVertices(GR* grafos) {
    int contador = 0;
    GR* atual = grafos;

    // Percorre a lista ligada contando os v�rtices
    while (atual != NULL) {
        contador++;
        atual = atual->proximo;
    }

    return contador;
}

// Fun��o auxiliar para imprimir o caminho armazenado no array
void imprimirCaminho(int caminho[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (i > 0) {
            printf("->");
        }
        printf("%d", caminho[i]);
    }
    printf("\n");
}

// Fun��o auxiliar para realizar DFS e imprimir caminhos
void DFSUtil(GR* grafo, int v, int visitado[], int caminho[], int pos) {
    // Marca o v�rtice atual como visitado e adiciona ao caminho
    visitado[v] = 1;
    caminho[pos] = v;
    pos++;

    // Procura o v�rtice atual no grafo
    GR* vertice = grafo;
    while (vertice != NULL && vertice->vertice != v)
        vertice = vertice->proximo;

    int contador = 1;

    // Se o v�rtice atual n�o tem mais adjacentes n�o visitados, imprime o caminho
    if (vertice == NULL || vertice->arestas == NULL) {
        printf("Caminho %d: ", contador++);
        imprimirCaminho(caminho, pos);
    }
    else {
        // Verifica os adjacentes n�o visitados
        GA* adj = vertice->arestas;
        int temAdjacenteNaoVisitado = 0;
        while (adj != NULL) {
            if (!visitado[adj->verticed]) {
                temAdjacenteNaoVisitado = 1;
                DFSUtil(grafo, adj->verticed, visitado, caminho, pos);
            }
            adj = adj->proximo;
        }

        // Se n�o h� mais adjacentes n�o visitados, imprime o caminho
        if (!temAdjacenteNaoVisitado) {
            imprimirCaminho(caminho, pos);
        }
    }

    // Desmarca o v�rtice atual (backtracking)
    visitado[v] = 0;
    pos--;
}

// Fun��o para iniciar a DFS
void DFS(GR* grafo, int v, int numVertices) {
    int* visitado = (int*)malloc(numVertices * sizeof(int));
    int* caminho = (int*)malloc(numVertices * sizeof(int));
    for (int i = 0; i < numVertices; i++)
        visitado[i] = 0;

    // Chama a fun��o auxiliar para fazer a DFS
    DFSUtil(grafo, v, visitado, caminho, 0);

    //free(visitado);
    //free(caminho);
}

// Fun��o para calcular o somat�rio m�ximo poss�vel
void calcularSomatorioMaximo(GR* verticeAtual, int somaAtual, int* somaMaximo, int* caminhoMaximo, bool* visitado, int* caminhoAtual, int* tamanhoCaminhoAtual, int* tamanhoCaminhoMaximo) {
    if (verticeAtual == NULL) {
        return;
    }

    somaAtual += verticeAtual->dado;
    visitado[verticeAtual->vertice] = true;
    caminhoAtual[(*tamanhoCaminhoAtual)++] = verticeAtual->vertice;

    if (somaAtual > *somaMaximo) {
        *somaMaximo = somaAtual;
        memcpy(caminhoMaximo, caminhoAtual, sizeof(int) * (*tamanhoCaminhoAtual));
        *tamanhoCaminhoMaximo = *tamanhoCaminhoAtual;
    }

    GA* arestaAtual = verticeAtual->arestas;
    while (arestaAtual != NULL) {
        if (!visitado[arestaAtual->verticed]) {
            GR* proxVertice = encontrarVertice(verticeAtual, arestaAtual->verticed);
            calcularSomatorioMaximo(proxVertice, somaAtual, somaMaximo, caminhoMaximo, visitado, caminhoAtual, tamanhoCaminhoAtual, tamanhoCaminhoMaximo);
        }
        arestaAtual = arestaAtual->proximo;
    }

    somaAtual -= verticeAtual->dado;
    visitado[verticeAtual->vertice] = false;
    (*tamanhoCaminhoAtual)--;
}

// Fun��o para iniciar o c�lculo do somat�rio m�ximo poss�vel
void iniciarCalculoSomatorioMaximo(GR* grafos) {
    if (grafos == NULL) {
        printf("Erro: O grafo est� vazio.\n");
        return;
    }

    bool temArestas = false;
    GR* verticeAtual = grafos;
    while (verticeAtual != NULL) {
        if (verticeAtual->arestas != NULL) {
            temArestas = true;
            break;
        }
        verticeAtual = verticeAtual->proximo;
    }

    if (!temArestas) {
        printf("Erro: Grafo nao criado.\n");
        return;
    }

    int somaMaximo = 0;
    int caminhoMaximo[1000] = { 0 };
    int caminhoAtual[1000] = { 0 };
    int tamanhoCaminhoAtual = 0;
    int tamanhoCaminhoMaximo = 0;
    bool visitado[1000] = { false };

    verticeAtual = grafos;
    while (verticeAtual != NULL) {
        calcularSomatorioMaximo(verticeAtual, 0, &somaMaximo, caminhoMaximo, visitado, caminhoAtual, &tamanhoCaminhoAtual, &tamanhoCaminhoMaximo);
        verticeAtual = verticeAtual->proximo;
    }

    printf("Somatorio Maximo: %d\n", somaMaximo);
    printf("Caminho Maximo: ");
    for (int i = 0; i < tamanhoCaminhoMaximo; i++) {
        printf("%d ", caminhoMaximo[i]);
    }
    printf("\n");
}

// Fun��o auxiliar para encontrar um caminho entre v�rtices e somar os dados dos v�rtices no caminho
void encontrarCaminhoESomarDadosUtil(GR* verticeAtual, int verticeDestino, int somaAtual, int* somaMaximo, int* caminhoMaximo, bool* visitado, int* caminhoAtual, int* tamanhoCaminhoAtual, int* tamanhoCaminhoMaximo) {
    if (verticeAtual == NULL) {
        return;
    }

    // Marcar o v�rtice atual como visitado e adicionar ao caminho atual
    visitado[verticeAtual->vertice] = true;
    caminhoAtual[(*tamanhoCaminhoAtual)++] = verticeAtual->vertice;

    // Se encontrarmos o v�rtice de destino, verificar se a soma atual � a maior e atualizar
    if (verticeAtual->vertice == verticeDestino && somaAtual > *somaMaximo) {
        *somaMaximo = somaAtual;
        memcpy(caminhoMaximo, caminhoAtual, sizeof(int) * (*tamanhoCaminhoAtual));
        *tamanhoCaminhoMaximo = *tamanhoCaminhoAtual;
    }

    // Percorrer todas as arestas do v�rtice atual
    GA* arestaAtual = verticeAtual->arestas;
    while (arestaAtual != NULL) {
        // Verificar se o v�rtice de destino da aresta ainda n�o foi visitado
        if (!visitado[arestaAtual->verticed]) {
            // Encontrar o pr�ximo v�rtice na lista de v�rtices
            GR* proxVertice = encontrarVertice(verticeAtual, arestaAtual->verticed);
            // Verificar se o pr�ximo v�rtice foi encontrado
            if (proxVertice != NULL) {
                // Chamar recursivamente a fun��o para o pr�ximo v�rtice
                encontrarCaminhoESomarDadosUtil(proxVertice, verticeDestino, somaAtual + proxVertice->dado, somaMaximo, caminhoMaximo, visitado, caminhoAtual, tamanhoCaminhoAtual, tamanhoCaminhoMaximo);
            }
        }
        arestaAtual = arestaAtual->proximo;
    }

    // Desmarcar o v�rtice atual (backtracking)
    visitado[verticeAtual->vertice] = false;
    (*tamanhoCaminhoAtual)--;
}

// Fun��o para encontrar um caminho entre dois v�rtices e calcular a soma dos dados
int encontrarCaminhoESomarDados(GR* grafo, int verticeOrigem, int verticeDestino) {
    // Verificar se os v�rtices de origem e destino existem
    GR* verticeOrigemPtr = encontrarVertice(grafo, verticeOrigem);
    GR* verticeDestinoPtr = encontrarVertice(grafo, verticeDestino);
    if (verticeOrigemPtr == NULL || verticeDestinoPtr == NULL) {
        printf("Erro: Vertice de origem ou destino nao encontrado.\n");
        return -1; // Retorna -1 se algum dos v�rtices n�o for encontrado
    }

    // Inicializar vari�veis para a busca em profundidade (DFS)
    bool visitado[1000] = { false };
    int caminho[1000] = { 0 };
    int tamanhoCaminho = 0;
    int somaDados = 0;
    int somaMaxima = 0;
    int caminhoMaximo[1000] = { 0 };
    int tamanhoCaminhoMaximo = 0;

    // Chamada recursiva da fun��o auxiliar para encontrar o caminho e calcular a soma dos dados
    encontrarCaminhoESomarDadosUtil(verticeOrigemPtr, verticeDestino, 0, &somaMaxima, caminhoMaximo, visitado, caminho, &tamanhoCaminho, &tamanhoCaminhoMaximo);

    // Verificar se um caminho foi encontrado
    if (somaMaxima > 0) {
        // Imprimir o caminho encontrado
        printf("Caminho encontrado: ");
        for (int i = 0; i < tamanhoCaminhoMaximo; i++) {
            printf("%d ", caminhoMaximo[i]);
            if (i < tamanhoCaminhoMaximo - 1) {
                printf("-> ");
            }
        }
        printf("\n");
        // Retornar a soma dos dados dos v�rtices no caminho
        return somaMaxima;
    }
    else {
        printf("Nenhum caminho encontrado entre os vertices %d e %d.\n", verticeOrigem, verticeDestino);
        return 0; // Retorna 0 se nenhum caminho for encontrado
    }
}

// Fun��o para exibir o menu e ler a escolha do usu�rio
int menu() {
    int escolha;

    printf("\n------ Menu ------\n");
    printf("0 - Sair\n");
    printf("1 - Criar ligacoes de vertices\n");
    printf("2 - Mostrar vertices e ligacoes\n");
    printf("3 - Remover vertices ou arestas\n");
    printf("4 - Mostrar somatorio maximo\n");
    printf("5 - Procura em profundidade\n");
    printf("6 - Somatorio entre dois vertices\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &escolha);

    return escolha;
}

int main() {
    GR* grafos = NULL;

    // Ler os dados da matriz do ficheiro txt
    lerFicheiro(&grafos, "matriz.txt");

    imprimirVertices(grafos);

    int opcao, verticeOrigem, verticeDestino, numVertices;

    do {
        opcao = menu();

        switch (opcao) {
        case 0:
            printf("Saindo...\n");
            break;
        case 1:
            do {
                printf("Escolha o tipo de ligacao:\n");
                printf("0 - Voltar ao menu principal\n");
                printf("1 - Diagonais\n");
                printf("2 - Verticais\n");
                printf("3 - Horizontais\n");
                printf("Digite sua escolha: ");
                scanf("%d", &escolha_tipo_ligacao);

                if (escolha_tipo_ligacao == 0) {
                    printf("Voltando ao menu principal.\n");
                    break;  // Volta ao menu principal
                }

                if ((escolha_tipo_ligacao == 1 && diagonal_escolhida) ||
                    (escolha_tipo_ligacao == 2 && vertical_escolhida) ||
                    (escolha_tipo_ligacao == 3 && horizontal_escolhida)) {
                    printf("Tipo de ligacao ja escolhida. Por favor, escolha outro tipo de ligacao.\n");
                }
                else {
                    if (escolha_tipo_ligacao == 1) {
                        diagonal_escolhida = true;
                    }
                    else if (escolha_tipo_ligacao == 2) {
                        vertical_escolhida = true;
                    }
                    else if (escolha_tipo_ligacao == 3) {
                        horizontal_escolhida = true;
                    }
                    criarArestas(grafos, escolha_tipo_ligacao);
                }
            } while (!(diagonal_escolhida && vertical_escolhida && horizontal_escolhida));
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
        case 4:
            printf("Opcao 4 selecionada: Mostrar somatorio maximo\n");
            iniciarCalculoSomatorioMaximo(grafos, escolha_tipo_ligacao);
            break;
        case 5:
            printf("Opcao 5 selecionada: Procura em profundidade\n");
            printf("Indique o vertice de origem\n");
            scanf("%d", &verticeOrigem);
            numVertices = contarVertices(grafos);
            if (numVertices > 0) {
                DFS(grafos, verticeOrigem, numVertices);
            }
            else {
                printf("Erro: Grafo vazio.\n");
            }
            break;
        case 6:
            printf("Opcao 6 selecionada: Somatorio entre dois vertices\n");
            printf("Indique o vertice de origem\n");
            scanf("%d", &verticeOrigem);
            printf("Indique o vertice de destino\n");
            scanf("%d", &verticeDestino);
            int somaDadosCaminho = encontrarCaminhoESomarDados(grafos, verticeOrigem, verticeDestino);
            if (somaDadosCaminho > 0) {
                printf("Soma dos dados do caminho encontrado: %d\n", somaDadosCaminho);
            }
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}

