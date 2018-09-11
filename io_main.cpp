#include "core.h"

int bunker = 0;
int path_type[4] = {1,1,2,3};

int hasNeighborRoom(int x, int y){
  int i,j;
  int ret = 0;
  for(i = x - 1; i <= x + 1; ++i){
    for(j = y - 1; j <= y + 1; ++j){
      if(roomAndPath[i][j] == ROOM && roomAndPath[x][y] != ROOM){
	if(i == x - 1 && j == y - 1){
	  ret = BOT_R_CORNER;
        }
	else if(i == x + 1 && j == y + 1){
	  ret = TOP_L_CORNER;
        }
	else if(i == x - 1 && j == y + 1){
	  ret = TOP_R_CORNER;
        }
	else if(i == x - 1 && j == y){
	  return VERT_WALL;
        }
        else if(i == x + 1 && j == y){
	  return VERT_WALL;
	}
        else if(i == x && j == y - 1){
	  return HORZ_WALL;
	}
        else if(i == x + 1 && j == y - 1){
	  ret = BOT_L_CORNER;
	}
        else if(i == x && j == y + 1){
	  return HORZ_WALL;
	}
      }
    }
  }
  return ret;
}

void makeWalls(){
  int i,j,r;
  for(j = 1; j < HEIGHT - 1; ++j){
    for(i = 1; i < WIDTH - 1; ++i){
      int hard = hasNeighborRoom(i,j);
      if(hard != 0 && roomAndPath[i][j] != PATH){
        if(roomAndPath[i][j] != ROOM){
	  hardness[i][j] = hard;
        }
      }
    }
  }
  for(r = 0; r < numRooms; ++r){
    
    int x,y,w,h,randSpot;
    x = roomDimensions[r][0];
    y = roomDimensions[r][1];
    w = roomDimensions[r][2];
    h = roomDimensions[r][3];
    if(hardness[x - 2][y - 1] != HARDEST){
      randSpot = rand() % h;
      hardness[x - 1][y + randSpot] = 0;
      grassPattern[x - 1][y + randSpot] = 8;
    }
    if(hardness[x + w + 1][y + 1] != HARDEST){
      randSpot = rand() % h;
      hardness[x + w][y + randSpot] = 0;
      grassPattern[x + w][y + randSpot] = 8;
    }
    if(hardness[x - 1][y - 2] != HARDEST){
      randSpot = (rand() % w);
      hardness[x + randSpot][y - 1] = 0;
      grassPattern[x + randSpot][y - 1] = 8;
    }
    if(hardness[x + 1][y + h + 1] != HARDEST){
      randSpot = (rand() % w);
      hardness[x + randSpot][y + h] = 0;
      grassPattern[x + randSpot][y + h] = 8;
    }
  }
}

void addTrees(int ratio){
  int i,j;
  for(i = 1; i < WIDTH - 1; ++i){
    for(j = 1; j < HEIGHT - 1; ++j){
      int tree = rand() % ratio;
      if(tree == 0 && hardness[i][j] == 0 && i != pcX && j != pcY && 
				 staircases[i][j] != UP_STAIRCASE && 			
				 staircases[i][j] != DOWN_STAIRCASE){
	if(roomAndPath[i][j] != ROOM && dungeonLevel == 0){
	  hardness[i][j] = TREE_HARD;
	}
	else if(dungeonLevel > 0){
	  hardness[i][j] = TREE_HARD;
	}
      }
    }
  }
}

void drawPaths(int corners[numRooms][2]){
  int p,i,j,x,y,xp,yp,bigx,bigy;
  for(p = 0; p < numRooms - 1; ++p){
    if(corners[p][0] < corners[p+1][0]){
      xp = corners[p+1][0];
      x = corners[p][0];
      bigx = 1;
    }
    else{
      xp = corners[p][0];
      x = corners[p+1][0];
      bigx = 1;
    }
    if(corners[p][1] < corners[p+1][1]){
      yp = corners[p+1][1];
      y = corners[p][1];
      bigy = 1;
    }
    else{
      yp = corners[p][1];
      y = corners[p+1][1];
      bigy = 1;
    }
    if(bigy == 1){
      for(i = x; i <= xp; ++i){
	if(roomAndPath[i][yp] != ROOM){
          roomAndPath[i][yp] = PATH;
          hardness[i][yp] = 0;
	}
      }
    }
    if(bigx == 1){
      for(j = y; j <= yp; ++j){
	if(roomAndPath[xp][j] != ROOM){
          roomAndPath[xp][j] = PATH;
          hardness[xp][j] = 0;
	}
      }
    }
    if(bigy == 1){
      for(i = x; i <= xp; ++i){
	if(roomAndPath[i][y] != ROOM){
          roomAndPath[i][y] = PATH;
          hardness[i][y] = 0;
	}
      }
    }
    if(bigx == 1){
      for(j = y; j <= yp; ++j){
	if(roomAndPath[x][j] != ROOM){
          roomAndPath[x][j] = PATH;
          hardness[x][j] = 0;
	}
      }
    }
  }
}

void generateRooms(){
  int r,i,j,x,y,w,h,isEmpty;
  for(i = 0; i < WIDTH; ++i){
    for(j = 0; j < HEIGHT; ++j){
      staircases[i][j] = (char) 0;
      seen[i][j] = HIDDEN;
    }
  }
  r = 0;
  for(i = 0; i < 100; ++i){
    roomDimensions[i][0] = 0;
    roomDimensions[i][1] = 0;
    roomDimensions[i][2] = 0;
    roomDimensions[i][3] = 0;
  }
  int corners[numRooms][2] = {{0,0}};
  roomNumber = numRooms;
  while(r < numRooms){
    x = (rand() % (WIDTH - 24)) + 4;
    y = (rand() % (HEIGHT - 18)) + 4;
    w = (rand() % 12) + 7;
    h = (rand() % 10) + 5;
    isEmpty = 1;
    for(j = y - 2; j < (y + h + 4); ++j){
      for(i = x - 2; i < (x + w + 4); ++i){
        if(roomAndPath[i][j] == ROOM || roomAndPath[i][j] == EDGE){
	  isEmpty = 0;
	}
      }
    }
    if(isEmpty == 1){
      for(j = y; j < (y + h); ++j){
	for(i = x; i < (x + w); ++i){
          roomAndPath[i][j] = ROOM;
          hardness[i][j] = 0;
	}
      }
      corners[r][0] = x + (rand() % w);
      corners[r][1] = y + (rand() % h);
      roomDimensions[r][0] = x;
      roomDimensions[r][1] = y;
      roomDimensions[r][2] = w;
      roomDimensions[r][3] = h;
      ++r;
    }
  }
  if(dungeonLevel == 0){
    addTrees(50);
    makeWalls();
  }
  else if(dungeonLevel == secondBossLevel){
    pathDijkstra(corners,path_type);
    for(i = 1; i < WIDTH - 1; ++i){
      for(j = 1; j < HEIGHT - 1; ++j){
        hardness[i][j] = 0;
      }
    }
  }
  else if(dungeonLevel < 0){
    if(bunker == 0){
      if(dungeonLevel == firstBossLevel){
        path_type[0] = 200;
        path_type[1] = 1;
        path_type[2] = 15;
        path_type[3] = 50;
      }
      pathDijkstra(corners,path_type);
    }
    else if(bunker == 1){
      drawPaths(corners);
    }
    path_type[0] = 1;
    path_type[1] = 1;
    path_type[2] = 2;
    path_type[3] = 3;
  }
  else if(dungeonLevel > 0){
    addTrees(10);
  }
}

