#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.h"

#define MAX_MAP_WIDTH 200

//Viki code

void update_visited_to_false(struct Map *map) {
  for (int i = 0; i < map->platform_count; i++) {
    map->platform_array[i]->is_visited = false;
  }
}

struct Platform *find_safe_platform(struct Map *map) {
  map->head_platform->is_visited = true;

  for (int i = 0; i < map->head_platform->num_links; ++i) {
    struct Platform *temp = map->head_platform->link_array[i]->platform_node;
    // proverka dali platformata e available
    if (map->head_platform->link_array[i]->distance < polizei.jump &&
        map->head_platform->link_array[i]->distance > pesho.jump) {
      break;
    }

    // if unvisited
    if (!temp->is_visited) {
      struct Platform *result = find_safe_platform(map);
      if (result != NULL) {
        return result;
      }
    }
  }
  return NULL;
}

// util function to find_safe_platform so that it turns all visited booleans
// to false before beginning
struct Platform *find_safe_platform_util(struct Map *map) {
  update_visited_to_false(map);
  struct Platform *safe_platform = find_safe_platform(map);
  return safe_platform;
}

// positions polizei in the beggining of the platform and pesho at the end
void position_characters(struct Map *map) {
  struct Block *head = map->head_platform->block_head;
  if (head == NULL) {
    return;
  }
  polizei.position = head;

  struct Block *temp = head;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  pesho.position = temp;
}

struct Platform **check_available_route(struct Platform *starting_platform,
                                        struct Platform *safe_platform) {
  int routeCount = 0;
  struct Platform **stored_links =
      realloc(stored_links, (routeCount + 1) * sizeof(struct Platform *));

  struct Platform *queue[100];
  int front = 0;
  int rear = 0;

  starting_platform->is_visited = true;
  queue[rear++] = starting_platform;

  while (front < rear) {
    struct Platform *current_platform = queue[front++];

    if (current_platform == safe_platform) {
      int block_sum = 0;
      int routeIndex = front - 1;
      while (routeIndex >= 0) {
        struct Platform *platform = queue[routeIndex];
        if (platform->num_blocks > 2 && platform->num_blocks % 2 == 0) {
          platform->num_blocks += 1;
        }
        block_sum += platform->num_blocks;
        routeIndex--;
      }

      int numberOfLinks = front - 1;
      int numberOfPlatforms = front;

      // calculate the sum of all links and platform block counts in the route
      int polizei_sum = numberOfLinks + numberOfPlatforms * (block_sum - 1);
      int pesho_sum = numberOfLinks + numberOfPlatforms * (block_sum - 1) -
                      starting_platform->num_blocks;

      if (polizei_sum > pesho_sum) {
        printf("Pesho is caught!!\n");
      } else {
        printf("Pesho managed to evade jail!\n");
        stored_links =
            realloc(stored_links, (routeCount + 1) * sizeof(struct Platform *));
        stored_links[routeCount] = malloc(front * sizeof(struct Platform *));
        for (int i = 0; i < front; i++) {
          stored_links[routeCount][i] = *queue[i];
        }
        routeCount++;
      }
    } else {
      struct Platform *neighbor_platforms[100];
      int neighbor_count = 0;

      // store available neighbor platforms
      for (int i = 0; i < current_platform->num_links; ++i) {
        struct Platform *next_platform =
            current_platform->link_array[i]->platform_node;
        if (!next_platform->is_visited &&
            current_platform->link_array[i]->distance < pesho.jump) {
          neighbor_platforms[neighbor_count++] = next_platform;
          next_platform->is_visited = true;
        }
      }

      if (neighbor_count > 0) {
        // find the neighbor platform with the lowest block count and is
        // unvisited
        struct Platform *next_platform = neighbor_platforms[0];
        int lowest_block_count = next_platform->num_blocks;

        for (int i = 1; i < neighbor_count; ++i) {
          if (neighbor_platforms[i]->num_blocks < lowest_block_count) {
            next_platform = neighbor_platforms[i];
            lowest_block_count = next_platform->num_blocks;
          }
        }

        queue[rear++] = next_platform;
      }
    }
  }

  return stored_links;
}

void real_life(struct Map *map) {
  struct Platform *safe_platform = find_safe_platform_util(map);

  if (safe_platform == NULL) {
    return;
  }

  struct Platform *starting_platform = map->head_platform;
  check_available_route(starting_platform, safe_platform);
}

//End of Viki code

// Anto code

