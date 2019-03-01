#include "fonctions.h"

// Je passe en paramètre la structure map
// Utilité : la fonction sert à remplir le tableau carte avec les informations d'un fichier texte
// elle retourne 0 si tout marche bien et 1 si l'ouverture de fichier ne fonctionne pas
int load_map(s_map* map) {
  float close = 1;
  char c;

  FILE *file = NULL;
  file = fopen("appart3.txt", "r");

  if (file == NULL) {
    printf("ERROR\n");
    return EXIT_FAILURE;
  }

  fscanf(file, "%d:%d", &map->width, &map->height);
  printf("Map : width = %d ; height = %d\n", map->width, map->height);
  c = fgetc(file); // character \n

  map->tab_map = malloc (sizeof(char*) * map->height);
  for(int h = 0; h < map->height; h++)
    map->tab_map[h] = malloc (sizeof(char) * map->width);

  map->tab_target = malloc (sizeof(int*) * map->height);
  for(int h = 0; h < map->height; h++)
    map->tab_target[h] = malloc (sizeof(int) * map->width);

  map->tab_fire = malloc (sizeof(char*) * map->height);
  for(int h = 0; h < map->height; h++)
    map->tab_fire[h] = malloc (sizeof(char) * map->width);

  for(int i = 0; i < map->height; i++) {
    for(int j = 0; j < map->width; j++) {
      c = fgetc(file);
      if(c == '\n')
        c = fgetc(file);
      map->tab_map[i][j] = c;
    }
  }

  close = fclose(file);

  if (close != 0)
    printf("ERROR !\n");

return EXIT_SUCCESS;
}

// Je passe en paramètre la structure map, la vie du robot, le nombre de mouvements du robot et s'il a gagné
// Utilité : la fonction sert à afficher le contenue du tableau map contenant la carte désirée
// elle ne retourne rien
void display_map(s_map* map, int life, int number_movement, int victory) {
  system("clear");

  for(int i = 0; i < map->height; i++) {
    for(int j = 0; j < map->width; j++) {
      printf("%c", map->tab_map[i][j]);
    }
    printf("\n");
  }

  if(life <= 10 && life >= 7) {
    printf("\t\t    Health : ");
    printf("\x1b[%dm", GREEN);
    printf("%d\n", life);
    printf("\x1b[%dm", WHITE);
  }
  else if(life < 7 && life > 3) {
    printf("\t\t    Health : ");
    printf("%d\n", life);
  }
  else if(life <= 3 && life > 0) {
    printf("\t\t    Health : ");
    printf("\x1b[%dm", RED);
    printf("%d\n", life);
  }
  else if(life == 0) {
    printf("\t\t    Health : ");
    printf("\x1b[%dm", RED);
    printf("%d\n", life);

  }
  printf("\x1b[%dm", WHITE);
  printf("\t     Number of movements : ");
  printf("\x1b[%dm", BLUE);
  printf("%d\n\n", number_movement);
  printf("\x1b[%dm", WHITE);

  if(life == 0) {
    printf("\x1b[%dm", RED);
    printf("\t\t    GAME OVER\n\n");
  }
  if(life != 0 && victory == 1) {
    printf("\x1b[%dm", YELLOW);
    printf("\t\t    VICTORY !!!\n\n");
    printf("\x1b[%dm", WHITE);
  }

}