void generateRock(){
  int i,j;
  for(i = 0; i < WIDTH; ++i){
    for(j = 0; j < HEIGHT; ++j){
      hardness[i][j] = 0;
      roomAndPath[i][j] = (char) 0;
      seen[i][j] = HIDDEN;
      if(dungeonLevel == 0){
        grassPattern[i][j] = rand() % 8;
      }
    }
  }
  for(i = 0; i < WIDTH; ++i){
    hardness[i][0] = HARDEST;
    hardness[i][HEIGHT - 1] = HARDEST;
    roomAndPath[i][0] = EDGE;
    roomAndPath[i][HEIGHT - 1] = EDGE;
  }
  for(j = 0; j < HEIGHT; ++j){
    hardness[0][j] = HARDEST;
    hardness[WIDTH - 1][j] = HARDEST;
    roomAndPath[0][j] = EDGE;
    roomAndPath[WIDTH - 1][j] = EDGE;
  }
  for(j = 1; j < HEIGHT - 1; ++j){
    for(i = 1; i < WIDTH - 1; ++i){
      if(dungeonLevel >= 0){
        hardness[i][j] = 0;
      }
      else{
        hardness[i][j] = (rand() % 254) + 1;
        roomAndPath[i][j] = ROCK;
      }
    }
  }
}

void randomDungeon(){
  generateRock();
  generateRooms();
}

void writeToFile(){
  FILE *file;
  int version = 0;
  stringstream filePath;
  filePath << getenv("HOME") << "/.rlg327/dungeon" << dungeonLevel;
  int i,j;
  roomNumber = 0;
  for(i = 0; i < 100; ++i){
    if(roomDimensions[i][0] != 0){
      ++roomNumber;
    }
  }
  if(dungeonLevel == secondBossLevel){
    for(i = 1; i < WIDTH - 1; ++i){
      for(j = 1; j < HEIGHT - 1; ++j){
        if(roomAndPath[i][j] != ROOM && roomAndPath[i][j] != PATH){
          hardness[i][j] = 1;
        }
      }
    }
  }
  int fileSize = 16820 + (4 * roomNumber);
  int endian;
  if(!(file = fopen(filePath.str().c_str(), "w"))){
    fprintf(stderr,"Failed to open file.\n");
    return;
  }
  endian = htobe32(version);
  fwrite(&endian, 4, 1, file);
  endian = htobe32(fileSize);
  fwrite(&endian,4,1,file);
  endian = htobe32(numObjs);
  fwrite(&endian,4,1,file);
  endian = htobe32(pcX);
  fwrite(&endian,4,1,file);
  if(staircases[pcX][pcY] == UP_STAIRCASE && dungeonLevel >= 0){
    ++pcY;
  }
  if(staircases[pcX][pcY] == DOWN_STAIRCASE && dungeonLevel > 0){
    --pcY;
  }
  endian = htobe32(pcY);
  fwrite(&endian,4,1,file);
  endian = htobe32(monsleft);
  fwrite(&endian,4,1,file);
  for(i = 0; i < HEIGHT; ++i){
    for(j = 0; j < WIDTH; ++j){
      fwrite(&staircases[j][i],1,1,file);
    }
  }
  for(i = 0; i < 100; ++i){
    if(roomDimensions[i][0] != 0){
      fwrite(&roomDimensions[i][0],1,1,file);
      fwrite(&roomDimensions[i][1],1,1,file);
      fwrite(&roomDimensions[i][2],1,1,file);
      fwrite(&roomDimensions[i][3],1,1,file);
    }
  }
  for(i = 0; i < HEIGHT; ++i){
    for(j = 0; j < WIDTH; ++j){
      fwrite(&hardness[j][i],1,1,file);
    }
  }
  for(i = 0; i < HEIGHT; ++i){
    for(j = 0; j < WIDTH; ++j){
      fwrite(&seen[j][i],1,1,file);
    }
  }

  fclose(file);
}

int readFromFile(){
  FILE *file;
  int i,j,r;
  for(i = 0; i < HEIGHT; ++i){
    for(j = 0; j < WIDTH; ++j){
      roomAndPath[j][i] = ROCK;
      seen[i][j] = HIDDEN;
    }
  }
  int monNum = 0;
  stringstream filePath;
  filePath << getenv("HOME") << "/.rlg327/dungeon" << dungeonLevel;
  unsigned char x,y,w,h;
  char *useless[4];
  int endian = 0;
  int fileSize = 0;
  if(!(file = fopen(filePath.str().c_str(), "r"))){
    fprintf(stderr,"Failed to open file.\n");
    return monNum;
  }
  fread(&useless,4,1,file);
  fread(&endian,4,1,file);

  fileSize = be32toh(endian);
  int rooms = (fileSize - 16820) / 4;

  fread(&endian,4,1,file);
  numObjs = be32toh(endian);

  fread(&endian,4,1,file);
  pcX = be32toh(endian);

  fread(&endian,4,1,file);
  pcY = be32toh(endian);

  fread(&endian,4,1,file);
  monNum = be32toh(endian);

  for(i = 0; i < HEIGHT; ++i){
    for(j = 0; j < WIDTH; ++j){
      fread(&staircases[j][i],1,1,file);
    }
  }
  for(r = 0; r < rooms; ++r){
    fread(&x,1,1,file);
    fread(&y,1,1,file);
    fread(&w,1,1,file);
    fread(&h,1,1,file);

    roomDimensions[r][0] = x;
    roomDimensions[r][1] = y;
    roomDimensions[r][2] = w;
    roomDimensions[r][3] = h;

    for(j = y; j < (y + h); ++j){
      for(i = x; i < (x + w); ++i){
	roomAndPath[i][j] = ROOM;
      }
    }
  }
  for(i = 0; i < HEIGHT; ++i){
    for(j = 0; j < WIDTH; ++j){
      fread(&hardness[j][i],1,1,file);
    }
  }
  for(i = 0; i < HEIGHT; ++i){
    for(j = 0; j < WIDTH; ++j){
      fread(&seen[j][i],1,1,file);
    }
  }
  roomNumber = rooms;
  for(i = 0; i < HEIGHT; ++i){
    for(j = 0; j < WIDTH; ++j){
      if(hardness[j][i] > 0 && hardness[j][i] != HARDEST){
        roomAndPath[j][i] = ROCK;
      }
      if(hardness[j][i] == HARDEST){
        roomAndPath[j][i] = EDGE;
      }
    }
  }
  for(i = 0; i < HEIGHT; ++i){
    for(j = 0; j < WIDTH; ++j){
      if(hardness[j][i] == 0 && roomAndPath[j][i] != ROOM){
        roomAndPath[j][i] = PATH;
      }
    }
  }
  if(dungeonLevel == secondBossLevel){
    for(i = 1; i < WIDTH - 1; ++i){
      for(j = 1; j < HEIGHT - 1; ++j){
        hardness[i][j] = 0;
      }
    }
  }
  fclose(file);
  return monNum;
}

void printDungeon(){
  int i,j;
  for(j = 0; j < HEIGHT; ++j){
    for(i = 0; i < WIDTH; ++i){
      if(roomAndPath[i][j] == EDGE){
	printf(" ");
      }
      else if(characterMap[i][j] != (char) 0){
	printf("%c",characterMap[i][j]);
      }
      else{
	printf("%c",roomAndPath[i][j]);
      }
    }
    printf("\n");
  }
}

