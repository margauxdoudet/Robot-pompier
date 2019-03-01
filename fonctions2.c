#include "fonctions.h"


// Je passe en paramètre les structures map et robot et le point désiré avec ses coordonnées
// Utilité : la fonction sert à remplir un tableau d'une carte virtuelle dans le but de créer un labyrinthe atteignant le point désiré (extincteur, puis feu (si connu), puis la case NOT_EXPLORED la plus proche)
// elle ne retourne rien
void fill_vmap(s_map* map, s_robot* robot, s_position target) {
  int index = 0;
  int target_end = 0;

  robot->tab_searchfire = malloc (sizeof(char*) * map->height);
  for(int h = 0; h < map->height; h++)
    robot->tab_searchfire[h] = malloc (sizeof(char) * map->width);

  // FILL MAP TAB_SEARCHFIRE
  if(map->hash_index == 0) {
    for(int i = 0; i < map->height; i++) {
      for(int j = 0; j < map->width; j++) {
        if(map->tab_map[i][j] == WALL || map->tab_map[i][j] == DOOR)
          robot->tab_searchfire[i][j] = WALL;
        else if(map->tab_map[i][j] == EMPTY || map->tab_map[i][j] == ONE || map->tab_map[i][j] == TWO || map->tab_map[i][j] == THREE || map->tab_map[i][j] == ROBOT)
          robot->tab_searchfire[i][j] = NOT_EXPLORED;
      }
    }
  }

  for(int i = 0; i < map->height; i++) {
    for(int j = 0; j < map->width; j++) {
      if(map->tab_map[i][j] == WALL || map->tab_map[i][j] == DOOR)
        map->tab_target[i][j] = -2;
      else if(robot->state == ROBOT) {
        if(map->tab_map[i][j] == EXTINGUISHER)
          map->tab_target[i][j] = 0;
        else
          map->tab_target[i][j] = -1;
      }
      else if(robot->state == FIREBOT && map->fire.x != 0 && map->fire.y != 0 && map->hash_index == 0) {
        if(map->tab_map[i][j] == ONE)
          map->tab_target[i][j] = 0;
        else
          map->tab_target[i][j] = -1;
      }
      else if(robot->state == FIREBOT && map->hash_index == 1) {
        if(robot->tab_searchfire[i][j] == NOT_EXPLORED)
          map->tab_target[i][j] = 0;
        else
          map->tab_target[i][j] = -1;
      }
    }
  }

  for(int i = 0; i < map->height; i++) {
    for(int j = 0; j < map->width; j++) {
      if(map->tab_map[i][j] == WALL)
        map->tab_fire[i][j] = WALL;
      else if(map->tab_map[i][j] == DOOR)
        map->tab_fire[i][j] = DOOR;
      else if(map->tab_map[i][j] == ONE)
        map->tab_fire[i][j] = ONE;
      else if(map->tab_map[i][j] == TWO)
        map->tab_fire[i][j] = TWO;
      else if(map->tab_map[i][j] == THREE)
        map->tab_fire[i][j] = THREE;
      else
        map->tab_fire[i][j] = EMPTY;
    }
  }
  //display_target(map->width, map->height, map->tab_fire);

  while(target_end == 0) {
    for(int i = 0; i < map->height; i++) {
      for(int j = 0; j < map->width; j++) {
        if(map->tab_target[i][j] == index) {
          if(map->tab_target[i][j + 1] == -1) {
            map->tab_target[i][j + 1] = index + 1;
          }
          if(map->tab_target[i][j - 1] == -1) {
            map->tab_target[i][j - 1] = index + 1;
          }
          if(map->tab_target[i + 1][j] == -1) {
            map->tab_target[i + 1][j] = index + 1;
          }
          if(map->tab_target[i - 1][j] == -1) {
            map->tab_target[i - 1][j] = index + 1;
          }
        }
      }
    }
    if(map->tab_target[robot->pos.x][robot->pos.y] != -1) {
      target_end = 1;
    }
    index++;
  }

}

// Je passe en paramètre les structures map et robot
// Utilité : la fonction sert à interprêter le mouvement nécessaire par rapport à la position du robot. Elle appelle la fonction nécessaire suivi de la fonction affichage
// elle ne retourne rien
void movement_robot(s_map* map, s_robot* robot) {

  while(map->tab_target[robot->pos.x][robot->pos.y] != 0) {
    if(map->tab_target[robot->pos.x - 1][robot->pos.y] < map->tab_target[robot->pos.x][robot->pos.y] && map->tab_target[robot->pos.x - 1][robot->pos.y] >= 0)
      move_up(map, robot);
    else if(map->tab_target[robot->pos.x + 1][robot->pos.y] < map->tab_target[robot->pos.x][robot->pos.y] && map->tab_target[robot->pos.x + 1][robot->pos.y] >= 0)
      move_down(map, robot);
    else if(map->tab_target[robot->pos.x][robot->pos.y - 1] < map->tab_target[robot->pos.x][robot->pos.y] && map->tab_target[robot->pos.x][robot->pos.y - 1] >= 0)
      move_left(map, robot);
    else if(map->tab_target[robot->pos.x][robot->pos.y + 1] < map->tab_target[robot->pos.x][robot->pos.y] && map->tab_target[robot->pos.x][robot->pos.y + 1] >= 0)
      move_right(map, robot);
    display_map(map, robot->life, robot->nb_movement, robot->victory);
    usleep(80000);
  }

  robot->state = FIREBOT;
  map->tab_map[robot->pos.x][robot->pos.y] = robot->state;
  robot->tab_searchfire[robot->pos.x][robot->pos.y] = EXPLORED;
  display_map(map, robot->life, robot->nb_movement, robot->victory);

}