// Je passe en paramètre les structures map et robot
// Utilité : la fonction sert à initialiser la position du robot en fonction de la position de la porte d'entrée
// elle ne retourne rien
void start_robot(s_map* map, s_robot* robot) {
  robot->victory = 0;
  map->hash_index = 0;
  robot->state = ROBOT;
  map->fire.x = 0;
  map->fire.y = 0;

  for(int i = 0; i < map->height; i++) {
    for(int j = 0; j < map->width; j++) {
      if(map->tab_map[i][j] == DOOR) {
        robot->pos.x = i;
        robot->pos.y = j;
        map->tab_map[robot->pos.x][robot->pos.y] = robot->state;
      }
    }
  }
  robot->life = 10;
  robot->nb_movement = 0;

  if(robot->pos.x == (map->height - 1) && robot->pos.y != (map->width - 1) && robot->pos.y != 0 && map->tab_map[robot->pos.x][robot->pos.y + 1] == WALL && map->tab_map[robot->pos.x][robot->pos.y - 1] == WALL) { // robot moves to the top
    map->tab_map[robot->pos.x][robot->pos.y] = DOOR;
    (robot->pos.x)--;
  }
  else if(robot->pos.x == 0 && robot->pos.y != (map->width - 1) && robot->pos.y != 0 && map->tab_map[robot->pos.x][robot->pos.y + 1] == WALL && map->tab_map[robot->pos.x][robot->pos.y - 1] == WALL) { // robot moves to the bottom
    map->tab_map[robot->pos.x][robot->pos.y] = DOOR;
    (robot->pos.x)++;
  }
  else if(robot->pos.y == 0 && robot->pos.x != (map->height - 1) && robot->pos.x != 0 && map->tab_map[robot->pos.x + 1][robot->pos.y] == WALL && map->tab_map[robot->pos.x - 1][robot->pos.y] == WALL) { // robot moves to the right
    map->tab_map[robot->pos.x][robot->pos.y] = DOOR;
    (robot->pos.y)++;
  }
  else if(robot->pos.y == (map->width - 1) && robot->pos.x != (map->height - 1) && robot->pos.x != 0 && map->tab_map[robot->pos.x + 1][robot->pos.y] == WALL && map->tab_map[robot->pos.x - 1][robot->pos.y] == WALL) { // robot moves to the left
    map->tab_map[robot->pos.x][robot->pos.y] = DOOR;
    (robot->pos.y)--;
  }

  map->tab_map[robot->pos.x][robot->pos.y] = robot->state;
  (robot->nb_movement)++;
  display_map(map, robot->life, robot->nb_movement, robot->victory);

}

// Je passe en paramètre les structures map et robot
// Utilité : la fonction sert à déplacer le robot vers le haut
// elle ne retourne rien
void move_up(s_map* map, s_robot* robot) {

  (robot->pos.x)--;
  map->tab_map[robot->pos.x][robot->pos.y] = robot->state;
  robot->nb_movement++;

  if(map->tab_fire[robot->pos.x + 1][robot->pos.y] == ONE) {
    map->tab_map[robot->pos.x + 1][robot->pos.y] = ONE;
    robot->life = robot->life - 1;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x + 1;
      map->fire.y = robot->pos.y;
    }
  }
  else if(map->tab_fire[robot->pos.x + 1][robot->pos.y] == TWO) {
    map->tab_map[robot->pos.x + 1][robot->pos.y] = TWO;
    robot->life = robot->life - 2;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x + 1;
      map->fire.y = robot->pos.y;
    }
  }
  else if(map->tab_fire[robot->pos.x + 1][robot->pos.y] == THREE) {
    map->tab_map[robot->pos.x + 1][robot->pos.y] = THREE;
    robot->life = robot->life - 3;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x + 1;
      map->fire.y = robot->pos.y;
    }
  }
  else {
    map->tab_map[robot->pos.x + 1][robot->pos.y] = EMPTY;
    robot->tab_searchfire[robot->pos.x + 1][robot->pos.y] = EXPLORED;
  }

}