/*void makeDungeonEntrance(){
  int botStair = (rand() % (WIDTH - 10)) + 5;
  staircases[botStair][HEIGHT - 3] = DOWN_STAIRCASE;
  hardness[botStair][HEIGHT - 2] = 0;
  hardness[botStair][HEIGHT - 3] = 0;
  hardness[botStair][HEIGHT - 4] = 0;
  grassPattern[botStair][HEIGHT - 4] = 9;
  hardness[botStair][HEIGHT - 5] = 0;
  hardness[botStair][HEIGHT - 1] = DUNG_ENTRANCE;
  hardness[botStair][HEIGHT - 2] = DUNG_ENTRANCE;
  hardness[botStair + 1][HEIGHT - 2] = DUNG_ENTRANCE;
  hardness[botStair - 1][HEIGHT - 2] = DUNG_ENTRANCE;
  hardness[botStair + 1][HEIGHT - 1] = DUNG_ENTRANCE;
  hardness[botStair - 1][HEIGHT - 1] = DUNG_ENTRANCE;
  hardness[botStair + 1][HEIGHT - 3] = DUNG_ENTRANCE;
  hardness[botStair - 1][HEIGHT - 3] = DUNG_ENTRANCE;
  hardness[botStair + 1][HEIGHT - 4] = DUNG_ENTRANCE;
  hardness[botStair - 1][HEIGHT - 4] = DUNG_ENTRANCE;
  hardness[botStair + 2][HEIGHT - 2] = DUNG_ENTRANCE;
  hardness[botStair - 2][HEIGHT - 2] = DUNG_ENTRANCE;
  hardness[botStair + 3][HEIGHT - 2] = DUNG_ENTRANCE;
  hardness[botStair - 3][HEIGHT - 2] = DUNG_ENTRANCE;
  hardness[botStair + 2][HEIGHT - 1] = DUNG_ENTRANCE;
  hardness[botStair - 2][HEIGHT - 1] = DUNG_ENTRANCE;
  hardness[botStair + 4][HEIGHT - 1] = DUNG_ENTRANCE;
  hardness[botStair - 4][HEIGHT - 1] = DUNG_ENTRANCE;
  hardness[botStair + 4][HEIGHT - 2] = DUNG_ENTRANCE;
  hardness[botStair - 4][HEIGHT - 2] = DUNG_ENTRANCE;
  hardness[botStair + 3][HEIGHT - 1] = DUNG_ENTRANCE;
  hardness[botStair - 3][HEIGHT - 1] = DUNG_ENTRANCE;
  hardness[botStair + 2][HEIGHT - 3] = DUNG_ENTRANCE;
  hardness[botStair - 2][HEIGHT - 3] = DUNG_ENTRANCE;
  hardness[botStair + 3][HEIGHT - 3] = DUNG_ENTRANCE;
  hardness[botStair - 3][HEIGHT - 3] = DUNG_ENTRANCE;
  hardness[botStair + 2][HEIGHT - 4] = DUNG_ENTRANCE;
  hardness[botStair - 2][HEIGHT - 4] = DUNG_ENTRANCE;
}*/

void placePC(int pcPlaced){
  int i,j;

  for(j = 0; j < HEIGHT; ++j){
    for(i = 0; i < WIDTH; ++i){
      characterMap[i][j] = (char) 0;
      staircases[i][j] = (char) 0;
    }
  }
  if(pcPlaced == 0){
    pcX = (rand() % (roomDimensions[0][2] - 2)) + roomDimensions[0][0] + 1;
    pcY = (rand() % (roomDimensions[0][3] - 2)) + roomDimensions[0][1] + 1;
    characterMap[pcX][pcY] = PC;
    hardness[pcX][pcY] = 0;
    if(dungeonLevel == 0){
      int topStair = (rand() % (WIDTH - 10)) + 5;
      staircases[topStair][0] = UP_STAIRCASE;
      hardness[topStair][0] = 0;
      hardness[topStair][1] = 0;
      staircases[pcX][pcY] = DOWN_STAIRCASE;
/*      staircases[(rand()%(roomDimensions[roomNumber-1][2]-2))+roomDimensions[roomNumber-1][0]+1]
	   [(rand()%(roomDimensions[roomNumber-1][3]-2))+roomDimensions[roomNumber-1][1]+1]
		  = DOWN_STAIRCASE;*/
    }
    else if(dungeonLevel > 0){
      int topStair = (rand() % (WIDTH - 10)) + 5;
      staircases[topStair][0] = UP_STAIRCASE;
      hardness[topStair][0] = 0;
      hardness[topStair][1] = 0;
      int bottomStair = (rand() % (WIDTH - 10)) + 5;
      staircases[bottomStair][HEIGHT - 1] = DOWN_STAIRCASE;
      hardness[bottomStair][HEIGHT - 1] = 0;
      hardness[bottomStair][HEIGHT - 2] = 0;
      pcX = bottomStair;
      pcY = HEIGHT - 2;
    }
    else if(dungeonLevel < 0){
      staircases[pcX][pcY] = UP_STAIRCASE;
      if(!isBossLevel()){
        staircases[(rand()%(roomDimensions[roomNumber-1][2]-2))+roomDimensions[roomNumber-1][0]+1]
	   [(rand()%(roomDimensions[roomNumber-1][3]-2))+roomDimensions[roomNumber-1][1]+1]
		  = DOWN_STAIRCASE;
      }
    }
  }
  else{
    if(roomAndPath[pcX][pcY] == ROOM || roomAndPath[pcX][pcY] == PATH){
      characterMap[pcX][pcY] = PC;
    }
    else{
      pcX = (rand() % roomDimensions[0][2]) + roomDimensions[0][0];
      pcY = (rand() % roomDimensions[0][3]) + roomDimensions[0][1];
      characterMap[pcX][pcY] = PC;
    }
  }
 
}

void printAllDistances(){
  int i,j;
  for(j = 0; j < HEIGHT; ++j){
    for(i = 0; i < WIDTH; ++i){
      if(i == pcX && j == pcY){
        printf("%c",PC);
      }
      else if(all_distances[i][j] == INT_MAX){
        printf(" ");
      }
      else{
        printf("%d", all_distances[i][j] % 10);
      }
    }
    printf("\n");
  }
}

void printrpDistances(){
  int i,j;
  for(j = 0; j < HEIGHT; ++j){
    for(i = 0; i < WIDTH; ++i){
      if(i == pcX && j == pcY){
        printf("%c",PC);
      }
      else if(rp_distances[i][j] == INT_MAX){
        printf(" ");
      }
      else{
	printf("%d", rp_distances[i][j] % 10);
      }
    }
    printf("\n");
  }
}

bool isBossLevel(){
  return dungeonLevel == secondBossLevel || dungeonLevel == firstBossLevel;
}

void printLevel(){
  move(0,0);
  clrtoeol();
  attron(COLOR_PAIR(ILUM));
  if(dungeonLevel == 0){
    printw("%s VILLAGE",villageName.c_str());
  }
  else if(isBossLevel()){
    if(boss != 0){
      printw("%s ", boss->name.c_str());
      for(int i = 0; i < 20; ++i){
        if(boss->hp >= (boss->hpmax / 20) * i){
	  attron(COLOR_PAIR(MONST));
        }
        else{
	  attron(COLOR_PAIR(DARK));
        }
        addch(ACS_CKBOARD);
      }
    }
    else{
      printw("DUNGEON DEPTH %d",abs(dungeonLevel));
    }
  }
  else if(dungeonLevel > 0){
    printw("NORTHERN WOODS");
  }
  else if(dungeonLevel < 0){
    printw("DUNGEON DEPTH %d",abs(dungeonLevel));
  }
  attron(COLOR_PAIR(ILUM));
}

