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
    int dado_verticed; //  campo para armazenar o dado do vértice de destino
    struct ga* proximo;
} GA;

// Estrutura para armazenar o número total de linhas e colunas
typedef struct {
    int numLinhas;
    int numColunas;
} DimensoesMatriz;

int escolha_tipo_ligacao = 0;

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
void inserirAresta(GA** listaArestas, int VerticeDestino, int aresta, int VerticeDestinoDado) {
    GA* novaAresta = (GA*)malloc(sizeof(GA));
    if (novaAresta == NULL) {
        printf("Erro: Não foi possível alocar memória para a nova aresta.\n");
        return;
    }

    // Preencher os dados da nova aresta
    novaAresta->aresta = aresta;
    novaAresta->verticed = VerticeDestino;
    novaAresta->dado_verticed = VerticeDestinoDado;
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

    // Verificar se a matriz é válida
    if (numLinhas == 0 || numColunas == 0) {
        printf("Erro: Dimensões da matriz inválidas.\n");
        return;
    }

    GR* verticeAtual = grafos;

    // Percorrer todas as linhas e colunas da matriz
    for (int linha = 1; linha <= numLinhas; linha++) {
        for (int coluna = 1; coluna <= numColunas; coluna++) {
            // Determinar o número do vértice atual
            int verticeAtualNum = linha * 10 + coluna;

            // Encontrar o vértice atual na lista
            GR* verticeAtualPtr = encontrarVertice(grafos, verticeAtualNum);
            if (verticeAtualPtr == NULL) {
                printf("Erro: Vértice %d não encontrado.\n", verticeAtualNum);
                continue;
            }

            int arestaCount = 1;

            // Verificar se as arestas diagonais podem ser adicionadas
            if (opcao == 1) { // Ligações diagonais
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
            else if (opcao == 2) { // Ligações verticais
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
            else if (opcao == 3) { // Ligações horizontais
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

// Função para remover uma aresta específica de um vértice
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
            printf("Aresta para o vértice %d removida com sucesso.\n", verticeDestino);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("Aresta para o vértice %d não encontrada.\n", verticeDestino);
}

// Função para remover todas as arestas de um vértice
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

// Função para remover todas as arestas de todos os vértices do grafo
void removerTodasArestasGrafo(GR* grafos) {
    GR* verticeAtual = grafos;
    while (verticeAtual != NULL) {
        removerTodasArestas(&(verticeAtual->arestas));
        verticeAtual = verticeAtual->proximo;
    }
    printf("Todas as arestas do grafo foram removidas.\n");
}

// Função para remover um vértice e suas arestas
void removerVertice(GR* grafos, int vertice) {
    // Verificar se a lista de vértices está vazia
    if (grafos == NULL) {
        printf("Erro: A lista de vértices está vazia.\n");
        return;
    }

    GR* anterior = NULL;
    GR* atual = grafos;

    // Procurar o vértice na lista
    while (atual != NULL) {
        if (atual->vertice == vertice) {
            // Se o vértice a ser removido for o primeiro da lista
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

    // Se o vértice não for encontrado, exibir uma mensagem de erro
    printf("Erro: O vértice %d não foi encontrado.\n", vertice);
}



void imprimirVertices(GR* grafos) {
    GR* atual = grafos;

    printf("Lista de vertices:\n");
    // Percorre a lista de vértices
    while (atual != NULL) {
        printf("V: %02d, D: %d\n", atual->vertice, atual->dado);

        // Move para o próximo vértice na lista
        atual = atual->proximo;
    }
}


// Função para exibir o menu de remoção e ler a escolha do usuário
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


// Função para remover arestas baseada na escolha do usuário
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
                printf("Vertice %d não encontrado.\n", vertice);
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
                printf("Vertice %d não encontrado.\n", vertice);
            }
            break;
        }
        case 4: // Remover um vértice e suas arestas
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


// Função para verificar se a ligação entre dois vértices é diagonal
bool ligacaoDiagonal(GR* origem, GA* aresta) {
    // Verifica se a diferença entre os vértices é igual a 11 ou 9, indicando uma ligação diagonal
    return (aresta->verticed - origem->vertice) == 11 || (aresta->verticed - origem->vertice) == 9;
}

// Função para verificar se a ligação entre dois vértices é vertical
bool ligacaoVertical(GR* origem, GA* aresta) {
    // Verifica se a diferença entre os vértices é igual a 10, indicando uma ligação vertical
    return (aresta->verticed - origem->vertice) == 10;
}

// Função para verificar se a ligação entre dois vértices é horizontal
bool ligacaoHorizontal(GR* origem, GA* aresta) {
    // Verifica se a diferença entre os vértices é igual a 1, indicando uma ligação horizontal
    return (aresta->verticed - origem->vertice) == 1;
}

// Função para calcular o somatório máximo possível
void calcularSomatorioMaximo(GR* verticeAtual, int somaAtual, int* somaMaximo, int* caminhoMaximo, bool* visitado, int* caminhoAtual) {
    if (verticeAtual == NULL) {
        return;
    }

    // Adiciona o dado do vértice atual ao somatório atual
    somaAtual += verticeAtual->dado;

    // Marca o vértice atual como visitado
    visitado[verticeAtual->vertice] = true;

    // Atualiza o caminho atual
    caminhoAtual[verticeAtual->vertice] = verticeAtual->dado;

    // Se a soma atual for maior que a soma máxima, atualiza a soma máxima e o caminho máximo
    if (somaAtual > *somaMaximo) {
        *somaMaximo = somaAtual;
        memcpy(caminhoMaximo, caminhoAtual, sizeof(int) * (verticeAtual->vertice + 1));
    }

    // Itera sobre as arestas do vértice atual
    GA* arestaAtual = verticeAtual->arestas;
    while (arestaAtual != NULL) {
        // Se o vértice de destino ainda não foi visitado, chama recursivamente a função DFS
        if (!visitado[arestaAtual->verticed]) {
            calcularSomatorioMaximo(encontrarVertice(verticeAtual, arestaAtual->verticed), somaAtual, somaMaximo, caminhoMaximo, visitado, caminhoAtual);
        }
        arestaAtual = arestaAtual->proximo;
    }

    // Retrocede o caminho atual, atualiza a soma atual e desmarca o vértice atual como não visitado
    somaAtual -= verticeAtual->dado;
    caminhoAtual[verticeAtual->vertice] = 0;
    visitado[verticeAtual->vertice] = false;
}

// Função para iniciar o cálculo do somatório máximo possível
void iniciarCalculoSomatorioMaximo(GR* grafos, int tipoLigacao) {
    if (grafos == NULL) {
        printf("Erro: O grafo está vazio.\n");
        return;
    }
    if (tipoLigacao == 0 || tipoLigacao == NULL) {
        printf("Erro: Tipo de ligacao indefinido ou invalido.");
        return;
    }

    int somaMaximo = 0; // Inicializa a soma máxima como 0
    int caminhoMaximo[1000] = { 0 }; // Define um array para armazenar o caminho máximo e inicializa com 0s
    int caminhoAtual[1000] = { 0 }; // Define um array para armazenar o caminho atual e inicializa com 0s
    bool visitado[1000] = { false }; // Define um array para controlar os vértices visitados e inicializa com falsos

    // Inicia o cálculo do somatório máximo a partir de cada vértice do grafo
    GR* verticeAtual = grafos;
    while (verticeAtual != NULL) {
        calcularSomatorioMaximo(verticeAtual, 0, &somaMaximo, caminhoMaximo, visitado, caminhoAtual);
        verticeAtual = verticeAtual->proximo;
    }

    // Imprime o somatório máximo e o caminho máximo
    printf("Somatorio Maximo: %d\n", somaMaximo);
    printf("Caminho Maximo: ");
    for (int i = 0; i < 1000; i++) {
        if (caminhoMaximo[i] != 0) {
            printf("%d ", caminhoMaximo[i]);
        }
    }
    printf("\n");
}






// Função para exibir o menu e ler a escolha do usuário
int menu() {
    int escolha;

    printf("\n------ Menu ------\n");
    printf("0 - Sair\n");
    printf("1 - Criar ligacoes de vertices\n");
    printf("2 - Mostrar vertices e ligacoes\n");
    printf("3 - Remover vertices ou arestas\n");
    printf("4 - Mostrar somatorio maximo\n");
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

            escolha_tipo_ligacao = escolha;

            // Aqui você pode chamar a função para criar ligações de vértices
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
        case 4:
            printf("Opcao 4 selecionada: Mostrar somatorio maximo\n");
            iniciarCalculoSomatorioMaximo(grafos, escolha_tipo_ligacao);
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}

