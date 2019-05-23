#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#define SR sizeof(float)
#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"
#define PERMS 0666
#define SIZE 200


//typedef's
typedef struct ABSTRATA {  //struc para guardar as mensagens com os respetivos IDs
  int msgId;
  char content[SIZE];
  struct ABSTRATA *nseg;
}Abstrata;


Abstrata* insertNode(Abstrata *A, Abstrata *nv) {      //insere uma nova mensagem na struct
                                                       //estilo insertFirst para não ter de correr as listas para descobrir o ultimo elemento
	if(A == NULL)                                        //mais rápido do que a altenativa insertLast
		return nv;

  nv->nseg = A;
  return nv;
}

Abstrata* removeNodeId(Abstrata *A, int id) {  //remove por id e devolve cabeça da lista atualizada
  Abstrata *head = A, *prev = A;

  while(A != NULL) {
    if(A->msgId == id) {
      if(A == prev) {
        A = A->nseg;
        prev->nseg = NULL;
        return A;
      }
      prev->nseg = A->nseg;
      A->nseg = NULL;
      return head;
    }
    prev = A;
    A = A->nseg;
  }
  return head;
}

Abstrata* createNode(int ID, char content[]) {  //cria uma nova mensagem

  Abstrata *newAbstrata = (Abstrata*)malloc(sizeof(Abstrata));
  newAbstrata->msgId = ID;
  strcpy(newAbstrata->content, content);

  return newAbstrata;
}

void listNodes(Abstrata  *A) {  //lista todas as mensagens
  while(A != NULL) {
    printf("\nmsgId: %d\n", A->msgId);
    printf("content: \n%s\n\n", A->content);

    A = A->nseg;
  }
}


int main () {
  mknod(FIFO1, S_IFIFO | PERMS, 0);
  mknod(FIFO2, S_IFIFO | PERMS, 0);
  float readfd, writefd;
  int select, x, ID;
  char content[SIZE];

  Abstrata *list = NULL;

  while(1) {
    readfd = open(FIFO1, 0);
    read(readfd, &select, sizeof(int));
    read(readfd, &ID, sizeof(int));
    read(readfd, &x, sizeof(int));
    read(readfd, &content, sizeof(content));

    switch (select){
      case 1 :
        list = insertNode(list, createNode(ID, content));
      break;
      case 2 :
        if(list == NULL)
          printf("\nNo messages\n");

        else {
          printf("%s\n", content);
          listNodes(list);
          writefd = open(FIFO2, 1);
          write (writefd, &list, sizeof(Abstrata*));
        }
      break;
      case 3 :
        list = removeNodeId(list, x);
      break;
    }

    //writefd = open(FIFO2, 1);
    //write (writefd, &res, SR);
  }
  return 0;
}