void secondBossPrint(int lookX, int lookY){
  int i,j;
  move(1,0);
  attron(COLOR_PAIR(ILUM));
  for(j = lookY - (NCURSES_HEIGHT / 2); j <= lookY + (NCURSES_HEIGHT / 2); ++j){
    for(i = lookX - (NCURSES_WIDTH / 2); i < lookX + (NCURSES_WIDTH / 2) - 1; ++i){
      if(i > 0 && i < WIDTH - 1 && j > 0 && j < HEIGHT - 1){
	if(i == lookX && j == lookY && (i != pcX || j != pcY)){
	  if(characterMap[i][j] != (char) 0 && seen[i][j] == VISIBLE){
	    if(main_character->status == DEAD){
	      attron(COLOR_PAIR(MONST));
	      addch('X');
	      attron(COLOR_PAIR(DARK));
	    }
	    else{
	      attron(COLOR_PAIR(ILUM));
  	      addch('*');
	      attroff(COLOR_PAIR(ILUM));
              attron(COLOR_PAIR(DARK));
	    }
          }
	  else if(objdun[i][j] != 0 || mondun[i][j] != 0){
	    attron(COLOR_PAIR(ILUM));
  	    addch('*');
	    attroff(COLOR_PAIR(ILUM));
            attron(COLOR_PAIR(DARK));
	  }
	  else{
	    attron(COLOR_PAIR(ILUM));
  	    addch('X');
	    attroff(COLOR_PAIR(ILUM));
            attron(COLOR_PAIR(DARK));
	  }
	}
        else if(i == pcX && j == pcY){
	  main_character->setColor();
  	  addch(characterMap[i][j]);
          attron(COLOR_PAIR(ILUM));
        }
	else if(characterMap[i][j] != (char) 0){
	  mondun[i][j]->setColor();
          addch(characterMap[i][j]);
        }
	else if(objectMap[i][j] != (char) 0){
	  objdun[i][j]->setColor();
	  if(objdun[i][j]->next == 0){
	    addch(objectMap[i][j]);
	  }
	  else{
	    addch(STACK_CHAR);
	  }
        }
        else if(staircases[i][j] != (char) 0){
          attron(COLOR_PAIR(ILUM));
          addch(staircases[i][j]);
        }
        else if(roomAndPath[i][j] == ROOM || roomAndPath[i][j] == PATH){
	  attron(COLOR_PAIR(LAV_ROCK));
	  addch(roomAndPath[i][j]);
	  attron(COLOR_PAIR(ILUM));
        }
        else{
	  int fire = rand() % 6;
          if(fire == 0){
	    attron(COLOR_PAIR(Y_LAV));
            addch('\%');
          }
          else if(fire == 1){
	    attron(COLOR_PAIR(Y_LAV));
            addch('(');
          }
          else if(fire == 2){
	    attron(COLOR_PAIR(LAV));
	    addch('\%');
	  }
          else if(fire == 3){
	    attron(COLOR_PAIR(LAV));
	    addch(')');
	  }
          else if(fire == 4){
	    attron(COLOR_PAIR(LAV));
	    addch('#');
	  }
          else if(fire == 5){
	    attron(COLOR_PAIR(LAV));
	    addch('(');
	  }
        }
      }
      else{
	attron(COLOR_PAIR(DARK));
        addch(ROCK_CHAR);
      }
    }
    printw("\n");
  }
}

void villagePrint(int lookX, int lookY){
  int i,j;
  move(1,0);
  attron(COLOR_PAIR(ILUM));
  for(j = lookY - (NCURSES_HEIGHT / 2); j <= lookY + (NCURSES_HEIGHT / 2); ++j){
    for(i = lookX - (NCURSES_WIDTH / 2); i < lookX + (NCURSES_WIDTH / 2) - 1; ++i){
      if(i > 0 && i < WIDTH - 1 && j > 0 && j < HEIGHT - 1){
	if(i == lookX && j == lookY && (i != pcX || j != pcY)){
	  if(characterMap[i][j] != (char) 0 && seen[i][j] == VISIBLE){
	    if(main_character->status == DEAD){
	      attron(COLOR_PAIR(MONST));
	      addch('X');
	      attron(COLOR_PAIR(DARK));
	    }
	    else{
	      attron(COLOR_PAIR(ILUM));
  	      addch('*');
	      attroff(COLOR_PAIR(ILUM));
              attron(COLOR_PAIR(DARK));
	    }
          }
	  else if(objdun[i][j] != 0 || mondun[i][j] != 0){
	    attron(COLOR_PAIR(ILUM));
  	    addch('*');
	    attroff(COLOR_PAIR(ILUM));
            attron(COLOR_PAIR(DARK));
	  }
	  else{
	    attron(COLOR_PAIR(ILUM));
  	    addch('X');
	    attroff(COLOR_PAIR(ILUM));
            attron(COLOR_PAIR(DARK));
	  }
	}
        else if(i == pcX && j == pcY){
	  main_character->setColor();
  	  addch(characterMap[i][j]);
          attron(COLOR_PAIR(ILUM));
        }
	else if(characterMap[i][j] != (char) 0){
	  mondun[i][j]->setColor();
          addch(characterMap[i][j]);
        }
	else if(objectMap[i][j] != (char) 0){
	  objdun[i][j]->setColor();
	  if(objdun[i][j]->next == 0){
	    addch(objectMap[i][j]);
	  }
	  else{
	    addch(STACK_CHAR);
	  }
        }
        else if(staircases[i][j] != (char) 0){
          attron(COLOR_PAIR(ILUM));
          addch(staircases[i][j]);
        }
        else if(hardness[i][j] == VERT_WALL){
          attron(COLOR_PAIR(R_ILUM));
	  addch(ACS_VLINE);
          attron(COLOR_PAIR(ILUM));
        }
        else if(hardness[i][j] == HORZ_WALL){
          attron(COLOR_PAIR(R_ILUM));
	  addch(ACS_HLINE);
          attron(COLOR_PAIR(ILUM));
        }
        else if(hardness[i][j] == TOP_R_CORNER){
          attron(COLOR_PAIR(R_ILUM));
	  addch(ACS_URCORNER);
          attron(COLOR_PAIR(ILUM));
        }
        else if(hardness[i][j] == TOP_L_CORNER){
          attron(COLOR_PAIR(R_ILUM));
	  addch(ACS_ULCORNER);
          attron(COLOR_PAIR(ILUM));
        }
        else if(hardness[i][j] == BOT_R_CORNER){
          attron(COLOR_PAIR(R_ILUM));
	  addch(ACS_LRCORNER);
          attron(COLOR_PAIR(ILUM));
        }
        else if(hardness[i][j] == BOT_L_CORNER){
          attron(COLOR_PAIR(R_ILUM));
	  addch(ACS_LLCORNER);
          attron(COLOR_PAIR(ILUM));
        }
        else if(hardness[i][j] == TREE_HARD){
          attron(COLOR_PAIR(GRAS));
	  addch(TREE_CHAR);
	  attroff(COLOR_PAIR(GRAS));
          attron(COLOR_PAIR(ILUM));
        }
        else if(hardness[i][j] == 0 && roomAndPath[i][j] != ROOM){
	  int grass = grassPattern[i][j];
          if(grass == 8){
            attron(COLOR_PAIR(R_ILUM));
	    addch('+');
            attron(COLOR_PAIR(ILUM));
	  }
	  else if(grass == 9){
	    attron(COLOR_PAIR(R_DARK));
	    addch('+');
	    attron(COLOR_PAIR(ILUM));
	  }
	  else if(grass >= 4){
	    attron(COLOR_PAIR(GRAS));
	    addch(grassTypes[grass - 4]);
	    attron(COLOR_PAIR(ILUM));
	  }
	  else if(grass < 4){
	    attron(COLOR_PAIR(D_GRAS));
	    addch(grassTypes[grass]);
	    attron(COLOR_PAIR(ILUM));
	  }
        }
        else if(roomAndPath[i][j] == ROOM){
	  attron(COLOR_PAIR(ILUM));
	  addch(roomAndPath[i][j]);
	  attron(COLOR_PAIR(ILUM));
        }
      }
      else if(j == 0 && i > 0 && i < WIDTH && staircases[i][j] == UP_STAIRCASE){
	attron(COLOR_PAIR(ILUM));
	addch(staircases[i][j]);
      }
      else{
	int color = rand() % 5;
	if(color == 0){
          attron(COLOR_PAIR(R_DARK));
	  addch(TREE_CHAR);
	  attroff(COLOR_PAIR(R_DARK));
          attron(COLOR_PAIR(ILUM));
	}
	else{
          attron(COLOR_PAIR(D_GRAS));
	  addch(TREE_CHAR);
	  attroff(COLOR_PAIR(D_GRAS));
          attron(COLOR_PAIR(ILUM));
	}
      }
    }
    printw("\n");
  }
}

