#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define SIZE 40

//typedef's
typedef struct ABSTRATA {  //struc para guardar as mensagens com os respetivos IDs
  int msgId;
  char content[SIZE];
  struct ABSTRATA *nseg;
}Abstrata;


Abstrata* insertNode(Abstrata *A, Abstrata *nv) {     //insere uma nova mensagem na struct
                                                      //estilo insertFirst para não ter de correr as listas para descobrir o ultimo elemento
                                                      //mais rápido do que a altenativa insertLast
	if(A == NULL)
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

Abstrata* createNode(int ID) {  //cria uma nova mensagem
  char text[SIZE];

  printf("Insert content\n");
  scanf("%s", text);

  Abstrata *newAbstrata = (Abstrata*)malloc(sizeof(Abstrata));
  newAbstrata->msgId = ID;
  strcpy(newAbstrata->content, text);

  return newAbstrata;
}

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

int main() {  //main
  Abstrata *list = NULL;
  int ID = 0, select, x;

  printMenu();

  while(1) {
    scanf("%d", &select);
    switch (select) {
      case 1:
        //inserir nova mensagem na lista
        ID++;
        Abstrata *new = createNode(ID);
        list = insertNode(list, new);

        printMenu();
      break;

      case 2:
        //listar todas as mensagens
        if(list == NULL)
          printf("\nNo messages\n");

        else
          listNodes(list);

        printMenu();
      break;

      case 3:
        //remove mensagem por id
        printf("Insert id to remove\n");
        scanf("%d", &x);

        list = removeNodeId(list, x);

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
}
