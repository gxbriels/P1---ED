#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int ID;
  char nome[50];
  char categoria[50];
  char marca[30];
  float preco;
  int garantia;
  int qtd_estoque;
} t_item;

typedef struct {
  t_item vet[100];
  int tam;
} t_tabela;

// Função para carregar a tabela do arquivo
t_tabela LoadTab(char fileName[]) {
  FILE *arq = fopen(fileName, "rt");
  t_tabela aux;
  char linha[128];
  char *token;

  if (arq == NULL) {
    printf("Erro ao abrir o arquivo %s\n", fileName);
    aux.tam = 0;
    return aux;
  }

  fgets(linha, sizeof(linha), arq); // Pula o cabeçalho
  aux.tam = 0;

  // Lê os itens do arquivo
  while (fgets(linha, sizeof(linha), arq)) {
    if (linha[strlen(linha) - 1] == '\n') {
      linha[strlen(linha) - 1] = '\0'; // Remove o caractere de nova linha
    }

    token = strtok(linha, ",");
    aux.vet[aux.tam].ID = atoi(token); // Lê o ID

    token = strtok(NULL, ",");
    strcpy(aux.vet[aux.tam].nome, token); // Lê o Nome

    token = strtok(NULL, ",");
    strcpy(aux.vet[aux.tam].categoria, token); // Lê a Categoria

    token = strtok(NULL, ",");
    strcpy(aux.vet[aux.tam].marca, token); // Lê a Marca

    token = strtok(NULL, ",");
    aux.vet[aux.tam].preco = atof(token); // Lê o Preço

    token = strtok(NULL, ",");
    aux.vet[aux.tam].garantia = atoi(token); // Lê a Garantia

    token = strtok(NULL, ",");
    aux.vet[aux.tam].qtd_estoque = atoi(token); // Lê a Quantidade em Estoque

    aux.tam++; // Incrementa a quantidade de itens
  }

  fclose(arq);
  return aux;
}

// Função para gerar o relatório
void fazRelatorio(t_tabela tab) {
  printf("RELATÓRIO TOTAL EM ESTOQUE POR MARCA (R$ E QUANT. TOTAIS)\n\n");

  // Para armazenar o total por marca
  typedef struct {
    char marca[30];
    float valor_total;
    int qtd_total;
  } t_relatorio;

  t_relatorio relatorio[100]; // Para até 100 marcas diferentes
  int num_marcas = 0;

  // Inicializa o relatório
  for (int i = 0; i < 100; i++) {
    relatorio[i].valor_total = 0;
    relatorio[i].qtd_total = 0;
  }

  // Preenche o relatório com base nos dados da tabela
  for (int i = 0; i < tab.tam; i++) {
    int marca_encontrada = 0;

    // Verifica se a marca já está no relatório
    int j = 0;
    while (j < num_marcas && !marca_encontrada) {
      if (strcmp(relatorio[j].marca, tab.vet[i].marca) == 0) {
        // Atualiza os valores de quantidade e valor total
        relatorio[j].valor_total += tab.vet[i].preco * tab.vet[i].qtd_estoque;
        relatorio[j].qtd_total += tab.vet[i].qtd_estoque;
        marca_encontrada = 1;
      }
      j++;
    }

    // Se a marca não foi encontrada, adiciona uma nova entrada
    if (!marca_encontrada) {
      strcpy(relatorio[num_marcas].marca, tab.vet[i].marca);
      relatorio[num_marcas].valor_total =
          tab.vet[i].preco * tab.vet[i].qtd_estoque;
      relatorio[num_marcas].qtd_total = tab.vet[i].qtd_estoque;
      num_marcas++;
    }
  }

  // Ordena as marcas em ordem alfabética
  for (int i = 0; i < num_marcas - 1; i++) {
    for (int j = i + 1; j < num_marcas; j++) {
      if (strcmp(relatorio[i].marca, relatorio[j].marca) > 0) {
        t_relatorio temp = relatorio[i];
        relatorio[i] = relatorio[j];
        relatorio[j] = temp;
      }
    }
  }

  // Exibe o relatório com a marca, valor total e quantidade total
  for (int i = 0; i < num_marcas; i++) {
    printf("%s  %.2f  %d itens\n", relatorio[i].marca, relatorio[i].valor_total,
           relatorio[i].qtd_total);
  }

  printf("\n\n");

  // Encontrar o maior e menor total em estoque
  float maiorTotal = 0, menorTotal = 99999999;
  char maiorMarca[30], menorMarca[30];

  for (int i = 0; i < num_marcas; i++) {
    if (relatorio[i].valor_total > maiorTotal) {
      maiorTotal = relatorio[i].valor_total;
      strcpy(maiorMarca, relatorio[i].marca);
    }
    if (relatorio[i].valor_total < menorTotal) {
      menorTotal = relatorio[i].valor_total;
      strcpy(menorMarca, relatorio[i].marca);
    }
  }

  // Exibir o maior e menor total
  printf("MAIOR TOTAL EM ESTOQUE: %s, %.2f\n", maiorMarca, maiorTotal);
  printf("MENOR TOTAL EM ESTOQUE: %s, %.2f\n", menorMarca, menorTotal);
}

int main() {
  // Carregar a tabela do arquivo
  t_tabela tabela = LoadTab("tabti.txt");

  // Gerar o relatório
  fazRelatorio(tabela);

  return 0;
}