void forestPrint(int lookX, int lookY){
  int i,j;
  move(1,0);
  fogOfWar(FOREST_SIGHT_RADIUS);
  attron(COLOR_PAIR(DARK));
  for(j = lookY - (NCURSES_HEIGHT / 2); j <= lookY + (NCURSES_HEIGHT / 2); ++j){
    for(i = lookX - (NCURSES_WIDTH / 2); i < lookX + (NCURSES_WIDTH / 2) - 1; ++i){
      if(i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT){
	if(i == lookX && j == lookY && characterMap[i][j] != PC){
	  if((characterMap[i][j] != (char) 0 || objectMap[i][j] != (char) 0) && seen[i][j] == VISIBLE){
	    if(main_character->status == DEAD){
	      attron(COLOR_PAIR(MONST));
	      addch('X');
	      attron(COLOR_PAIR(DARK));
	    }
	    else{
	      attron(COLOR_PAIR(ILUM));
  	      addch('*');
	      attroff(COLOR_PAIR(ILUM));
              attron(COLOR_PAIR(DARK));
	    }
          }
	  else if(objectMap[i][j] != (char) 0 && seen[i][j] == SEEN){
	    attron(COLOR_PAIR(ILUM));
  	    addch('*');
	    attroff(COLOR_PAIR(ILUM));
            attron(COLOR_PAIR(DARK));
	  }
	  else{
	    attron(COLOR_PAIR(ILUM));
  	    addch('X');
	    attroff(COLOR_PAIR(ILUM));
            attron(COLOR_PAIR(DARK));
	  }
	}
        else if(seen[i][j] == VISIBLE || seen[i][j] == SEEN){
          if(seen[i][j] == VISIBLE){
	    attron(COLOR_PAIR(ILUM));
	    if(characterMap[i][j] != (char) 0 && characterMap[i][j] != PC){
	      mondun[i][j]->setColor();
  	      addch(characterMap[i][j]);
              attron(COLOR_PAIR(DARK));
	    }
	    else if(characterMap[i][j] == PC){
	      main_character->setColor();
  	      addch(characterMap[i][j]);
              attron(COLOR_PAIR(DARK));
	    }
	    else if(staircases[i][j] != (char) 0){
	      addch(staircases[i][j]);
            }
	    else if(objectMap[i][j] != (char) 0){
	      objdun[i][j]->setColor();
	      if(objdun[i][j]->next == 0){
	        addch(objectMap[i][j]);
	      }
	      else{
	        addch(STACK_CHAR);
	      }
            }
	    else if(hardness[i][j] == TREE_HARD){
	      attron(COLOR_PAIR(GRAS));
	      addch(TREE_CHAR);
	      attroff(COLOR_PAIR(GRAS));
	      attron(COLOR_PAIR(DARK));
            }
            else if(hardness[i][j] == HARDEST && seen[i][j] != HIDDEN){
	      attron(COLOR_PAIR(GRAS));
	      addch(TREE_CHAR);
	      attron(COLOR_PAIR(DARK));
            }
	    else{
	      int grass = grassPattern[i][j];
              if(grass == 6 || grass == 8 || grass == 9){
		attron(COLOR_PAIR(R_ILUM));
		addch(ROOM);
	      }
	      else if(grass >= 4){
		attron(COLOR_PAIR(GRAS));
		addch(ROOM);
		attron(COLOR_PAIR(DARK));
	      }
	      else{
	        attron(COLOR_PAIR(GRAS));
	        addch(grassTypes[grass]);
	        attron(COLOR_PAIR(DARK));
	      }
	    }
	    seen[i][j] = SEEN;
	    refresh();
	    attroff(COLOR_PAIR(ILUM));
	    attron(COLOR_PAIR(DARK));
          }
	  else if(staircases[i][j] != (char) 0){
	    addch(staircases[i][j]);
          }
	  else if(objectMap[i][j] != (char) 0){
	    if(objdun[i][j]->next == 0){
	      attron(COLOR_PAIR(DARK));
	      addch(objectMap[i][j]);
	    }
	    else{
	      attron(COLOR_PAIR(DARK));
	      addch(STACK_CHAR);
	    }
          }
	  else if(hardness[i][j] == TREE_HARD){
	    attron(COLOR_PAIR(D_GRAS));
	    addch(TREE_CHAR);
	    attroff(COLOR_PAIR(D_GRAS));
	    attron(COLOR_PAIR(DARK));
          }
          else if(hardness[i][j] == HARDEST && seen[i][j] != HIDDEN){
	    attron(COLOR_PAIR(D_GRAS));
	    addch(TREE_CHAR);
	    attron(COLOR_PAIR(DARK));
          }
          else{
	    int grass = grassPattern[i][j];
            if(grass == 6 || grass == 8){
	      attron(COLOR_PAIR(R_DARK));
	      addch(ROOM);
	    }
	    else if(grass >= 4){
	      attron(COLOR_PAIR(D_GRAS));
	      addch(ROOM);
	      attron(COLOR_PAIR(DARK));
	    }
	    else{
	      attron(COLOR_PAIR(D_GRAS));
	      addch(grassTypes[grass]);
	      attron(COLOR_PAIR(DARK));
	    }
          }
        }
        else if(hardness[i][j] == HARDEST && seen[i][j] != HIDDEN){
	  attron(COLOR_PAIR(D_GRAS));
	  addch(TREE_CHAR);
	  attron(COLOR_PAIR(DARK));
        }
	else{
	  addch(' ');
	}
      }
      else{
	addch(' ');
      }
    }
    printw("\n");
  }
}