//Vadim texta ot Map.txt v dvuizmeren dinamichen masiv ot charove
char** readMapFromFile(char filepath[100], int* Rows, int* Colls) {

  //File opening
  int len = strlen(filepath);
  if (len > 0 && filepath[len - 1] == '\n') {
    filepath[len - 1] = '\0';
  }
  FILE* file = fopen(filepath, "r");
  if (file == NULL) {
    printf("Failed to open file.\n");
    return NULL;
  }

  //Counting Colls
  char line[MAX_MAP_WIDTH];
  fgets(line, sizeof(line), file);
  *Colls = strlen(line);
  rewind(file);

  //Counting rows
  *Rows = 0;
  while (fgets(line, sizeof(line), file) != NULL) {
    (*Rows)++;
  }

  //Zadelqme pamet za pointeri kum char* masivi
  char** map = malloc((*Rows) * sizeof(char*));
  if (map == NULL) {
    printf("Failed to allocate memory\n");
    return NULL;
  }

  //Zadelqme pamet za char* masivite
  for (int i = 0; i < *Rows; i++) {
    map[i] = malloc((*Colls + 1) * sizeof(char));
  }

  rewind(file);

  //Kopvame i printvame mapa
  printf("Map:\n");
  int i = 0;
  while (fgets(line, sizeof(line), file) != NULL) {
    strcpy(map[i], line);
    printf("%s", line);
    i++;
  }

  fclose(file);

  return map;
}

//Chekvame dali na dadeni koordinati ima platforma
bool Platform_Check(char** Map, int x, int y, int Rows, int Colls) {
  if (x >= 0 && y < Rows && y >= 0 && x < Colls) {
    return Map[y][x] == '#';
  }
  return false;
}

//Suzdavame nova platforma
struct Platform* Platform_Create(char** Map, int x, int y, int Rows, int Colls){
  //Zadelqme pamet za Platformata i purviq block
  struct Platform *platform = malloc(1 * sizeof(struct Platform));
  struct Block *Head_block = malloc(1 * sizeof(struct Block));

  platform->block_head = Head_block;
  platform->is_visited = 0;
  platform->num_blocks = 0;
  Head_block->value = 0;
  Head_block->x = x;
  Head_block->y = y;
  Head_block->next = NULL;

  //Suzdavame iterator
  struct Block *temp = Head_block;
  int block_count = 1; 

  //Svurvzvame vsichki blockove v platformata
  for(int i = 1; Platform_Check(Map, x + i, y, Rows, Colls); i++){
    temp->next = malloc(1 * sizeof(struct Block));
    temp = temp->next;
    temp->x = x + i;
    temp->y = y;
    temp->next = NULL;
    temp->value = i - 1;;
    block_count++;

  }

  platform->num_blocks = block_count;

  return platform;
}
struct Links** Link_Create(struct Map* Graph, struct Platform* platform){
  struct Links** link_array = malloc(50 * sizeof(struct Links*)); //ochakvame max 50 platformi
  struct Links* link;
  int min_distance;
  platform->link_array = link_array;
  for(int i = 0; i < Graph->platform_count - 1; i++){
    platform->link_array[i] = malloc(1 * sizeof(struct Links));
    platform->link_array[i]->index = i;
    platform->link_array[i]->platform_node = Graph->platform_array[i];
    min_distance = abs(platform->block_head->x - Graph->platform_array[i]->block_head->x) + abs(platform->block_head->y - Graph->platform_array[i]->block_head->y);
    for(int j; j < Graph->platform_array[i]->num_blocks; j++){
      for(int k; k < platform->num_blocks; k++){
        if(min_distance > (abs(platform->block_head->x - Graph->platform_array[i]->block_head->x) + abs(platform->block_head->y - Graph->platform_array[i]->block_head->y))){

        }
      }

      
    }
  }
  return link_array;
}
//Suzdavame graph of plain text map
struct Map* Graph_create(char **Map, int Rows, int Colls){

  //Zadelqme pamet za Grapha i arraya ot pointeri kum platformi
  struct Map* Graph = malloc(1 * sizeof(struct Map));
  Graph->platform_count = 0;
  Graph->platform_array = malloc(50 * sizeof(struct Platform));

  struct Platform *Temp;
  for(int y = 0; y < Rows; y++){
    for(int x = 0; x < Colls; x++){
      if(Platform_Check(Map, x, y, Rows, Colls)) {
        Temp = Platform_Create(Map, x, y, Rows, Colls);
        if(Graph->platform_count == 0) Graph->head_platform = Temp;
        Graph->platform_array[Graph->platform_count] = Temp;
        Graph->platform_count++;
        x = x + Temp->num_blocks - 1;
        if(Graph->platform_count > 1) Temp->link_array = Link_Create(Graph, Temp);
        else Graph->head_platform->link_array = malloc(50 * sizeof(struct Links*));
      }
    }
  }

  return Graph;
}