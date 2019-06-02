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
#include "lista.c"

int main () {
  mknod(FIFO1, S_IFIFO | PERMS, 0);
  mknod(FIFO2, S_IFIFO | PERMS, 0);
  float readfd, writefd;
  int select, x, ID, msgid, count = 0, flag;
  char content[SIZE], msgcnt[SIZE];

  Abstrata *list = NULL, *head = NULL;

  while(1) {
    readfd = open(FIFO1, 0);
    read(readfd, &select, sizeof(int));
    read(readfd, &ID, sizeof(int));
    read(readfd, &x, sizeof(int));
    read(readfd, &content, sizeof(content));

    switch (select){
      case 1 :
        list = insertNode(list, createNodeServer(ID, content));
        count++;
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
        list = removeNodeId(list, x);
        listNodes(list);
        if(count > 0)
          count--;
      break;
    }
  }
  return 0;
}
