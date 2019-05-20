#include<stdio.h>
#include<stdlib.h>
#define SIZE 40

//typedef's
typedef struct ABSTRATA {  //pixeis pertencentes ao intervalo de cor num determinado frame
  int msgId;
  char[SIZE]
  struct ABSTRATA *nseg;
}Abstrata;

Abstrata* insertNode(Abstrata *A, Abstrata *nv) {  //estilo insertFirst para não ter de correr as listas para descobrir o ultimo elemento
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
