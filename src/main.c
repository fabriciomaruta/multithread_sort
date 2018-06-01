/* Ordenacao em Multithread
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define TRABALHADORES 2
#define MAX 15000


pthread_mutex_t trava,trava2;

typedef struct {
  int v[MAX];
  int inicio;
  int fim;

} argumentos;

void merge(int *vetor, int comeco, int meio, int fim);
void mergeSort(int *vetor, int comeco, int fim);

void *ordena(void *args){
  pthread_mutex_lock(&trava);
  argumentos *pass = (argumentos*) args;
  mergeSort((pass->v), pass->inicio, pass->fim);
  pthread_mutex_unlock(&trava);
  return NULL;
}

int main() {
  int i = 0, contador = 0, j;
  argumentos vetor;
  pthread_t thread[2];

  while(scanf("%d", &(vetor.v[i]))!= EOF){
    contador++;   // Saber o tamanho do vetor
    i++;
  }
  int fatordiv = contador/2;
  vetor.inicio = 0;
  vetor.fim = fatordiv;
  pthread_mutex_lock(&trava2);
  pthread_create(&(thread[0]), NULL, ordena, (void *)&vetor);
  pthread_mutex_unlock(&trava2);
  sleep(1);
  vetor.inicio = fatordiv + 1;
  vetor.fim = contador-1;
  pthread_create(&(thread[1]), NULL, ordena, (void *)&vetor);



  for(int i = 0; i<TRABALHADORES;i++){
    pthread_join(thread[i], NULL);
  }
  merge(vetor.v, 0, fatordiv,contador-1);
  for(int i = 0; i < contador; i++){
    printf("%d ", vetor.v[i]);
  }
  printf("\n");

}


void merge(int *vetor, int comeco, int meio, int fim) {
      int com1 = comeco, com2 = meio+1, comAux = 0, tam = fim-comeco+1;
      int *vetAux;
      vetAux = (int*)malloc(tam * sizeof(int));

      while(com1 <= meio && com2 <= fim){
          if(vetor[com1] < vetor[com2]) {
              vetAux[comAux] = vetor[com1];
              com1++;
         } else {
             vetAux[comAux] = vetor[com2];
             com2++;
       }
         comAux++;
     }

     while(com1 <= meio){  //Caso ainda haja elementos na primeira metade
         vetAux[comAux] = vetor[com1];
         comAux++;
         com1++;
     }

     while(com2 <= fim) {   //Caso ainda haja elementos na segunda metade
         vetAux[comAux] = vetor[com2];
         comAux++;
         com2++;
     }

     for(comAux = comeco; comAux <= fim; comAux++){    //Move os elementos de volta para o vetor original
         vetor[comAux] = vetAux[comAux-comeco];
     }

     free(vetAux);
 }
 void mergeSort(int *vetor, int comeco, int fim){
     if (comeco < fim) {
         int meio = (fim+comeco)/2;

         mergeSort(vetor, comeco, meio);
         mergeSort(vetor, meio+1, fim);
         merge(vetor, comeco, meio, fim);
     }
 }
