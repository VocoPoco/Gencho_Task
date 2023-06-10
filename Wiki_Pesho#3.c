#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Block {
  int value; // value na node (0, 1, 2, 3, 4, 5)
  struct Block *next;
};
struct Links {
  struct Platform *platform_node;
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

void update_visited_to_false(struct Map *map) {
  for (int i = 0; i < map->platform_count; i++) {
    map->platform_array[i]->is_visited = false;
  }
}

struct Platform *find_safe_platform(struct Map *map) {
  map->head_platform->is_visited = true;

  for (int i = 0; i < map->head_platform->num_links; ++i) {
    struct Platform *temp = map->head_platform->link_array[i]->platform_node;
    if (map->head_platform->link_array[i]->distance < polizei.jump &&
        map->head_platform->link_array[i]->distance > pesho.jump) {
      break;
    }

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

void turn_pesho(struct Map *map) {
  if (pesho.position->next == NULL) {
    map->head_platform = map->head_platform;
  }
  pesho.position = pesho.position->next;
  // position edno napred ili mesti platformata
  //  tursi nai malko malki platformi
}

void turn_polizei(struct Map *map) {}


void dfs_search(struct Platform *currentPlatform,
                struct Platform *starting_platform, int *link_route,
                int routeIndex, int ***stored_links, int *routeCount) {
  currentPlatform->is_visited = true;
  link_route[routeIndex] = currentPlatform->num_blocks;
  routeIndex++;

  if (currentPlatform == starting_platform) {
    bool allLinksUnderThreshold = true;
    for (int i = 0; i < routeIndex - 1; ++i) {
      if (currentPlatform->link_array[i]->distance >= pesho.jump) {
        allLinksUnderThreshold = false;
        break;
      }
    }

    if (allLinksUnderThreshold) {
      *stored_links = realloc(*stored_links, (*routeCount + 1) * sizeof(int *));
      (*stored_links)[*routeCount] = calloc(routeIndex, sizeof(int));

      for (int i = 0; i < routeIndex; ++i) {
        (*stored_links)[*routeCount][i] = link_route[i];
      }

      (*routeCount)++;
    }
  } else {
    for (int i = 0; i < currentPlatform->num_links; ++i) {
      struct Platform *nextPlatform = currentPlatform->link_array[i]->platform_node;
      if (!nextPlatform->is_visited) {
        dfs_search(nextPlatform, starting_platform, link_route,
                   routeIndex, stored_links, routeCount);
      }
    }
  }

  routeIndex--;
  currentPlatform->is_visited = false;
}

void real_life(struct Map *map) {
  struct Platform *safe_platform = find_safe_platform_util(map);

  if (safe_platform == NULL) {
    return;
  }

  struct Platform *starting_platform = map->head_platform;

  int **stored_links = NULL;
  int routeCount = 0;

  int *link_route = calloc(map->platform_count, sizeof(int));

  dfs_search(safe_platform, starting_platform, link_route, 0,
             &stored_links, &routeCount);

  free(link_route);

  for (int i = 0; i < routeCount; ++i) {
    free(stored_links[i]);
  }
  free(stored_links);
}


int main(void) {
  printf("Hello World\n");
  return 0;
}