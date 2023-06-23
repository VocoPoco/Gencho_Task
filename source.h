#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Block {
  int value; // value na node (0, 1, 2, 3, 4, 5)
  struct Block *next; //next block in platform
  int x; //koordinat x na text mapa
  int y; //koordinat y na text mapa

};
struct Links {
  struct Platform *platform_node; //neighbor platform pointer
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
  int jump; //jump height na Pesho
} pesho;
struct Polizei {
  struct Block *position; // pointer kum positiona mu
  int jump; //jump height na policiqta
} polizei;



void update_visited_to_false(struct Map *map);

struct Platform *find_safe_platform(struct Map *map);

struct Platform *find_safe_platform_util(struct Map *map);

void position_characters(struct Map *map);

struct Platform** check_available_route(struct Platform *starting_platform, struct Platform *safe_platform);

void real_life(struct Map *map);

char** readMapFromFile(char filepath[100], int* Rows, int* Colls);

bool Platform_Check(char** Map, int x, int y, int Rows, int Colls);

struct Platform* Platform_Create(char** Map, int x, int y, int Rows, int Colls);

struct Links** Link_Create(struct Map* Graph, struct Platform* platform);

struct Map* Graph_create(char **Map, int Rows, int Colls);

#endif
