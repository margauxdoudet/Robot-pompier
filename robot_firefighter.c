// J'ai réussi à régler le problème de téléportation, le robot bouge dorénavant de façon fluide

#include "fonctions.h"

int main() {
  s_map *map = (s_map*) malloc (sizeof(s_map));
  if(map == NULL) { // if the malloc didn't work, exit the program
    printf("Not enoough memory space\n");
    exit(EXIT_FAILURE);
  }

  s_robot *robot = (s_robot*) malloc (sizeof(s_robot));
  if(robot == NULL) { // if the malloc didn't work, exit the program
    printf("Not enoough memory space\n");
    exit(EXIT_FAILURE);
  }

  int out = load_map(map);
  if(out == 1) {
    return EXIT_FAILURE;
  }

  start_robot(map, robot);

  fill_vmap(map, robot, map->extinguisher);

  movement_robot(map, robot);

  find_fire(map, robot);
  robot->life--;
  robot->victory = 1;
  display_map(map, robot->life, robot->nb_movement, robot->victory);

  free(robot->tab_searchfire);
  free(map->tab_fire);
  free(map->tab_target);
  free(robot);
  free(map->tab_map);
  free(map);

return EXIT_SUCCESS;
}