void dungeonPrint(int lookX, int lookY){
  int i,j;
  move(1,0);
  fogOfWar(DUNGEON_SIGHT_RADIUS);
  attron(COLOR_PAIR(DARK));
  for(j = lookY - (NCURSES_HEIGHT / 2); j <= lookY + (NCURSES_HEIGHT / 2); ++j){
    for(i = lookX - (NCURSES_WIDTH / 2); i < lookX + (NCURSES_WIDTH / 2) - 1; ++i){
      if(i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT){
	if(i == lookX && j == lookY && characterMap[i][j] != PC){
	  if((characterMap[i][j] != (char) 0 || objectMap[i][j] != (char) 0) && seen[i][j] == VISIBLE){
	    if(main_character->status == DEAD){
	      attron(COLOR_PAIR(MONST));
	      addch('X');
	      attron(COLOR_PAIR(DARK));
	    }
	    else{
	      attron(COLOR_PAIR(ILUM));
  	      addch('*');
	      attroff(COLOR_PAIR(ILUM));
              attron(COLOR_PAIR(DARK));
	    }
          }
	  else if(objectMap[i][j] != (char) 0 && seen[i][j] == SEEN){
	    attron(COLOR_PAIR(ILUM));
  	    addch('*');
	    attroff(COLOR_PAIR(ILUM));
            attron(COLOR_PAIR(DARK));
	  }
	  else{
	    attron(COLOR_PAIR(ILUM));
  	    addch('X');
	    attroff(COLOR_PAIR(ILUM));
            attron(COLOR_PAIR(DARK));
	  }
	}
        else if(seen[i][j] == VISIBLE || seen[i][j] == SEEN){
          if(seen[i][j] == VISIBLE){
	    attron(COLOR_PAIR(ILUM));
	    if(characterMap[i][j] != (char) 0 && characterMap[i][j] != PC){
	      mondun[i][j]->setColor();
  	      addch(characterMap[i][j]);
              attron(COLOR_PAIR(DARK));
	    }
	    else if(characterMap[i][j] == PC){
	      main_character->setColor();
  	      addch(characterMap[i][j]);
              attron(COLOR_PAIR(DARK));
	    }
	    else if(staircases[i][j] != (char) 0){
	      addch(staircases[i][j]);
            }
	    else if(objectMap[i][j] != (char) 0){
	      objdun[i][j]->setColor();
	      if(objdun[i][j]->next == 0){
	        addch(objectMap[i][j]);
	      }
	      else{
	        addch(STACK_CHAR);
	      }
            }
	    else if(hardness[i][j] != 0){
	      attron(COLOR_PAIR(R_ILUM));
	      addch(ROCK_CHAR);
	      attroff(COLOR_PAIR(R_ILUM));
	      attron(COLOR_PAIR(DARK));
            }
	    else{
	      addch(roomAndPath[i][j]);
	    }
	    seen[i][j] = SEEN;
	    refresh();
	    attroff(COLOR_PAIR(ILUM));
	    attron(COLOR_PAIR(DARK));
          }
	  else if(staircases[i][j] != (char) 0){
	    addch(staircases[i][j]);
          }
	  else if(objectMap[i][j] != (char) 0){
	    if(objdun[i][j]->next == 0){
	      attron(COLOR_PAIR(DARK));
	      addch(objectMap[i][j]);
	    }
	    else{
	      attron(COLOR_PAIR(DARK));
	      addch(STACK_CHAR);
	    }
          }
	  else if(hardness[i][j] != 0){
	    attron(COLOR_PAIR(R_DARK));
	    addch(ROCK_CHAR);
	    attroff(COLOR_PAIR(R_DARK));
	    attron(COLOR_PAIR(DARK));
          }
          else{
	    addch(roomAndPath[i][j]);
          }
        }
	else{
	  addch(' ');
	}
      }
      else{
	addch(' ');
      }
    }
    printw("\n");
  }
}

void ncursesPrint(int lookX, int lookY){
  if(dungeonLevel == 0){
    villagePrint(lookX,lookY);
  }
  else if(dungeonLevel == secondBossLevel){
    secondBossPrint(lookX,lookY);
  }
  else if(dungeonLevel < 0){
    dungeonPrint(lookX,lookY);
  }
  else if(dungeonLevel > 0){
    forestPrint(lookX,lookY);
  }
  main_character->printStats();
  attron(COLOR_PAIR(ILUM));
}

void runGame(int numMon){

  int monNum = numMon;
  numObjs = 15 + (rand() % 10);
  placePC(0);
  fullDijkstra(pcX,pcY);
  rpDijkstra(pcX,pcY);
  if(dungeonLevel == 0){
    printLevel();
    simulateGame(20);
  }
  while(nextDungeon == 1){
    if(dungeonLevel > highestReached){
      nextDungeon = 0;
      ++highestReached;
      monNum = ((difficulty + 1) * dungeonLevel) + diffType[difficulty] + 
	       (rand() % (dungeonLevel + 1)) - dungeonLevel;
      numObjs = monNum;
      randomDungeon();
      placePC(0);
      fullDijkstra(pcX,pcY);
      rpDijkstra(pcX,pcY);
      printLevel();
      simulateGame(monNum);
    }
    else if(dungeonLevel < lowestReached){
      nextDungeon = 0;
      --lowestReached;
      if(dungeonLevel == secondBossLevel){
        monNum = 1;
      }
      else{
        monNum = ((difficulty + 1) * -1 * dungeonLevel) + diffType[difficulty] + 
	         (rand() % ((-1 * dungeonLevel) + 1)) + dungeonLevel;
      }
      numObjs = monNum;
      if(dungeonLevel == secondBossLevel){
        numObjs = 0;
      }
      randomDungeon();
      placePC(0);
      fullDijkstra(pcX,pcY);
      rpDijkstra(pcX,pcY);
      printLevel();
      simulateGame(monNum);
    }
    else{
      nextDungeon = 0;
      monNum = readFromFile();
      fullDijkstra(pcX,pcY);
      rpDijkstra(pcX,pcY);
      printLevel();
      simulateGame(monNum);
    }
  }
}

void commandArgs(int argc, char *argv[]){
 
  int loaded = 0;
  int saved = 0;
  int numMon = numRooms;
  int i;
  for(i = 1; i < argc; ++i){
    if(strcmp(argv[i],"--load") == 0){
      loaded = 1;
    }
    else if(strcmp(argv[i],"--save") == 0){
      saved = 1;
    }
    else if(strcmp(argv[i],"--bunker") == 0){
      bunker = 1;
    }
    else if(strcmp(argv[i],"--nummon") == 0 && i < argc - 1){
      numMon = atoi(argv[i + 1]); 
    }
    else if(strcmp(argv[i],"--crazy") == 0){
      path_type[0] = 200;
      path_type[1] = 1;
      path_type[2] = 15;
      path_type[3] = 50;
    }
    else if(strcmp(argv[i],"--tame") == 0){
      path_type[0] = 0;
      path_type[1] = 1;
      path_type[2] = 2;
      path_type[3] = 3;
    }
  }
  if(loaded == 1 && saved == 1){
    readFromFile();
    writeToFile();
  }
  if(loaded == 1 && saved == 0){
    readFromFile();
  }
  if(loaded == 0 && saved == 1){
    randomDungeon();
    writeToFile();
  }
  if(loaded == 0 && saved == 0){
    randomDungeon();
  }
  runGame(numMon);
}

void customGame(){
  move(0,0);
  printw("CUSTOM GAME\n");
  printw("DUNGEON TYPE:\n");
  printw("PRESS 1 FOR STANDARD\nPRESS 2 FOR TAME\nPRESS 3 FOR CRAZY\nPRESS 4 FOR BUNKER\n");
  refresh();
  int key = 0;
  int chosen = 0;
  int numMon;
  bunker = 0;
  while(chosen == 0){
    key = getch();
    if(key == KEY_1){
      chosen = 1;
    }
    else if(key == KEY_2){
      chosen = 1;
      path_type[0] = 0;
      path_type[1] = 1;
      path_type[2] = 2;
      path_type[3] = 3;
    }
    else if(key == KEY_3){
      chosen = 1;
      path_type[0] = 200;
      path_type[1] = 1;
      path_type[2] = 15;
      path_type[3] = 50;
    }
    else if(key == KEY_4){
      chosen = 1;
      bunker = 1;
    }
  }
  move(0,0);
  printw("CUSTOM GAME\n");
  printw("DIFFICULTY:\n");
  printw("PRESS 1 FOR STANDARD\nPRESS 2 FOR EASY\nPRESS 3 FOR HARD\nPRESS 4 FOR EXPERT\n");
  refresh();
  key = 0;
  chosen = 0;
  while(chosen == 0){
    key = getch();
    if(key == KEY_1){
      chosen = 1;
      difficulty = STD;
      numMon = STD_MONS;
    }
    else if(key == KEY_2){
      chosen = 1;
      difficulty = easy;
      numMon = EASY_MONS;
    }
    else if(key == KEY_3){
      chosen = 1;
      difficulty = hard;
      numMon = HARD_MONS;
    }
    else if(key == KEY_4){
      chosen = 1;
      difficulty = exp;
      numMon = EXP_MONS;
    }
  }
  dungeonLevel = 0;
  highestReached = 0;
  lowestReached = 0;
  nextDungeon = 0;
  randomDungeon();
  runGame(numMon);
}

