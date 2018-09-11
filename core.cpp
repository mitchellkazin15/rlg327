#include "core.h"

//global variables
int roomDimensions[100][4];
int roomNumber;
char roomAndPath[WIDTH][HEIGHT];
unsigned char hardness[WIDTH][HEIGHT];
char characterMap[WIDTH][HEIGHT];
char objectMap[WIDTH][HEIGHT];
char staircases[WIDTH][HEIGHT];
unsigned char seen[WIDTH][HEIGHT];
int pcX;
int pcY;
char prevSpace;
int all_distances[WIDTH][HEIGHT];
int rp_distances[WIDTH][HEIGHT];
int dungeonLevel;
int highestReached;
int lowestReached;
int nextDungeon;
stringstream dungeonFile;
int killed;
int monsleft;
int difficulty;
int diffType[4];
int quit;
int numObjs;
int firstBossLevel;
int secondBossLevel;
int grassPattern[WIDTH][HEIGHT];
string villageName;
bool isFrozen = false;
int frozenTurn;

string villageNames[13] = 
{
"CLIFFMOOR",
"FAYVIEW",
"HAVENMILL",
"OLDFALL",
"IRON HILL",
"WESTERWHEAT",
"ORHAVEN",
"WELLOAK",
"WHITEBURN",
"PINEWALL",
"OAKENBARROW",
"HIGHSUMMER",
"HOLLENDALE"
};

char grassTypes[4] = 
{
  ';',
  '\'',
  ',',
  '.'
};

player* main_character;
monster *boss;
monster *mondun[WIDTH][HEIGHT];
object *objdun[WIDTH][HEIGHT];





