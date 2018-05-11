/* Gabriela Akemi Shima (RA 135819)
 * Lab 5 - Contador de Numeros Primos em Mutithread
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <pthread.h>

 #define MAX_WORKERS 4
 #define MAX_ELEMENTOS 100
 #define ATIVO 1
 #define INATIVO 0

 typedef struct {
   int N;
   int ID;
 } thread_args;

 pthread_mutex_t trava;
 int n_workers = 0;
 pthread_t workers[MAX_WORKERS];
 int worker_status[MAX_WORKERS];
 int qtde_primos = 0;
 int qtde_elementos = 0, vetor[MAX_ELEMENTOS];

int verificaPrimo(long unsigned int numero) {
   int divisoes = 0, num = numero;
   for(int i = 1; i <= num; ++i){
      if(numero%i == 0) divisoes++;
   }
   if(divisoes == 2) return 1;
   else return 0;
}

void* worker(void *arg) {
  thread_args *info = (thread_args *)arg;
  //printf("Fibo(%d)=%d\n", info->N, M);
  //printf(" Thread %d:Numero = %d, eh primo? = %d\n",info->ID, info->N, verificaPrimo(info->N));
  pthread_mutex_lock(&trava);
  qtde_primos += verificaPrimo(info->N);
  n_workers -= 1;
  worker_status[info->ID] = INATIVO;
  free(info);
  pthread_mutex_unlock(&trava);
  return NULL;
}

int main() {

  int numero_recebido;
  thread_args *send_args;
  int j = 0;

  do {
    scanf("%d", &numero_recebido);
    vetor[qtde_elementos] = numero_recebido;
    qtde_elementos++;
} while ( getchar() != '\n' && j < MAX_ELEMENTOS);

  //for(int i = 0; i < qtde_elementos; ++i) printf(" vetor[%d] = %d\n", i, vetor[i]);

  for(int i = 0; i <= qtde_elementos ;++i){
    if(n_workers < MAX_WORKERS){ //Caso o numero de threads ativas seja menor que o maximo
      pthread_mutex_lock(&trava);
      //printf("Iniciando nova thread\n");
      send_args = (thread_args*)malloc(sizeof(thread_args));
      send_args->N = vetor[i];
      /* Procura espaco para thread livre */
      j = 0;
      while (worker_status[j] == ATIVO) j++;
      send_args->ID = j;
      worker_status[j] = ATIVO;
      n_workers += 1;
      //printf("Threads ativas: %d de %d\n", n_workers, MAX_WORKERS);
      pthread_create(& (workers[j]), NULL, worker, (void*) send_args);
      pthread_mutex_unlock(&trava);
    }
    else {
      --i;
    }
  }

  /* Esperando threads */
  //printf("Terminando programa. Esperando threads terminarem...\n");
  for (int i=0; i<MAX_WORKERS; i++) {
    if (worker_status[i]==ATIVO) {
      pthread_join(workers[i], NULL);
    }
  }

  //printf("=============================\n");
  //printf("Quantidade de primos: %d\n", qtde_primos);
  printf("%d\n", qtde_primos);
  return 0;
}
