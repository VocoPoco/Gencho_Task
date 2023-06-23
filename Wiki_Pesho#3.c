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

struct Platform** check_available_route(struct Platform *starting_platform,
                             struct Platform *safe_platform) {
  int routeCount = 0;
  struct Platform **stored_links = calloc(1, sizeof(struct Platform *));

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
      int pesho_sum = numberOfLinks + numberOfPlatforms * (block_sum - 1) - starting_platform->num_blocks;

    if (polizei_sum > pesho_sum) {
      printf("Pesho is caught!!\n");
    } else {
      printf("Pesho managed to evade jail!\n");
      struct Platform **temp_links = calloc(1 , (routeCount + 1) * sizeof(struct Platform *));
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
        struct Platform *next_platform = current_platform->link_array[i]->platform_node;
        if (!next_platform->is_visited && current_platform->link_array[i]->distance < pesho.jump) {
          neighbor_platforms[neighbor_count++] = next_platform;
          next_platform->is_visited = true;
        }
      }

      if (neighbor_count > 0) {
        // find the neighbor platform with the lowest block count and is unvisited
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

// int main(void) {
//   struct Map map;
//   map.platform_count = 2;
//   map.platform_array = calloc(1, map.platform_count * sizeof(struct Platform *));

//   struct Platform *platform1 = calloc(1, sizeof(struct Platform));
//   map.head_platform = platform1;
//   platform1->num_links = 1;
//   platform1->link_array = calloc(1, platform1->num_links * sizeof(struct Links *));
//   platform1->is_visited = false;

//   struct Platform *platform2 = calloc(1, sizeof(struct Platform));
//   map.platform_array[1] = platform2;
//   platform2->num_links = 0;
//   platform2->is_visited = false;

//   struct Links *link12 = calloc(1, sizeof(struct Links));
//   link12->platform_node = platform2;
//   link12->distance = 5;
//   link12->index = 0;
//   platform1->link_array[0] = link12;

//   pesho.jump = 1;
//   polizei.jump = 3;

//   real_life(&map);

//   return 0;
// }
//
// expected output: "Pesho managed to evade jail!"