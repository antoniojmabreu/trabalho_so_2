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
  int filaId;
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

void removeFila(Abstrata **root, int value){
  if(*root != NULL) {
    Abstrata *temp = *root, *helper = *root;

      while(temp){
        if(temp->filaId == value){
          if(temp == *root){
            helper = temp;
            *root = (*root)->nseg;
            temp = *root;
            helper->nseg = NULL;

            free(helper);
            helper = NULL;
        }
        else{
          helper->nseg = temp->nseg;
          temp->nseg = NULL;
          free(temp);
          temp = helper->nseg;
        }
      }else{
      helper =temp;
      temp = temp->nseg;
    }
   }
  }
}

Abstrata* createNode(int ID, char content[], int fila) {  //cria uma nova mensagem

  Abstrata *newAbstrata = (Abstrata*)malloc(sizeof(Abstrata));
  newAbstrata->msgId = ID;
  newAbstrata->filaId = fila;
  strcpy(newAbstrata->content, content);

  return newAbstrata;
}

void listNodes(Abstrata  *A) {  //lista todas as mensagens
  while(A != NULL) {
    printf("fila %d\n", A->filaId);
    printf("msgId: %d\n", A->msgId);
    printf("content: %s\n\n", A->content);

    A = A->nseg;
  }
}

Abstrata* escreverDados(Abstrata *A) {
  remove("data.txt");
  FILE* fout = fopen("data.txt", "w");
  Abstrata *head = A;

  if (fout == NULL) {
    fprintf(stderr, "\nError opend file\n");
    exit (1);
  }

  while(A != NULL) {
    fprintf(fout,"%d\n", A->filaId);
    fprintf(fout,"%d\n", A->msgId);
    fprintf(fout,"%s", A->content);
    A = A->nseg;
  }

  fclose(fout);

  return head;
}

void carregaDados(Abstrata **A) {
  char c[SIZE], a[SIZE], b[SIZE];
  FILE* fin = fopen("data.txt", "r");

  if (fin == NULL) {
    fprintf(stderr, "\nError opend file\n");
    exit (1);
  }

  while(!feof(fin)) {
    fgets(b, SIZE, fin);
    fgets(a, SIZE, fin);
    fgets(c, SIZE, fin);

    if(strcmp(c, "") != 0) *A = insertNode(*A, createNode(atoi(a), c, atoi(b)));
    //fgets(c, SIZE, fin);
  }

  fclose(fin);
}

int countMsg(Abstrata *A) {
  int count = 0;

  while(A != NULL) {
    count++;
    A = A->nseg;
  }
  return count;
}

