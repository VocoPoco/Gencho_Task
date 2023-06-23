#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.h"

struct Polizei polizei;
struct Pesho pesho;

int main(void) {

  system("cls");

  int Rows = 0;
  int Colls = 0;
  char filepath[100];
  fgets(filepath, sizeof(filepath), stdin);

  char** Map = readMapFromFile(filepath, &Rows, &Colls);
  if (Map == NULL) {
    printf("Failed to create map\n");
    return -1;
  }

  scanf("%d", &pesho.jump);
  scanf("%d", &polizei.jump);
  struct Map *Graph = Graph_create(Map, Rows, Colls);

  // real_life(Graph);

  return 0;
}
