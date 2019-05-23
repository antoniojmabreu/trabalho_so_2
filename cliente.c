#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
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

void listNodes(Abstrata  *A) {  //lista todas as mensagens
  while(A != NULL) {
    printf("\nmsgId: %d\n", A->msgId);
    printf("content: \n%s\n\n", A->content);

    A = A->nseg;
  }
}

void printMenu() {
  printf("\n\n1 -> Insert new\n2 -> List all\n3 -> Remove msg\n0 -> exit\n\n->");
}


int main () {
  mknod(FIFO1, S_IFIFO | PERMS, 0);
  mknod(FIFO2, S_IFIFO | PERMS, 0);
  float readfd, writefd;
  int ID = 0, select, x;
  char content[SIZE];
  Abstrata *list = NULL;

  printMenu();

  while(1) {
    scanf("%d", &select);
    switch (select) {
      case 1:
        //inserir nova mensagem na lista
        ID++;

        printf("Insert content\n");
        scanf("%s", content);
        printf("%s\n", content);

        writefd = open(FIFO1, 1);
        write(writefd, &select, sizeof(int));
        write(writefd, &ID, sizeof(int));
        write(writefd, &x, sizeof(int));
        write(writefd, &content, sizeof(content));

        printMenu();
      break;

      case 2:
        //listar todas as mensagens
        writefd = open(FIFO1, 1);
        write(writefd, &select, sizeof(int));
        write(writefd, &ID, sizeof(int));
        write(writefd, &x, sizeof(int));
        write(writefd, &content, sizeof(content));

        //readfd = open(FIFO2, 0);
        //read(readfd, &cont, SR);


          printf("else\n");
          readfd = open(FIFO2, 0);
          read(readfd, &list, sizeof(Abstrata*));

          listNodes(list);

        printMenu();
      break;

      case 3:
        //remove mensagem por id
        printf("Insert id to remove\n");
        scanf("%d", &x);

        writefd = open(FIFO1, 1);
        write(writefd, &select, sizeof(int));
        write(writefd, &ID, sizeof(int));
        write(writefd, &x, sizeof(int));
        write(writefd, &content, sizeof(content));

        printMenu();
      break;

      case 0:
        //sair
        exit(1);
      break;

      default:
        printf("\n !!! Seleção inválida !!!\n\n Selecione Opção -> ");
        continue;
      break;
    }
  }

  //readfd = open(FIFO2, 0);
  //read(readfd, &res, SR);

  unlink(FIFO1);
  unlink(FIFO2);
  //printf("Result %.3f\n",res);

  return 0;
}