void characterCreation(){
  clear();
  move(0,0);
  attron(COLOR_PAIR(ILUM));
  printw("SELECT CLASS\n\n");
  printw("1: ");
  attron(COLOR_PAIR(ILUM));
  addch(PC);
  attron(COLOR_PAIR(ILUM));
  printw(" - KNIGHT\n");
  printw("  VIT-14 STR-13 DEX-13 INT-7 END-9 DEF-14\n\n");
  printw("2: ");
  attron(COLOR_PAIR(R_ILUM));
  addch(PC);
  attron(COLOR_PAIR(ILUM));
  printw(" - BARBARIAN\n");
  printw("  VIT-14 STR-16 DEX-10 INT-4 END-11 DEF-10\n\n");
  printw("3: ");
  attron(COLOR_PAIR(GRAS));
  addch(PC);
  attron(COLOR_PAIR(ILUM));
  printw(" - ELF\n");
  printw("  VIT-9 STR-9 DEX-15 INT-12 END-14 DEF-7\n\n");
  printw("4: ");
  attron(COLOR_PAIR(BL));
  addch(PC);
  attron(COLOR_PAIR(ILUM));
  printw(" - MAGE\n");
  printw("  VIT-10 STR-7 DEX-9 INT-16 END-11 DEF-10\n\n");
  printw("5: ");
  attron(COLOR_PAIR(R_DARK));
  addch(PC);
  attron(COLOR_PAIR(ILUM));
  printw(" - ORC\n");
  printw("  VIT-16 STR-16 DEX-4 INT-2 END-6 DEF-16\n\n");
  printw("6: ");
  attron(COLOR_PAIR(YEL));
  addch(PC);
  attron(COLOR_PAIR(ILUM));
  printw(" - PEASANT\n");
  printw("  VIT-8 STR-8 DEX-8 INT-8 END-8 DEF-8\n\n");
  printw("7: ");
  attron(COLOR_PAIR(MONST));
  addch(PC);
  attron(COLOR_PAIR(ILUM));
  printw(" - AUTOBOT\n");
  printw("  VIT-100 STR-100 DEX-100 INT-100 END-100 DEF-100\n\n");
  int k;
  bool chosen = false;
  while(!chosen){
    if(k == KEY_1){          //KNIGHT
      main_character->vit = 14;
      main_character->str = 13;
      main_character->dex = 13;
      main_character->intel = 7;
      main_character->endur = 9;
      main_character->def = 14;
      main_character->color = ILUM;
      main_character->level = 10;
      main_character->speed = 10;
      main_character->gold = 50;
      initKnight();
      chosen = true;
    }
    else if(k == KEY_2){     //BARBARIAN
      main_character->vit = 14;
      main_character->str = 16;
      main_character->dex = 10;
      main_character->intel = 4;
      main_character->endur = 11;
      main_character->def = 10;
      main_character->color = R_ILUM;
      main_character->level = 10;
      main_character->speed = 10;
      main_character->gold = 10;
      initBarbarian();
      chosen = true;
    }
    else if(k == KEY_3){     //ELF
      main_character->vit = 9;
      main_character->str = 9;
      main_character->dex = 15;
      main_character->intel = 12;
      main_character->endur = 14;
      main_character->def = 7;
      main_character->color = GRAS;
      main_character->level = 10;
      main_character->speed = 14;
      main_character->ammo = 50;
      main_character->gold = 30;
      initElf();
      chosen = true;
    }
    else if(k == KEY_4){     //MAGE
      main_character->vit = 10;
      main_character->str = 7;
      main_character->dex = 9;
      main_character->intel = 16;
      main_character->endur = 11;
      main_character->def = 10;
      main_character->color = BL;
      main_character->level = 10;
      main_character->speed = 10;
      main_character->gold = 50;
      initMage();
      chosen = true;
    }
    else if(k == KEY_5){     //ORC
      main_character->vit = 16;
      main_character->str = 16;
      main_character->dex = 4;
      main_character->intel = 2;
      main_character->endur = 6;
      main_character->def = 16;
      main_character->color = R_DARK;
      main_character->level = 10;
      main_character->speed = 8;
      main_character->gold = 10;
      initOrc();
      chosen = true;
    }
    else if(k == KEY_6){     //PEASANT
      main_character->vit = 8;
      main_character->str = 8;
      main_character->dex = 8;
      main_character->intel =8;
      main_character->endur =8;
      main_character->def = 8;
      main_character->color = YEL;
      main_character->level = 0;
      main_character->speed = 8;
      main_character->gold = 3;
      initPeasant();
      chosen = true;
    }
    else if(k == KEY_7){     //AUTOBOT
      main_character->vit = 100;
      main_character->str = 100;
      main_character->dex = 100;
      main_character->intel = 100;
      main_character->endur = 100;
      main_character->def = 100;
      main_character->color = MONST;
      main_character->level = 100;
      main_character->speed = 20;
      main_character->ammo = 500;
      main_character->gold = 500;
      initAutobot();
      chosen = true;
    }
    else{
      k = getch();
    }
  }
  main_character->initStats();
  main_character->hp = main_character->hpmax;
  main_character->stamina = main_character->stammax;
  clear();
}

int levelDifficulty(){
  if(dungeonLevel < 0){
    return 20 + abs(dungeonLevel) * 15 + (abs(dungeonLevel) / 3) * 20;
  }
  else if(dungeonLevel > 0){
    return 70 + dungeonLevel * 10 + (dungeonLevel / 3) * 10;
  }
  return 0;
}

void chooseName(){
  clear();
  move(0,0);
  echo();
  printw("WHAT IS YOUR NAME?\n\n");
  string name;
  int ch = getch();
  int i = 0;
  while(ch != '\n'){
    if(i == 12) break;
    name.push_back(ch);
    ch = getch();
    ++i;
  }
  main_character->name = name;
  noecho();
}

void printControls(){
  move(0,0);
  clear();
  attron(COLOR_PAIR(ILUM));
  printw("CONTROLS: (SHIFT + Q to quit game)\n\n");
  printw("Use NUMPAD to move\n");
  printw("Press '>' to move downstairs and '<' to go up\n\n");
  printw("              INVENTORY\n");
  printw("e - show equipped items\n");
  printw("i - show inventory and inspect items\n");
  printw("w - equip item from inventory\n");
  printw("t - take off an equipped item and place in inventory\n");
  printw("d - drop an item from inventory onto floor\n");
  printw("x - expunge an item from the game\n");
  printw("c - consume an item from consumables menu\n");
  printw("r - use ranged item if equipped\n");
  printw("s - use scrolls\n");
  printw("l - level up stat if possible\n");
  printw("h - help with controls\n\n");
  printw("SHIFT + L to enter LOOK MODE\n");
  printw("i - inspect item or monster in LOOK MODE if possible\n");
  printw("\n");
  getch();
}

