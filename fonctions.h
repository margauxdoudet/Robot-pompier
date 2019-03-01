#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define WALL 'x'
#define ROBOT 'R'
#define DOOR 'D'
#define NOT_EXPLORED '#'
#define EXPLORED '*'
#define EXTINGUISHER 'E'
#define FIREBOT 'F' // the robot found the extinguisher
#define EMPTY ' '
#define RED 31
#define WHITE 37
#define GREEN 32
#define BLUE 34
#define YELLOW 93
#define ONE '1'
#define TWO '2'
#define THREE '3'
#define UP '8'
#define DOWN '2'
#define RIGHT '6'
#define LEFT '4'

// declaration of structures
typedef struct s_position {
  int x;
  int y;
}s_position;

typedef struct s_map {
  char** tab_map;
  char** tab_fire;
  int width;
  int height;
  struct s_position extinguisher;
  int** tab_target;
  struct s_position fire;
  struct s_position hash;
  int hash_index;
}s_map;

typedef struct s_robot {
  struct s_position pos;
  int nb_movement;
  int life;
  char** tab_searchfire;
  char state;
  int victory;
}s_robot;

// declarations of fonctions
int load_map(s_map* map);
void display_map(s_map* map, int life, int number_movement, int victory);
void start_robot(s_map* map, s_robot* robot);
void move_up(s_map* map, s_robot* robot);
void move_down(s_map* map, s_robot* robot);
void move_left(s_map* map, s_robot* robot);
void move_right(s_map* map, s_robot* robot);
void movement_robot(s_map* map, s_robot* robot);
void fill_vmap(s_map* map, s_robot* robot, s_position target);
void display_target(int width, int height,int** tab);
void find_fire(s_map* map, s_robot* robot);
void if_fire_not_found_yet(s_map* map, s_robot* robot);
void search_fire(s_map* map, s_robot* robot);
void move_discover(s_map* map, s_robot* robot);

#endif
