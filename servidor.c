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
#define SIZE 500


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

void escreverDados(Abstrata *A) {
  FILE* fout = fopen("data.dat", "w");

  if (fout == NULL) {
    fprintf(stderr, "\nError opend file\n");
    exit (1);
  }

  fwrite(&A, sizeof(Abstrata), 1, fout);
  fclose(fout);

}

Abstrata* carregaDados() {
  Abstrata *newAbstrata; //= (Abstrata*)malloc(sizeof(Abstrata));

  FILE* fin = fopen("data.dat", "r");

  if (fin == NULL) {
    fprintf(stderr, "\nError opend file\n");
    exit (1);
  }

  while(fread(&newAbstrata, sizeof(Abstrata), 1, fin))
    fread(&newAbstrata, sizeof(Abstrata), 1, fin);

  fclose(fin);

  return newAbstrata;
}

int countMsg(Abstrata *A) {
  int count = 0;

  while(A != NULL) {
    count++;
    A = A->nseg;
  }
  return count;
}


int findMsg(Abstrata *A, int ID) {
  Abstrata *head = A;

  while(A != NULL) {
    if(A->msgId == ID)
      return 1;
    A = A->nseg;
  }
  A = head;

  return 0;
}


int main () {
  mknod(FIFO1, S_IFIFO | PERMS, 0);
  mknod(FIFO2, S_IFIFO | PERMS, 0);
  float readfd, writefd;
  Abstrata *list = NULL, *head = NULL;
  int select, x, ID, msgid, count = 0, flag;
  char content[SIZE], msgcnt[SIZE];

  list = carregaDados();

  while(1) {
    readfd = open(FIFO1, 0);
    read(readfd, &select, sizeof(int));
    read(readfd, &ID, sizeof(int));
    read(readfd, &x, sizeof(int));
    read(readfd, &content, sizeof(content));

    switch (select){
      case 1 :
        list = insertNode(list, createNode(ID, content));
        //count++;
      break;
      case 2 :
        if(list == NULL) {
          flag = -1;
          printf("\nNo messages\n");

          writefd = open(FIFO2, 1);
          write (writefd, &msgid, sizeof(int));
          write (writefd, &msgcnt, sizeof(msgcnt));
          write (writefd, &flag, sizeof(int));
        }
        else {
          count = countMsg(list);
          flag = count;
          head = list;

          while(list != NULL) {
            msgid = list->msgId;
            printf("%d\n", list->msgId);
            strcpy(msgcnt, list->content);
            writefd = open(FIFO2, 1);
            write (writefd, &msgid, sizeof(int));
            write (writefd, &msgcnt, sizeof(msgcnt));
            write (writefd, &flag, sizeof(int));
            printf("flag %d\n", flag);
            flag--;

            list = list->nseg;
          }
          list = head;
          listNodes(list);
        }
      break;
      case 3 :
        flag = 0;

        listNodes(list);
        if(findMsg(list, x) == 1) {
          flag = 1;
          list = removeNodeId(list, x);
          //if(count > 0)
            //count--;
        }

        writefd = open(FIFO2, 1);
        write (writefd, &msgid, sizeof(int));
        write (writefd, &msgcnt, sizeof(msgcnt));
        write (writefd, &flag, sizeof(int));

      break;
      case 0:
        escreverDados(list);
        free(list);
        exit(1);
      break;
    }
  }
  return 0;
}