// Je passe en paramètre les structures map et robot
// Utilité : la fonction sert à déplacer le robot vers le bas
// elle ne retourne rien
void move_down(s_map* map, s_robot* robot) {

  (robot->pos.x)++;
  map->tab_map[robot->pos.x][robot->pos.y] = robot->state;
  robot->nb_movement++;

  if(map->tab_fire[robot->pos.x - 1][robot->pos.y] == ONE) {
    map->tab_map[robot->pos.x - 1][robot->pos.y] = ONE;
    robot->life = robot->life - 1;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x - 1;
      map->fire.y = robot->pos.y;
    }
  }
  else if(map->tab_fire[robot->pos.x - 1][robot->pos.y] == TWO) {
    map->tab_map[robot->pos.x - 1][robot->pos.y] = TWO;
    robot->life = robot->life - 2;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x - 1;
      map->fire.y = robot->pos.y;
    }
  }
  else if(map->tab_fire[robot->pos.x - 1][robot->pos.y] == THREE) {
    map->tab_map[robot->pos.x - 1][robot->pos.y] = THREE;
    robot->life = robot->life - 3;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x - 1;
      map->fire.y = robot->pos.y;
    }
  }
  else {
    map->tab_map[robot->pos.x - 1][robot->pos.y] = EMPTY;
    robot->tab_searchfire[robot->pos.x - 1][robot->pos.y] = EXPLORED;
  }

}

// Je passe en paramètre les structures map et robot
// Utilité : la fonction sert à déplacer le robot vers la gauche
// elle ne retourne rien
void move_left(s_map* map, s_robot* robot) {

  (robot->pos.y)--;
  map->tab_map[robot->pos.x][robot->pos.y] = robot->state;
  robot->nb_movement++;

  if(map->tab_fire[robot->pos.x][robot->pos.y + 1] == ONE) {
    map->tab_map[robot->pos.x][robot->pos.y + 1] = ONE;
    robot->life = robot->life - 1;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x;
      map->fire.y = robot->pos.y + 1;
    }
  }
  else if(map->tab_fire[robot->pos.x][robot->pos.y + 1] == TWO) {
    map->tab_map[robot->pos.x][robot->pos.y + 1] = TWO;
    robot->life = robot->life - 2;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x;
      map->fire.y = robot->pos.y + 1;
    }
  }
  else if(map->tab_fire[robot->pos.x][robot->pos.y + 1] == THREE) {
    map->tab_map[robot->pos.x][robot->pos.y + 1] = THREE;
    robot->life = robot->life - 3;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x;
      map->fire.y = robot->pos.y + 1;
    }
  }
  else {
    map->tab_map[robot->pos.x][robot->pos.y + 1] = EMPTY;
    robot->tab_searchfire[robot->pos.x][robot->pos.y + 1] = EXPLORED;
  }

}

// Je passe en paramètre les structures map et robot
// Utilité : la fonction sert à déplacer le robot vers la droite en remettant derrière l'information nécessaire afin de ne pas modifier la carte après le passage du robot. Elle sert aussi à remplir la carte virtuelle robot->tab_searchfire d'étoiles après le passage du robot
// elle ne retourne rien
void move_right(s_map* map, s_robot* robot) {

  (robot->pos.y)++;
  map->tab_map[robot->pos.x][robot->pos.y] = robot->state;
  robot->nb_movement++;

  if(map->tab_fire[robot->pos.x][robot->pos.y - 1] == ONE) {
    map->tab_map[robot->pos.x][robot->pos.y - 1] = ONE;
    printf("%d : %d\n", robot->pos.x, robot->pos.y);
    robot->life = robot->life - 1;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x;
      map->fire.y = robot->pos.y - 1;
    }
  }
  else if(map->tab_fire[robot->pos.x][robot->pos.y - 1] == TWO) {
    map->tab_map[robot->pos.x][robot->pos.y - 1] = TWO;
    robot->life = robot->life - 2;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x;
      map->fire.y = robot->pos.y - 1;
    }
  }
  else if(map->tab_fire[robot->pos.x][robot->pos.y - 1] == THREE) {
    map->tab_map[robot->pos.x][robot->pos.y - 1] = THREE;
    robot->life = robot->life - 3;
    if(robot->pos.x > 0 && robot->pos.x < map->height && robot->pos.y > 0 && robot->pos.y < map->width) {
      map->fire.x = robot->pos.x;
      map->fire.y = robot->pos.y - 1;
    }
  }
  else {
    map->tab_map[robot->pos.x][robot->pos.y - 1] = EMPTY;
    robot->tab_searchfire[robot->pos.x][robot->pos.y - 1] = EXPLORED;
  }

}