void printMainMenu(){
  clear();
  move(0,0);
  printw("\n  	  	    	       WELCOME TO\n");
  attron(COLOR_PAIR(GRAS));
  printw("		          VILLAGE IN THE WOODS\n");
  attron(COLOR_PAIR(ILUM));
  printw("		      (PRESS ANY KEY TO CONTINUE)\n\n\n\n");
  attron(COLOR_PAIR(ILUM));
  printw("     	                      .\"");
  for(int i = 0; i < 8; ++i){
    addch(ACS_S1);
  }
  printw("\".\n");
  printw("	                     /            \\\n");
  printw("	                    |              |\n");
  printw("	                    |,  .");
  addch(ACS_S7);
  printw(".  .");
  addch(ACS_S7);
  printw(".  ,|\n");
  printw("	                    | )(__/  \\__)( |\n");
  printw("	                    |/     /\\     \\|\n");
  attron(COLOR_PAIR(YEL));
  printw("	          (");
  attron(COLOR_PAIR(MONST));
  printw("@");
  attron(COLOR_PAIR(YEL));
  printw("_");
  attron(COLOR_PAIR(ILUM));
  printw("       (_     ^^     _)\n");
  attron(COLOR_PAIR(YEL));
  printw("	           ) \\");
  attron(COLOR_PAIR(DARK));
  printw("_______");
  attron(COLOR_PAIR(ILUM));
  printw("\\__|IIIIII|__/");
  attron(COLOR_PAIR(DARK));
  printw("__________________________\n");
  attron(COLOR_PAIR(YEL));
  printw("	    (");
  attron(COLOR_PAIR(MONST));
  printw("@");
  attron(COLOR_PAIR(YEL));
  printw(")");
  attron(COLOR_PAIR(R_DARK));
  printw("@");
  attron(COLOR_PAIR(R_ILUM));
  printw("8");
  attron(COLOR_PAIR(R_DARK));
  printw("@");
  attron(COLOR_PAIR(R_ILUM));
  printw("8");
  attron(COLOR_PAIR(YEL));
  printw("{}<");
  attron(COLOR_PAIR(DARK));
  printw("________");
  attron(COLOR_PAIR(ILUM));
  printw("|-\\IIIIII/-|");
  attron(COLOR_PAIR(DARK));
  printw("___________________________>\n");
  attron(COLOR_PAIR(YEL));
  printw("	           )_/        ");
  attron(COLOR_PAIR(ILUM));
  printw("\\          /\n");
  attron(COLOR_PAIR(YEL));
  printw("	          (");
  attron(COLOR_PAIR(MONST));
  printw("@           ");
  attron(COLOR_PAIR(ILUM));
  printw("`--------`\n"); 
  refresh();
  getch();
}

void homeScreen(){
  dungeonLevel = 0;
  highestReached = 0;
  lowestReached = 0;
  nextDungeon = 0;
  printMainMenu();
  clear();
  move(0,0);
  attron(COLOR_PAIR(ILUM));
  printw("PRESS 1 TO START YOUR ADVENTURE\n");
  printw("PRESS 2 FOR CONTROLS");
  refresh();
  boss = 0;
  int key = 0;
  int chosen = 0;
  int numMon = numRooms;
  while(chosen == 0){
    clear();
    move(0,0);
    printw("PRESS 1 TO START YOUR ADVENTURE\n");
    printw("PRESS 2 FOR CONTROLS");
    refresh();
    key = getch();
    if(key == KEY_1){
      chosen = 1;
      bunker = 0;
      path_type[0] = 1;
      path_type[1] = 1;
      path_type[2] = 2;
      path_type[3] = 3;
      villageName = villageNames[rand() % 13];
      boss = 0;
      firstBossLevel = FIRST_BOSS_LEVEL;//-1 * (rand() % 3) - 5;
      secondBossLevel = SECOND_BOSS_LEVEL;//-1 * (rand() % 3) - 3 + firstBossLevel;
      difficulty = STD;
      numMon = STD_MONS;
      chooseName();
      characterCreation();
      randomDungeon();
      runGame(numMon);
    }
    else if(key == KEY_2){
      printControls();
    }
  } 
}

void initializeDungeon(int argc, char *argv[]){

  diffType[easy] = EASY_MONS;
  diffType[STD] = STD_MONS;
  diffType[hard] = HARD_MONS;
  diffType[exp] = EXP_MONS;

  if(argc > 1){
    if(strcmp(argv[1],"--command") == 0){
      commandArgs(argc, argv);
    }
  }
  else{
    homeScreen();
  }
}

void seed(int argc, char *argv[]){
  int seed,i;
  int seeded = 0;
  for(i = 0; i < argc; ++i){
    if(strcmp(argv[i],"--seed") == 0 && i < argc - 1){
      seed = atoi(argv[i+1]);
      seeded = 1;
    }
  }
  if(argc == 1 || seeded == 0){
    seed = time(NULL);
  }
  srand(seed);
}

void setupColor(){
  start_color();
  init_color(COLOR_GRAY,500,500,500);
  init_color(COLOR_YELLOW,800,700,0);
  init_color(COLOR_DARK_GREEN,0,300,0);
  init_color(COLOR_GREEN,0,600,0);
  init_color(COLOR_BLACK,100,100,100);
  init_color(COLOR_BLUE,200,200,800);
  init_color(COLOR_CYAN,0,500,500);
  init_color(COLOR_RED,800,0,0);
  init_color(COLOR_WHITE,1000,1000,1000);
  init_color(COLOR_ROCK_DARK,400,200,40);
  init_color(COLOR_ROCK_ILUM,700,350,70);
  init_color(COLOR_MAGENTA,800,0,800);
  init_color(COLOR_RED_LAVA,800,100,100);
  init_color(COLOR_YELLOW_LAVA,900,600,0);
  init_pair(DARK,COLOR_GRAY,COLOR_BLACK);
  init_pair(ILUM,COLOR_WHITE,COLOR_BLACK);
  init_pair(R_DARK,COLOR_ROCK_DARK,COLOR_BLACK);
  init_pair(MONST,COLOR_RED,COLOR_BLACK);
  init_pair(R_ILUM,COLOR_ROCK_ILUM,COLOR_BLACK);
  init_pair(GRAS,COLOR_GREEN,COLOR_BLACK);
  init_pair(PC_COLOR,COLOR_YELLOW,COLOR_BLACK);
  init_pair(D_GRAS,COLOR_DARK_GREEN,COLOR_BLACK);
  init_pair(YEL,COLOR_YELLOW,COLOR_BLACK);
  init_pair(CY,COLOR_CYAN,COLOR_BLACK);
  init_pair(BL,COLOR_BLUE,COLOR_BLACK);
  init_pair(MAG,COLOR_MAGENTA,COLOR_BLACK);
  init_pair(LAV_ROCK,COLOR_GRAY,COLOR_BLACK);
  init_pair(LAV,COLOR_RED_LAVA,COLOR_BLACK);
  init_pair(Y_LAV,COLOR_YELLOW_LAVA,COLOR_BLACK);
  attron(COLOR_PAIR(ILUM));
  //attron(COLOR_WHITE);
}

int main(int argc, char *argv[]){
  
  dungeonFile << getenv("HOME") << "/.rlg327/dungeon";
  dungeonLevel = 0;
  nextDungeon = 0;
  highestReached = 0;
  lowestReached = 0;
  killed = 0;
  quit = 0;
  seed(argc,argv);
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  setupColor();
  main_character = new player();
  initializeDungeon(argc, argv);
  int key = 0;
  PLAY_AGAIN:
  move(0,0);
  if(quit == 1){
    goto QUIT;
  }
  getch();
  dungeonLevel = 0;
  highestReached = 0;
  lowestReached = 0;
  killed = 0;
  delete main_character;
  clrtoeol();
  printw("PLAY AGAIN? (y/n)");
  key = getch();
  while(key != KEY_n){
    if(key == KEY_y){
      main_character = new player();
      homeScreen();
      goto PLAY_AGAIN;
    }
    else{
      key = getch();
    }
  }
  QUIT:
  endwin();
  
  return 0;
}




