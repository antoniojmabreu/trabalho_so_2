#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
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

typedef struct SELECT_T {
  int select;
}select_t;

void printMenu() {
  printf("\n\n1 -> New message\n2 -> Show all messages\n3 -> Remove message\n0 -> exit\n\n->");
}


//void * thread(void *args) {
int main() {

  mknod(FIFO1, S_IFIFO | PERMS, 0);
  mknod(FIFO2, S_IFIFO | PERMS, 0);
  float readfd, writefd;
  int ID = 0, x, msgid, flag, select;
  char msgcnt[SIZE], content[SIZE];
  //select_t * ptr = (select_t *)args;
  printf("check 2\n" );
  printMenu();

  while(1) {
    scanf("%d", &select);
    switch (select) {
      case 1:

        //inserir nova mensagem na lista
        ID++;
        getchar();
        printf("Type mesage content:\n");
        //fflush(stdin);
        fgets(content, SIZE, stdin);
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

        while (1) {
          readfd = open(FIFO2, 0);
          read(readfd, &msgid, sizeof(int));
          read(readfd, &msgcnt, sizeof(msgcnt));
          read(readfd, &flag, sizeof(int));

          if(flag == -1) {
            printf("\nNo messages\n");
            break;
          }

          printf("msgid %d\n", msgid);
          printf("msgcnt %s\n\n", msgcnt);

          if(flag == 1)
            break;
        }

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

        readfd = open(FIFO2, 0);
        read(readfd, &msgid, sizeof(int));
        read(readfd, &msgcnt, sizeof(msgcnt));
        read(readfd, &flag, sizeof(int));

        if(flag == 0)
          printf("msgId = '%d' não existe\n", x);

        printMenu();
      break;

      case 0:
        //sair
        writefd = open(FIFO1, 1);
        write(writefd, &select, sizeof(int));
        write(writefd, &ID, sizeof(int));
        write(writefd, &x, sizeof(int));
        write(writefd, &content, sizeof(content));

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
/*
int main() {
  int select = 0;
  pthread_t th;

  while (1) {
    printMenu();
    scanf("%d", &select);
    select_t * ptr = (select_t *)malloc( sizeof(select_t));
    ptr->select = select;
    pthread_create(&th, NULL, thread, (void *)ptr);
    pthread_join(th, NULL);
    printf("check 1\n" );
  }
}
*/
