#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex, writeblock;
int data = 0, rcount = 0;
int nr = 0, nw = 0, i = 0;
int r, w;
pthread_t rtid[10000], wtid[10000];

void *reader(void *arg)
{
  int f;
  f = ((int)arg);
  sem_wait(&mutex);
  rcount = rcount + 1;
  if (rcount == 1)
    sem_wait(&writeblock);
  sem_post(&mutex);
  printf("Data read by the reader%d is %d\n", f, data);
  sleep(1);
  sem_wait(&mutex);
  rcount = rcount - 1;
  if (rcount == 0)
    sem_post(&writeblock);
  sem_post(&mutex);
}

void *writer(void *arg)
{
  int f;
  f = ((int)arg);
  sem_wait(&writeblock);
  data++;
  printf("Data writen by the writer%d is %d\n", f, data);
  sleep(1);
  sem_post(&writeblock);
}

void createReader(int n)
{
  for (i = nr; i < nr + n; i++)
    pthread_create(&rtid[i], NULL, reader, (void *)i);

  nr += n;
}

void createWriter(int n)
{
  for (i = nw; i < nw + n; i++)
    pthread_create(&wtid[i], NULL, writer, (void *)i);
  nw += n;
}

int main()
{
  int i, b;
  sem_init(&mutex, 0, 1);
  sem_init(&writeblock, 0, 1);

  while (1)
  {
    printf("enter number of readers and writers or -1 to exit: \n");
    scanf("%d", &r);
    if (r == -1)
    {
      break;
    }
    scanf("%d", &w);
    createWriter(w);
    createReader(r);
  }
  return 0;
}