// Je passe en paramètre la hauteur et la largeur de la carte (du tableau) et le tableau à afficher
// Utilité : la fonction sert à afficher le contenu des tableaux, soit les cartes virtuelles
// elle ne retourne rien
void display_target(int width, int height, int** tab) {

  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      printf("%02d ", tab[i][j]);
    }
    printf("\n");
  }

}

// Je passe en paramètre les structures map et robot
// Utilité : la fonction cherche à savoir si le robot connait déjà la position du feu et agit en conséquence. Si oui, elle appelle fill_vmap puis movement_robot, si non elle appelle if_fire_not_found
// elle ne retourne rien
void find_fire(s_map* map, s_robot* robot) {

  if(map->fire.x == 0 && map->fire.y == 0) // if the robot has not found the fire yet
    if_fire_not_found_yet(map, robot);
  else {
    fill_vmap(map, robot, map->hash);
    movement_robot(map, robot);
  }
  robot->victory = 1;
  display_map(map, robot->life, robot->nb_movement, robot->victory);

}

// Je passe en paramètre les structures map et robot
// Utilité : la fonction sert à chercher le feu si celui-ci n'est pas encore connu au moment de trouver l'extincteur
// elle ne retourne rien
void if_fire_not_found_yet(s_map* map, s_robot* robot) {
  int fire_found = 0;
  int x;
  int y;
  int target_end;
  int index;

  while(fire_found == 0) {
    if(map->tab_fire[robot->pos.x][robot->pos.y] == EMPTY) {
      if(robot->tab_searchfire[robot->pos.x - 1][robot->pos.y] == NOT_EXPLORED || robot->tab_searchfire[robot->pos.x + 1][robot->pos.y] == NOT_EXPLORED || robot->tab_searchfire[robot->pos.x][robot->pos.y + 1] == NOT_EXPLORED || robot->tab_searchfire[robot->pos.x][robot->pos.y - 1] == NOT_EXPLORED)
        move_discover(map, robot);
      else {
        map->hash_index = 1;
        //fill_vmap(map, robot, map->hash);
        //movement_robot(map, robot);
        //move_discover(map, robot);
          map->tab_map[robot->pos.x][robot->pos.y] = EMPTY;
          robot->tab_searchfire[robot->pos.x][robot->pos.y] = EXPLORED;
        while(map->hash_index == 1) {
          for(int i = 0; i < map->height; i++) {
            for(int j = 0; j < map->width; j++) {
              if(robot->tab_searchfire[i][j] == NOT_EXPLORED) {
                x = i;
                y = j;
                map->hash_index = 2;
              }
            }
          }
        }

        for(int i = 0; i < map->height; i++) {
          for(int j = 0; j < map->width; j++) {
            if(map->tab_map[i][j] == WALL || map->tab_map[i][j] == DOOR)
              map->tab_target[i][j] = -2;
            else if(i == x && j == y)
              map->tab_target[i][j] = 0;
            else
              map->tab_target[i][j] = -1;
          }
        }
        
        target_end = 0;
        index = 0;

        while(target_end == 0) {
          for(int i = 0; i < map->height; i++) {
            for(int j = 0; j < map->width; j++) {
              if(map->tab_target[i][j] == index) {
                if(map->tab_target[i][j + 1] == -1) {
                  map->tab_target[i][j + 1] = index + 1;
                }
                if(map->tab_target[i][j - 1] == -1) {
                  map->tab_target[i][j - 1] = index + 1;
                }
                if(map->tab_target[i + 1][j] == -1) {
                  map->tab_target[i + 1][j] = index + 1;
                }
                if(map->tab_target[i - 1][j] == -1) {
                  map->tab_target[i - 1][j] = index + 1;
                }
              }
            }
          }
          if(map->tab_target[robot->pos.x][robot->pos.y] != -1) {
            target_end = 1;
          }
          index++;
        }
        movement_robot(map, robot);
      }
      display_map(map, robot->life, robot->nb_movement, robot->victory);
      usleep(80000);
      if(map->tab_fire[robot->pos.x][robot->pos.y] == ONE || map->tab_fire[robot->pos.x][robot->pos.y] == TWO || map->tab_fire[robot->pos.x][robot->pos.y] == THREE)
        fire_found = 1;
    }
  }

}

// Je passe en paramètre les structures map et robot
// Utilité : la fonction sert à remplacer les cases NOT_EXPLORED par des cases EXPLORED si le robot est passé dessus (dans la carte virtuelle robot->tab_searchfire)
// elle ne retourne rien
void move_discover(s_map* map, s_robot* robot) {

  if(robot->tab_searchfire[robot->pos.x - 1][robot->pos.y] == NOT_EXPLORED)
    move_up(map, robot);
  else if(robot->tab_searchfire[robot->pos.x + 1][robot->pos.y] == NOT_EXPLORED)
    move_down(map, robot);
  else if(robot->tab_searchfire[robot->pos.x][robot->pos.y + 1] == NOT_EXPLORED)
    move_right(map, robot);
  else if(robot->tab_searchfire[robot->pos.x][robot->pos.y - 1] == NOT_EXPLORED)
    move_left(map, robot);

}
