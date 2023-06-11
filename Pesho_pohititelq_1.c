#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MAP_HEIGHT 100
#define MAX_MAP_WIDTH 200

struct Block {
  int value; // value na node (0, 1, 2, 3, 4, 5)
  struct Block *next;
  int x;
  int y;

};
struct Links {
  struct Platform *neighbor;
  int distance; // distance
  int index;    // neighbour index
};
struct Platform {
  struct Block *block_head;  //  linked list ot blockchetata vuv edna platforma
  struct Links **link_array; // array s razstoqniqta mejdu platformite
  int num_blocks;            // number of blocks in the linked list
  int num_links;             // number of neighbour link_array
  bool is_visited;           // is_visited
};
struct Map {
  struct Platform *head_platform;   // pointer kum nachalnata platforma
  struct Platform **platform_array; // array ot platformi
  int platform_count;               // kolko sa platformite
};
struct Pesho {
  struct Block *position; // pointer kum positiona mu
  int jump;
} pesho;
struct Polizei {
  struct Block *position; // pointer kum positiona mu
  int jump;
} polizei;

char** readMapFromFile(int* Rows, int* Colls) {
  
  char fPath[100];
  
  // printf("Enter File Path: ");
  // fgets(fPath, sizeof(fPath), stdin);
  printf("\n");
  fPath[strcspn(fPath, "\n")] = '\0';
  
  FILE* file = fopen("pp.txt", "r");
  if (file == NULL) {
    printf("Failed to open file.\n");
    return NULL;
  }

  char line[MAX_MAP_WIDTH];
  fgets(line, sizeof(line), file);
  *Colls = strlen(line);
  rewind(file);


  *Rows = 0;
  while (fgets(line, sizeof(line), file) != NULL) {
    (*Rows)++;
  }

  char** map = malloc((*Rows) * sizeof(char*));

  if (map == NULL) {
        printf("Failed to allocate memory\n");
        return NULL;
  }

  
  for (int i = 0; i < *Rows; i++) {

    map[i] = malloc((*Colls + 1) * sizeof(char));
  }

    rewind(file);

    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        strcpy(map[i], line);
        i++;
    }

    fclose(file);

    return map;
}


void free_Memory(char** Map, int Rows) {
    for (int i = 0; i < Rows; i++) {
        free(Map[i]);
    }

  
    free(Map);
}

struct Block* Block_Create(){


  
}

bool Platform_Check(char **Map, int x, int y){
  return Map[x][y] == '#';
}

struct Platform* Platform_Create(char** Map, int x, int y){
  struct Platform *platform = malloc(1 * sizeof(struct Platform));
  struct Block *Head_block = malloc(1 * sizeof(struct Block));

  platform->block_head = Head_block;
  platform->is_visited = 0;
  platform->num_blocks = 1;
  platform->link_array = malloc(100 * sizeof(struct Links));
  Head_block->value = 0;
  Head_block->x = x;
  Head_block->y = y;
  Head_block->next = NULL;
  
  for(int i = 1; Platform_Check(Map, x + i, y); i++){
    Head_block->next = malloc(1 * sizeof(struct Block));
    Head_block = Head_block->next;
    Head_block->x = x + i;
    Head_block->y = y;
    Head_block->next = NULL;
    Head_block->value = i - 1;
    
  }
 
  return platform;
}



struct Map* Graph_create(char **Map, int Rows, int Colls){
  
  struct Map* Graph = malloc(1 * sizeof(struct Map));
  Graph->platform_count = 0;
  Graph->platform_array = malloc(100 * sizeof(struct Platform));
  
  struct Platform *Temp;
  
  for(int x = 0; x < Rows; x++){
    for(int y = 0; y < Colls; y++){
      if(Platform_Check(Map, x, y)) {
        Temp = Platform_Create(Map, x, y);
        if(Graph->platform_count == 0) Graph->head_platform = Temp;
        Graph->platform_array[Graph->platform_count] = Temp;
        Graph->platform_count++;
      }
      else {
        
      }
    }
    
  }
  
  return Graph;
}  

int main(void) {
  
  int Rows = 0;
  int Colls = 0;

  

  
  char** Map = readMapFromFile(&Rows, &Colls);
  if (Map == NULL) {
    printf("Failed to create map\n");
    return -1;
  }

  struct Map *Graph = Graph_create(Map, Rows, Colls);
  
  // free_Memory(map, rows);
  

  
  return 0;
}