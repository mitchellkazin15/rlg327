//libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <endian.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <ncurses.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>

//namespaces
using namespace std;

//DUNGEON CONSTANTS
#define WIDTH 160
#define HEIGHT 105
#define HARDEST 255
#define ONE_HARD 85
#define ROOM '.'
#define PATH '+'
#define ROCK ' '
#define numRooms 25
#define EDGE 'E'
#define PC '@'
#define PC_SPEED 25
#define MAX_SPEED 20
#define MIN_SPEED 5
#define NUM_M_TYPES 16
#define NCURSES_WIDTH 80
#define NCURSES_HEIGHT 21
#define UP_STAIRCASE '<'
#define DOWN_STAIRCASE '>'
#define STD_MONS 15
#define EASY_MONS 5
#define HARD_MONS 25
#define EXP_MONS 50
#define DUNGEON_SIGHT_RADIUS 5
#define FOREST_SIGHT_RADIUS 5
#define MON_SIGHT_RADIUS 8
#define ROCK_CHAR '#'
#define WALL_CHAR '#'
#define TREE_CHAR '&'
#define GRASS_CHAR ';'
#define CARRY_SLOTS 10
#define SCROLL_SLOTS 10
#define SHOP_SLOTS 10
#define CONSUMABLE_SLOTS 20
#define STACK_CHAR '*'
#define LEVEL_SCALAR 8
#define OBJECT_SCALAR 10
#define VIT_SCALAR 10
#define TREE_HARD 10
#define DUNG_ENTRANCE 11
#define VERT_WALL 1
#define TOP_L_CORNER 2
#define TOP_R_CORNER 3
#define BOT_L_CORNER 4
#define BOT_R_CORNER 5
#define HORZ_WALL 6
#define FLOOR_OBJS 0
#define MON_DROP 1
#define DROP_RATE 5
#define BOSS_EXP_MULTIPLIER 2
#define BOSS_TELE 4
#define FROZEN_LENGTH 30
#define FIRST_BOSS_LEVEL -3
#define SECOND_BOSS_LEVEL -6

//KEY CONSTANTS
#define KEY_7 55
#define KEY_8 56
#define KEY_9 57
#define KEY_4 52
#define KEY_5 53
#define KEY_6 54
#define KEY_0 48
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_y 121
#define KEY_k 107
#define KEY_u 117
#define KEY_l 108
#define KEY_n 110
#define KEY_j 106
#define KEY_b 98
#define KEY_h 104
#define KEY_SPACE 32
#define KEY_L 76
#define KEY_Q 81
#define KEY_DOWNSTAIRS 62
#define KEY_UPSTAIRS 60
#define KEY_ESC 27
#define KEY_I 73
#define KEY_S1 70
#define KEY_S2 66
#define KEY_S3 126
#define KEY_S4 68
#define KEY_S5 69
#define KEY_S6 67
#define KEY_S7 72
#define KEY_S8 65
#define KEY_S9 126
#define KEY_a 97
#define KEY_b 98
#define KEY_c 99
#define KEY_d 100
#define KEY_e 101
#define KEY_f 102
#define KEY_g 103
#define KEY_h 104
#define KEY_i 105
#define KEY_w 119
#define KEY_t 116
#define KEY_x 120
#define KEY_m 109
#define KEY_o 111
#define KEY_p 112
#define KEY_q 113
#define KEY_r 114
#define KEY_s 115

//colors
#define COLOR_GRAY 8
#define COLOR_ROCK_DARK 9
#define COLOR_ROCK_ILUM 10
#define COLOR_DARK_GREEN 11
#define COLOR_RED_LAVA 12
#define COLOR_YELLOW_LAVA 13

//global variables
extern int roomDimensions[100][4];
extern int roomNumber;
extern char roomAndPath[WIDTH][HEIGHT];
extern unsigned char hardness[WIDTH][HEIGHT];
extern char characterMap[WIDTH][HEIGHT];
extern char objectMap[WIDTH][HEIGHT];
extern char staircases[WIDTH][HEIGHT];
extern unsigned char seen[WIDTH][HEIGHT];
extern int pcX;
extern int pcY;
extern char prevSpace;
extern int all_distances[WIDTH][HEIGHT];
extern int rp_distances[WIDTH][HEIGHT];
extern int dungeonLevel;
extern int highestReached;
extern int lowestReached;
extern int nextDungeon;
extern stringstream dungeonFile;
extern int killed;
extern int monsleft;
extern int difficulty;
extern int diffType[4];
extern int quit;
extern int numObjs;
extern int firstBossLevel;
extern int secondBossLevel;
extern int grassPattern[WIDTH][HEIGHT];
extern char grassTypes[4];
extern string villageName;
extern string villageNames[13];
extern bool isFrozen;
extern int frozenTurn;

//Dungeon enums
enum visible{
  HIDDEN,
  VISIBLE,
  SEEN
};

enum difficulty{
  easy,
  STD,
  hard,
  exp
};

typedef enum life_status {
  ALIVE,
  DEAD
} life_status_t;

enum color_pairs{
  blank,
  ILUM,
  DARK,
  R_DARK,
  MONST,
  R_ILUM,
  PC_COLOR,
  GRAS,
  D_GRAS,
  YEL,
  CY,
  BL,
  MAG,
  LAV_ROCK,
  LAV,
  Y_LAV
};

//file headers
#include "heap.h"
#include "macros.h"
#include "io_main.h"
#include "dijkstra.h"
#include "dice.h"
#include "object.h"
#include "object_description.h"
#include "monster.h"
#include "player.h"
#include "game.h"
#include "dice.h"
#include "monster_desc.h"

extern player* main_character;
extern monster *mondun[WIDTH][HEIGHT];
extern object *objdun[WIDTH][HEIGHT];
extern monster *boss;