int countFila(Abstrata *A, int ID) {
  int count = 0;

  while(A != NULL) {
    if(A->filaId == ID)
      count ++;;
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
int checkFilename(char fich[SIZE]){

    if(fich[0] == '.' && fich[1] == '.') {
      return 0;
    }
  return 1;
}

int main () {
  mknod(FIFO1, S_IFIFO | PERMS, 0);
  mknod(FIFO2, S_IFIFO | PERMS, 0);
  float readfd, writefd;
  Abstrata *list = NULL, *head = NULL;
  int select, x, ID, msgid, count = 0,check, flag,flagfiles,flagpass, filaid,fila;
  char content[SIZE], msgcnt[SIZE],fich[SIZE],pass[20];
  FILE *fp;
  char password[20] = "secret";

  carregaDados(&list);
  list = removeNodeId(list, 0);
  if(list != NULL)
    list = list->nseg;

  while(1) {
    readfd = open(FIFO1, 0);
    read(readfd, &select, sizeof(int));
    read(readfd, &ID, sizeof(int));
    read(readfd, &x, sizeof(int));
    read(readfd, &content, sizeof(content));
    read(readfd, &fich, sizeof(fich));
    read(readfd,&pass,sizeof(pass));
    read(readfd, &fila, sizeof(int));

    switch (select){
      case 1 :
        printf("fila %d\n", fila);
        printf("id %d\n", ID);
        printf("cont %s\n", content);
        list = insertNode(list, createNode(ID, content, fila));
        //count++;
      break;
      case 2 :
        if(list == NULL) {
          flag = -1;
          printf("\nNo messages\n");

          writefd = open(FIFO2, 1);
          write (writefd, &filaid, sizeof(int));
          write (writefd, &msgid, sizeof(int));
          write (writefd, &msgcnt, sizeof(msgcnt));
          write (writefd, &flag, sizeof(int));
          write (writefd, &flagfiles, sizeof(int));
          write (writefd, &flagpass, sizeof(int));
        }
        else {
          count = countMsg(list);
          flag = count;
          head = list;

          while(list != NULL) {
            filaid = list->filaId;
            msgid = list->msgId;
            strcpy(msgcnt, list->content);

            writefd = open(FIFO2, 1);
            write (writefd, &filaid, sizeof(int));
            write (writefd, &msgid, sizeof(int));
            write (writefd, &msgcnt, sizeof(msgcnt));
            write (writefd, &flag, sizeof(int));
            write (writefd, &flagfiles, sizeof(int));
            write (writefd, &flagpass, sizeof(int));
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
        }

        writefd = open(FIFO2, 1);
        write (writefd, &filaid, sizeof(int));
        write (writefd, &msgid, sizeof(int));
        write (writefd, &msgcnt, sizeof(msgcnt));
        write (writefd, &flag, sizeof(int));
        write (writefd, &flagfiles, sizeof(int));
        write (writefd, &flagpass, sizeof(int));

      break;
      case 4:
        printf("File requested: %s\n",fich);
         check = checkFilename(fich);
        if(check==0){

          printf("File Not allowed\n");
          flagfiles = -1;
          writefd = open(FIFO2, 1);
          write (writefd, &filaid, sizeof(int));
          write (writefd, &msgid, sizeof(int));
          write (writefd, &msgcnt, sizeof(msgcnt));
          write (writefd, &flag, sizeof(int));
          write (writefd, &flagfiles, sizeof(int));
          write (writefd, &flagpass, sizeof(int));
        }else{

        fp = fopen(fich,"r");
        if(fp!=NULL){
          flagfiles=1;
          writefd = open(FIFO2, 1);
          write (writefd, &filaid, sizeof(int));
          write (writefd, &msgid, sizeof(int));
          write (writefd, &msgcnt, sizeof(msgcnt));
          write (writefd, &flag, sizeof(int));
          write (writefd, &flagfiles, sizeof(int));
          write (writefd, &flagpass, sizeof(int));

        }else{
          flagfiles = -1;
          writefd = open(FIFO2, 1);
          write (writefd, &filaid, sizeof(int));
          write (writefd, &msgid, sizeof(int));
          write (writefd, &msgcnt, sizeof(msgcnt));
          write (writefd, &flag, sizeof(int));
          write (writefd, &flagfiles, sizeof(int));
          write (writefd, &flagpass, sizeof(int));

          printf("No such file \n");
        }

      }
      break;
      case 5:
        printf("File requested: %s\n",fich);
        printf("Password inserida: %s\n",pass);

         check = checkFilename(fich);
        if(check==0){
          printf("File Not allowed\n");
          flagfiles = -1;
          flagpass = -1;
          writefd = open(FIFO2, 1);
          write (writefd, &filaid, sizeof(int));
          write (writefd, &msgid, sizeof(int));
          write (writefd, &msgcnt, sizeof(msgcnt));
          write (writefd, &flag, sizeof(int));
          write (writefd, &flagfiles, sizeof(int));
          write (writefd, &flagpass, sizeof(int));
        }else{

          if(strcmp(pass,password)==0){
            fp = fopen(fich,"r");
            if(fp!=NULL){
              flagfiles=1;
              writefd = open(FIFO2, 1);
              write (writefd, &filaid, sizeof(int));
              write (writefd, &msgid, sizeof(int));
              write (writefd, &msgcnt, sizeof(msgcnt));
              write (writefd, &flag, sizeof(int));
              write (writefd, &flagfiles, sizeof(int));
              write (writefd, &flagpass, sizeof(int));
            }else{
              flagfiles = -1;
              writefd = open(FIFO2, 1);
              write (writefd, &filaid, sizeof(int));
              write (writefd, &msgid, sizeof(int));
              write (writefd, &msgcnt, sizeof(msgcnt));
              write (writefd, &flag, sizeof(int));
              write (writefd, &flagfiles, sizeof(int));
              write (writefd, &flagpass, sizeof(int));
              printf("No such file \n");
           }
          }else{
            printf("Wrong password: %s \n",pass);
            flagfiles = -1;
            flagpass = -1;
            writefd = open(FIFO2, 1);
            write (writefd, &filaid, sizeof(int));
            write (writefd, &msgid, sizeof(int));
            write (writefd, &msgcnt, sizeof(msgcnt));
            write (writefd, &flag, sizeof(int));
            write (writefd, &flagfiles, sizeof(int));
            write (writefd, &flagpass, sizeof(int));

          }
        }

      break;

      case 6:
        if(countFila(list, x) == 0) {
          flag = -1;
          printf("\nNo messages\n");

          writefd = open(FIFO2, 1);
          write (writefd, &filaid, sizeof(int));
          write (writefd, &msgid, sizeof(int));
          write (writefd, &msgcnt, sizeof(msgcnt));
          write (writefd, &flag, sizeof(int));
          write (writefd, &flagfiles, sizeof(int));
          write (writefd, &flagpass, sizeof(int));
        }
        else {
          count = countFila(list, x);
          flag = count;
          head = list;

          while(list != NULL) {
            if(list->filaId == x) {
              filaid = list->filaId;
              msgid = list->msgId;
              strcpy(msgcnt, list->content);

              writefd = open(FIFO2, 1);
              write (writefd, &filaid, sizeof(int));
              write (writefd, &msgid, sizeof(int));
              write (writefd, &msgcnt, sizeof(msgcnt));
              write (writefd, &flag, sizeof(int));
              write (writefd, &flagfiles, sizeof(int));
              write (writefd, &flagpass, sizeof(int));
              printf("flag %d\n", flag);
              flag--;
            }
            list = list->nseg;
          }
          list = head;
          listNodes(list);
        }
      break;
      case 7:
        removeFila(&list, x);
      break;
      case 0:
        fflush(stdin);
        list = escreverDados(list);
        free(list);
        exit(1);
      break;
    }
  }
  return 0;
}
