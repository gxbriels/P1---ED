#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char placa[50];
  char marca[50];
  char modelo[50];
  char cpfprop[50];
} t_item_v;

typedef struct {
  char cpf[50];
  char nome[50];
  char email[50];
  char celular[50];
} t_item_p;

typedef struct {
  char placaM[50];
  char dataM[50];
  float preco;
} t_item_m;

typedef struct {
  t_item_v vet[100];
  int tam;
} t_tab_v;

typedef struct {
  t_item_p vet[100];
  int tam;
} t_tab_p;

typedef struct {
  t_item_m vet[100];
  int tam;
} t_tab_m;

typedef struct {
  char pessoa[50];
  float valorTotal;
  int qtdMultas;
} t_ficha;

// Função para carregar a tabela de veículos
t_tab_v LoadTabV(char fileName[]) {
  FILE *arq = fopen(fileName, "rt");
  t_tab_v aux;
  char linha[64];
  char *token;

  fgets(linha, 63, arq); // Pula o cabeçalho

  aux.tam = 0;

  while (fgets(linha, 63, arq) != NULL) {
      size_t len = strlen(linha);
      if (len > 0 && (linha[len - 1] == '\n' || linha[len - 1] == '\r')) {
          linha[len - 1] = '\0';
      }

      if (strlen(linha) > 0) {
          token = strtok(linha, ",");
          if (token != NULL) strcpy(aux.vet[aux.tam].placa, token);

          token = strtok(NULL, ",");
          if (token != NULL) strcpy(aux.vet[aux.tam].marca, token);

          token = strtok(NULL, ",");
          if (token != NULL) strcpy(aux.vet[aux.tam].modelo, token);

          token = strtok(NULL, ",");
          if (token != NULL) strcpy(aux.vet[aux.tam].cpfprop, token);

          aux.tam++;
      }
  }

  fclose(arq);
  return aux;
}

// Função para carregar a tabela de proprietários
t_tab_p LoadTabP(char fileName[]) {
  FILE *arq = fopen(fileName, "rt");
  t_tab_p aux;
  char linha[90];
  char *token;

  fgets(linha, 80, arq); // Pula o cabeçalho

  aux.tam = 0;

  while (fgets(linha, 80, arq) != NULL) {
      size_t len = strlen(linha);
      if (len > 0 && (linha[len - 1] == '\n' || linha[len - 1] == '\r')) {
          linha[len - 1] = '\0';
      }

      if (strlen(linha) > 0) {
          token = strtok(linha, ",");
          if (token != NULL) strcpy(aux.vet[aux.tam].cpf, token);

          token = strtok(NULL, ",");
          if (token != NULL) strcpy(aux.vet[aux.tam].nome, token);

          token = strtok(NULL, ",");
          if (token != NULL) strcpy(aux.vet[aux.tam].email, token);

          token = strtok(NULL, ",");
          if (token != NULL) strcpy(aux.vet[aux.tam].celular, token);

          aux.tam++;
      }
  }

  fclose(arq);
  return aux;
}

// Função para carregar a tabela de multas
t_tab_m LoadTabM(char fileName[]) {
  FILE *arq = fopen(fileName, "rt");
  t_tab_m aux;
  char linha[90];
  char *token;

  aux.tam = 0;

  while (fgets(linha, 80, arq) != NULL) {
      size_t len = strlen(linha);
      if (len > 0 && (linha[len - 1] == '\n' || linha[len - 1] == '\r')) {
          linha[len - 1] = '\0';
      }

      if (strlen(linha) > 0) {
          token = strtok(linha, ",");
          if (token != NULL) strcpy(aux.vet[aux.tam].placaM, token);

          token = strtok(NULL, ",");
          if (token != NULL) strcpy(aux.vet[aux.tam].dataM, token);

          token = strtok(NULL, ",");
          if (token != NULL) aux.vet[aux.tam].preco = atof(token);

          aux.tam++;
      }
  }

  fclose(arq);
  return aux;
}

void fazRelatorio(t_tab_v tv, t_tab_p tp, t_tab_m tm) {

  t_ficha tab_infratores[100];

  for (int i = 0; i < tp.tam; i++) {
      strcpy(tab_infratores[i].pessoa, tp.vet[i].nome);
      tab_infratores[i].valorTotal = 0;
      tab_infratores[i].qtdMultas = 0;
  }

  for (int i = 0; i < tm.tam; i++) {
      for (int j = 0; j < tv.tam; j++) {
          if (strcmp(tm.vet[i].placaM, tv.vet[j].placa) == 0) {
              for (int k = 0; k < tp.tam; k++) {
                  if (strcmp(tv.vet[j].cpfprop, tp.vet[k].cpf) == 0) {
                      tab_infratores[k].valorTotal += tm.vet[i].preco;
                      tab_infratores[k].qtdMultas++;
                      j = tv.tam;
                      k = tp.tam;
                  }
              }
          }
      }
  }

  printf("RELATÓRIO VALOR TOTAL (R$) E QUANTIDADE DE MULTAS POR PROPRIETÁRIO\n\n");

  for (int i = 0; i < tp.tam; i++) {
      if (tab_infratores[i].qtdMultas > 0) {
          printf("%s    R$ %.2f    %d multas\n ", tab_infratores[i].pessoa, tab_infratores[i].valorTotal, tab_infratores[i].qtdMultas);
      }
  }

  printf("\n\n");

  float maiorTotal = 0, menorTotal = 9999999;
  int maiorInfrator = -1, menorInfrator = -1;

  for (int i = 0; i < tp.tam; i++) {
      if (tab_infratores[i].qtdMultas > 0) {
          if (tab_infratores[i].valorTotal > maiorTotal) {
              maiorTotal = tab_infratores[i].valorTotal;
              maiorInfrator = i;
          }
          if (tab_infratores[i].valorTotal < menorTotal) {
              menorTotal = tab_infratores[i].valorTotal;
              menorInfrator = i;
          }
      }
  }

  if (maiorInfrator != -1) {
      printf("MAIOR INFRATOR: %s, R$ %.2f, %d multas\n",
              tab_infratores[maiorInfrator].pessoa, maiorTotal,
              tab_infratores[maiorInfrator].qtdMultas);
  }

  if (menorInfrator != -1) {
      printf("MENOR INFRATOR: %s, R$ %.2f, %d multas\n",
              tab_infratores[menorInfrator].pessoa, menorTotal,
              tab_infratores[menorInfrator].qtdMultas);
  }
}

int main() {
  t_tab_v veiculos = LoadTabV("tabveiculos.txt");
  t_tab_p proprietarios = LoadTabP("tabproprietarios.txt");
  t_tab_m multas = LoadTabM("tabmultas.txt");

  fazRelatorio(veiculos, proprietarios, multas);

  return 0;
}
