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
  int filaId;
  char content[SIZE];
  struct ABSTRATA *nseg;
}Abstrata;

typedef struct SELECT_T {
  int select;
}select_t;

int carregaId() {
  FILE *fin = fopen("id.txt", "r");
  char a[SIZE];

  if (fin == NULL) {
    fprintf(stderr, "\nError opend file\n");
    exit (1);
  }

  fgets(a, SIZE, fin);

  return atoi(a);
}

void guardaId(int ID) {
  FILE *fout = fopen("id.txt", "w");

  if (fout == NULL) {
    fprintf(stderr, "\nError opend file\n");
    exit (1);
  }

  fprintf(fout,"%d\n", ID);
}

void printMenu() {
  printf("\n\n1 -> Nova mensagem\n2 -> Mostrar todas as mensagens\n3 -> Remover mensagem\n4 -> Ficheiros\n5 -> Mostrar fila\n6 -> Remover fila\n0 -> exit\n\n->");
}


//void * thread(void *args) {
int main() {

  mknod(FIFO1, S_IFIFO | PERMS, 0);
  mknod(FIFO2, S_IFIFO | PERMS, 0);
  float readfd, writefd;
  int ID = 0, x, msgid, flag, select,select2, flagfiles, fila, filaid,flagpass;
  char msgcnt[SIZE], content[SIZE], fich[SIZE], pass[20];
  FILE *fp;
  char c;

  ID = carregaId();

  printMenu();

  while(1) {
    scanf("%d", &select);
    switch (select) {
      case 1:
        //inserir nova mensagem na lista
        ID++;
        printf("Insira a fila:\n");
        scanf("%d", &fila);
        getchar();
        printf("Insira o conteudo:\n");
        fgets(content, SIZE, stdin);
        printf("fila %d\n", fila);
        printf("%s\n", content);

        writefd = open(FIFO1, 1);
        write(writefd, &select, sizeof(int));
        write(writefd, &ID, sizeof(int));
        write(writefd, &x, sizeof(int));
        write(writefd, &content, sizeof(content));
        write(writefd,&fich,sizeof(fich));
        write(writefd,&pass,sizeof(pass));
        write(writefd,&fila,sizeof(fila));


        printMenu();
      break;

      case 2:
        //listar todas as mensagens
        writefd = open(FIFO1, 1);
        write(writefd, &select, sizeof(int));
        write(writefd, &ID, sizeof(int));
        write(writefd, &x, sizeof(int));
        write(writefd, &content, sizeof(content));
        write(writefd,&fich,sizeof(fich));
        write(writefd,&pass,sizeof(pass));
        write(writefd,&fila,sizeof(fila));

        while (1) {
          readfd = open(FIFO2, 0);
          read(readfd, &filaid, sizeof(int));
          read(readfd, &msgid, sizeof(int));
          read(readfd, &msgcnt, sizeof(msgcnt));
          read(readfd, &flag, sizeof(int));
          read(readfd, &flagfiles, sizeof(int));
          read(readfd, &flagpass, sizeof(int));

          if(flag == -1) {
            printf("\nSem mensagens\n");
            break;
          }

          printf("filaid %d\n", filaid);
          printf("msgid %d\n", msgid);
          printf("msgcnt %s\n\n", msgcnt);

          if(flag == 1)
            break;
        }

        printMenu();
      break;

      case 3:
        //remove mensagem por id
        printf("Insira o id a remover\n");
        scanf("%d", &x);

        writefd = open(FIFO1, 1);
        write(writefd, &select, sizeof(int));
        write(writefd, &ID, sizeof(int));
        write(writefd, &x, sizeof(int));
        write(writefd, &content, sizeof(content));
        write(writefd,&fich,sizeof(fich));
        write(writefd,&pass,sizeof(pass));
        write(writefd,&fila,sizeof(fila));

        readfd = open(FIFO2, 0);
        read(readfd, &filaid, sizeof(int));
        read(readfd, &msgid, sizeof(int));
        read(readfd, &msgcnt, sizeof(msgcnt));
        read(readfd, &flag, sizeof(int));
        read(readfd, &flagfiles, sizeof(int));
        read(readfd, &flagpass, sizeof(int));

        if(flag == 0)
          printf("msgId = '%d' não existe\n", x);

        printMenu();
      break;
      case 4:
      printf("1 -> Ficheiros normais\n2 -> Ficheiros secretos\n");
      scanf("%d", &select2);
      switch (select2) {
      case 1:
        printf("Insira o nome do ficheiro a abir:\n");
        scanf("%s",fich);

        writefd = open(FIFO1, 1);
        write(writefd, &select, sizeof(int));
        write(writefd, &ID, sizeof(int));
        write(writefd, &x, sizeof(int));
        write(writefd, &content, sizeof(content));
        write(writefd,&fich,sizeof(fich));
        write(writefd,&pass,sizeof(pass));
        write(writefd,&fila,sizeof(fila));

        while(1){
          readfd = open(FIFO2, 0);
          read(readfd, &filaid, sizeof(int));
          read(readfd, &msgid, sizeof(int));
          read(readfd, &msgcnt, sizeof(msgcnt));
          read(readfd, &flag, sizeof(int));
          read(readfd, &flagfiles, sizeof(int));
          read(readfd, &flagpass, sizeof(int));

          if(flagfiles== -1){ printf("No such file or not allowed\n"); }
          if(flagfiles == 1){
            //abrir Ficheiro
            printf("SUCESS\n");
            fp = fopen(fich,"r");
            c = fgetc(fp);

            while(c != EOF){
              printf("%c",c);
              c = fgetc(fp);
            }
          }
          break;
          }

          break;

          case 2:
            select = 5;

            printf("Insira o nome do ficheiro a abir:\n");
            scanf("%s",fich);
            char pass[20];
            printf("Insira a password\n");
            scanf("%s",pass);

            writefd = open(FIFO1, 1);
            write(writefd, &select, sizeof(int));
            write(writefd, &ID, sizeof(int));
            write(writefd, &x, sizeof(int));
            write(writefd, &content, sizeof(content));
            write(writefd,&fich,sizeof(fich));
            write(writefd,&pass,sizeof(pass));
            write(writefd,&fila,sizeof(fila));

            while(1){
              readfd = open(FIFO2, 0);
              read(readfd, &filaid, sizeof(int));
              read(readfd, &msgid, sizeof(int));
              read(readfd, &msgcnt, sizeof(msgcnt));
              read(readfd, &flag, sizeof(int));
              read(readfd, &flagfiles, sizeof(int));
              read(readfd, &flagpass, sizeof(int));

              if(flagpass== -1){ printf("Wrong password\n"); }
              if(flagfiles == 1){
                //abrir Ficheiro
                printf("SUCESS\n");
                fp = fopen(fich,"r");
                c = fgetc(fp);

                while(c != EOF){
                  printf("%c",c);
                  c = fgetc(fp);
                }
              }
              break;
              }
            }
    printMenu();
    break;
      case 5:
        select = 6;
        printf("Insira a fila a mostrar\n");
        scanf("%d", &x);

        //listar todas as mensagens
        writefd = open(FIFO1, 1);
        write(writefd, &select, sizeof(int));
        write(writefd, &ID, sizeof(int));
        write(writefd, &x, sizeof(int));
        write(writefd, &content, sizeof(content));
        write(writefd,&fich,sizeof(fich));
        write(writefd,&pass,sizeof(pass));
        write(writefd,&fila,sizeof(fila));

        while (1) {
          readfd = open(FIFO2, 0);
          read(readfd, &filaid, sizeof(int));
          read(readfd, &msgid, sizeof(int));
          read(readfd, &msgcnt, sizeof(msgcnt));
          read(readfd, &flag, sizeof(int));
          read(readfd, &flagfiles, sizeof(int));
          read(readfd, &flagpass, sizeof(int));

          if(flag == -1) {
            printf("\nFila não existe\n");
            break;
          }

          printf("filaid %d\n", filaid);
          printf("msgid %d\n", msgid);
          printf("msgcnt %s\n\n", msgcnt);

          if(flag == 1)
            break;
        }

        printMenu();
      break;
      case 6:
      select = 7;
        //remove mensagem por id
        printf("Insira a fila a remover\n");
        scanf("%d", &x);

        writefd = open(FIFO1, 1);
        write(writefd, &select, sizeof(int));
        write(writefd, &ID, sizeof(int));
        write(writefd, &x, sizeof(int));
        write(writefd, &content, sizeof(content));
        write(writefd,&fich,sizeof(fich));
        write(writefd,&pass,sizeof(pass));
        write(writefd,&fila,sizeof(fila));

        printf("DONE\n");

        printMenu();
      break;
      case 0:
        //sair
        writefd = open(FIFO1, 1);
        write(writefd, &select, sizeof(int));
        write(writefd, &ID, sizeof(int));
        write(writefd, &x, sizeof(int));
        write(writefd, &content, sizeof(content));
        write(writefd,&fich,sizeof(fich));
        write(writefd,&pass,sizeof(pass));
        write(writefd,&fila,sizeof(fila));

        guardaId(ID);
        exit(1);
      break;

      default:
        printf("\n !!! Seleção inválida !!!\n\n Selecione Opção -> ");
        continue;
      break;
    }
  }

  unlink(FIFO1);
  unlink(FIFO2);

  return 0;
}
