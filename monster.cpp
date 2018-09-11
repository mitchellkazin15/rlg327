#include "core.h"

int canSee(int x0, int y0, int x1, int y1){
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
  if(dx * dx + dy * dy > MON_SIGHT_RADIUS * MON_SIGHT_RADIUS){
    return 0;
  }
  for(;;){
    if(hardness[x0][y0] != 0){
      return 0;
    }
    if(x0==x1 && y0==y1){
      return 1;
    }
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
  return 0;
}

void monster::moveOnGradient(){
  int i,j,saveX,saveY;
  characterMap[xcoord][ycoord] = (char) 0;
  saveX = xcoord;
  saveY = ycoord;
  if(xcoord < pcX){
    ++saveX;
  }
  if(ycoord < pcY){
    ++saveY;
  }
  if(xcoord > pcX){
    --saveX;
  }
  if(ycoord > pcY){
    --saveY;
  }
  if(rp_distances[saveX][saveY] < rp_distances[xcoord][ycoord] && hardness[saveX][saveY] == 0){
    goto SKIP_LOOP;
  }
  for(i = xcoord - 1; i <= xcoord + 1;++i){
    for(j = ycoord - 1; j <= ycoord + 1; ++j){
      if(i < WIDTH - 1 && i > 0 && j < HEIGHT - 1 && j > 0){
        if(rp_distances[i][j] < rp_distances[xcoord][ycoord]){
	  saveX = i;
	  saveY = j;
	}
      }
    }
  }
  SKIP_LOOP:
  if(saveX == pcX && saveY == pcY){
    if(aggressive){
      this->attack();
    }
  }
  else{
    xcoord = saveX;
    ycoord = saveY;
  }
  lastSeenX = pcX;
  lastSeenY = pcY;
}

void monster::moveOnGradientTunneling(){
  int i,j,saveX,saveY;
  characterMap[xcoord][ycoord] = (char) 0;
  saveX = xcoord;
  saveY = ycoord;
  if(xcoord < pcX){
    ++saveX;
  }
  if(ycoord < pcY){
    ++saveY;
  }
  if(xcoord > pcX){
    --saveX;
  }
  if(ycoord > pcY){
    --saveY;
  }
  if(all_distances[saveX][saveY] < all_distances[xcoord][ycoord] && hardness[saveX][saveY] == 0){
    goto SKIP_LOOP;
  }
  for(i = xcoord - 1; i <= xcoord + 1;++i){
    for(j = ycoord - 1; j <= ycoord + 1; ++j){
      if(i < WIDTH - 1 && i > 0 && j < HEIGHT - 1 && j > 0){
        if(all_distances[i][j] < all_distances[xcoord][ycoord]){
	  saveX = i;
	  saveY = j;
	}
      }
    }
  }
  SKIP_LOOP:
  if(saveX == pcX && saveY == pcY){
    if(aggressive){
      this->attack();
    }
  }
  else if(hardness[saveX][saveY] == 0){
    xcoord = saveX;
    ycoord = saveY;
  }
  else if(hardness[saveX][saveY] > 0 && hardness[saveX][saveY] < ONE_HARD){
    xcoord = saveX;
    ycoord = saveY;
    hardness[saveX][saveY] = 0;
    roomAndPath[saveX][saveY] = PATH;
    fullDijkstra(pcX,pcY);
    rpDijkstra(pcX,pcY); 
  }
  else if(hardness[saveX][saveY] > ONE_HARD){
    hardness[saveX][saveY] = hardness[saveX][saveY] - ONE_HARD;
  }
  lastSeenX = pcX;
  lastSeenY = pcY;
}

//no attributes (random motion)
void monster::move_0(){
  characterMap[xcoord][ycoord] = (char) 0;
  int xcor = xcoord;
  int ycor = ycoord;
  xcor = xcor + ((rand() % 3) - 1);
  ycor = ycor + ((rand() % 3) - 1);
  while(hardness[xcor][ycor] != 0){
    xcor = xcoord;
    ycor = ycoord;
    xcor = xcor + ((rand() % 3) - 1);
    ycor = ycor + ((rand() % 3) - 1);
  }
  if(xcor == pcX && ycor == pcY){
    if(aggressive){
      this->attack();
    }
  }
  else{
    xcoord = xcor;
    ycoord = ycor;
  }
}

//only smart
void monster::move_1(){
  if(canSee(xcoord,ycoord,pcX,pcY) == 1){
    moveOnGradient();
    lastSeenX = pcX;
    lastSeenY = pcY;
  }
  else if(lastSeenX > 0 && lastSeenY > 0){
    rpDijkstra(lastSeenX, lastSeenY);
    moveOnGradient();
    rpDijkstra(pcX, pcY);
  }
  else{
    move_0();
  }
}

//only telepathic
void monster::move_2(){
  characterMap[xcoord][ycoord] = (char) 0;
  int xcor = xcoord;
  int ycor = ycoord;
  if(xcor < pcX){
    ++xcor;
  }
  if(ycor < pcY){
    ++ycor;
  }
  if(xcor > pcX){
    --xcor;
  }
  if(ycor > pcY){
    --ycor;
  }
  if(xcor == pcX && ycor == pcY){
    if(aggressive){
      this->attack();
    }
  }
  else if(hardness[xcor][ycor] == 0){
    xcoord = xcor;
    ycoord = ycor;
  }
  else if(hardness[xcor][ycor] != 0){
    if(hardness[xcoord][ycor] == 0){
      ycoord = ycor;
    }
    else if(hardness[xcor][ycoord] == 0){
      xcoord = xcor;
    }
  }
}

//smart + telepathic
void monster::move_3(){
  moveOnGradient();
}

//only tunneling
void monster::move_4(){
  if(canSee(xcoord,ycoord,pcX,pcY) == 0){
    characterMap[xcoord][ycoord] = (char) 0;
    int xcor = xcoord;
    int ycor = ycoord;
    xcor = xcor + ((rand() % 3) - 1);
    ycor = ycor + ((rand() % 3) - 1);
    while(hardness[xcor][ycor] == HARDEST){
      xcor = xcoord;
      ycor = ycoord;
      xcor = xcor + ((rand() % 3) - 1);
      ycor = ycor + ((rand() % 3) - 1);
    }
    if(xcor == pcX && ycor == pcY){
      this->attack();
    }
    else if(hardness[xcor][ycor] < ONE_HARD + 1 && hardness[xcor][ycor] > 0){
      xcoord = xcor;
      ycoord = ycor;
      hardness[xcor][ycor] = 0;
      roomAndPath[xcor][ycor] = PATH;
      fullDijkstra(pcX,pcY);
      rpDijkstra(pcX,pcY); 
    }
    else if(hardness[xcor][ycor] == 0){
      xcoord = xcor;
      ycoord = ycor;
    }
    else if(hardness[xcor][ycor] > ONE_HARD){
      hardness[xcor][ycor] = hardness[xcor][ycor] - ONE_HARD;
      xcor = xcoord;
      ycor = ycoord;
    }
  }
  else{
    move_6();
  }
}

//smart + tunneling
void monster::move_5(){
  if(canSee(xcoord,ycoord,pcX,pcY) == 1){
    moveOnGradientTunneling();
    lastSeenX = pcX;
    lastSeenY = pcY;
  }
  else if(lastSeenX > 0 && lastSeenY > 0){
    rpDijkstra(lastSeenX, lastSeenY);
    moveOnGradientTunneling();
    rpDijkstra(pcX, pcY);
  }
  else{
    move_4();
  }
}

//telepathic + tunneling
void monster::move_6(){
  characterMap[xcoord][ycoord] = (char) 0;
  int xcor = xcoord;
  int ycor = ycoord;
  if(xcor < pcX){
    ++xcor;
  }
  if(ycor < pcY){
    ++ycor;
  }
  if(xcor > pcX){
    --xcor;
  }
  if(ycor > pcY){
    --ycor;
  }
  if(xcor == pcX && ycor == pcY){
    if(aggressive){
      this->attack();
    }
  }
  else if(hardness[xcor][ycor] < ONE_HARD + 1 && hardness[xcor][ycor] > 0){
    xcoord = xcor;
    ycoord = ycor;
    hardness[xcor][ycor] = 0;
    roomAndPath[xcor][ycor] = PATH;
    fullDijkstra(pcX,pcY);
    rpDijkstra(pcX,pcY); 
  }
  else if(hardness[xcor][ycor] == 0){
    xcoord = xcor;
    ycoord = ycor;
  }
  else if(hardness[xcor][ycor] > ONE_HARD){
    hardness[xcor][ycor] = hardness[xcor][ycor] - ONE_HARD;
    xcor = xcoord;
    ycor = ycoord;
  }
  else if(hardness[xcor][ycor] == HARDEST){
    if(hardness[xcoord][ycor] != HARDEST){
      ycoord = ycor;
    }
    else if(hardness[xcor][ycoord] != HARDEST){
      xcoord = xcor;
    }
  }
}

//smart + telepathic + tunneling
void monster::move_7(){
  moveOnGradientTunneling();
}


//only erratic
void monster::move_8(){
  move_0();
}

//smart + erratic
void monster::move_9(){
  if(rand() % 2){
    move_1();
  }
  else{
    move_0();
  }
}

//telepathic + erratic
void monster::move_a(){
  if(rand() % 2){
    move_2();
  }
  else{
    move_0();
  }
}

//smart + telepathic + erratic
void monster::move_b(){
  if(rand() % 2){
    move_3();
  }
  else{
    move_0();
  }
}

//tunneling + erratic
void monster::move_c(){
  int random = rand() % 2;
  if(random == 1){
    move_4();
  }
  if(random == 0){
    move_0();
  }
}

//smart + tunneling + erratic
void monster::move_d(){
  if(rand() % 2){
    move_5();
  }
  else{
    move_0();
  }
}

//telepathic + tunneling + erratic
void monster::move_e(){
  if(rand() % 2){
    move_6();
  }
  else{
    move_0();
  }
}

//smart + telepathic + tunneling + erratic
void monster::move_f(){
  if(rand() % 2){
    move_7();
  }
  else{
    move_0();
  }
}

/*
void (*moveFunction[NUM_M_TYPES])() =
{
  move_0, //no attributes (random motion)
  move_1, //only smart
  move_2, //only telepathic
  move_3, //smart + telepathic
  move_4, //only tunneling
  move_5, //smart + tunneling
  move_6, //telepathic + tunneling
  move_7, //smart + telepathic + tunneling
  move_8, //only erratic
  move_9, //smart + erratic
  move_a, //telepathic + erratic
  move_b, //smart + telepathic + erratic
  move_c, //tunneling + erratic
  move_d, //smart + tunneling + erratic
  move_e, //telepathic + tunneling + erratic
  move_f  //smart + telepathic + tunneling + erratic
};*/

void monster::mon_move(){
  if(m_type == 0){
    move_0();
  }
  else if(m_type == 1){
    move_1();
  }
  else if(m_type == 2){
    move_2();
  }
  else if(m_type == 3){
    move_3();
  }
  else if(m_type == 4){
    move_4();
  }
  else if(m_type == 5){
    move_5();
  }
  else if(m_type == 6){
    move_6();
  }
  else if(m_type == 7){
    move_7();
  }
  else if(m_type == 8){
    move_8();
  }
  else if(m_type == 9){
    move_9();
  }
  else if(m_type == 10){
    move_a();
  }
  else if(m_type == 11){
    move_b();
  }
  else if(m_type == 12){
    move_c();
  }
  else if(m_type == 13){
    move_d();
  }
  else if(m_type == 14){
    move_e();
  }
  else if(m_type == 15){
    move_f();
  }
}

int calculateDef(){
  int ret = main_character->def;
  if(main_character->offhand != 0){
    ret += main_character->offhand->defence;
  }
  if(main_character->armor != 0){
    ret += main_character->armor->defence;
  }
  if(main_character->helmet != 0){
    ret += main_character->helmet->defence;
  }
  if(main_character->boots != 0){
    ret += main_character->boots->defence;
  }
  if(main_character->gloves != 0){
    ret += main_character->gloves->defence;
  }
  return ret;
}

monster::monster(){
  aggressive = false;
  status = ALIVE;
  boss = 0;
  shop = 0;
  for(int i = 0; i < SHOP_SLOTS; ++i){
    shopObjs[i] = 0;
  }
}

void monster::attack(){
  int dam = damage->roll();
  int defense = calculateDef();
  if(dam <= defense){
    dam = 1;
  }
  else{
    dam -= defense;
  }
  main_character->hp -= dam;
}

monster::monster(int type, char sym){
  int monRoom = (rand() % (numRooms - 1)) + 1;
  xcoord = (rand() % (roomDimensions[monRoom][2] - 2)) + 
		      roomDimensions[monRoom][0] + 1;
  ycoord = (rand() % (roomDimensions[monRoom][3] - 2)) + 
		      roomDimensions[monRoom][1] + 1;
  speed = (rand() % MAX_SPEED - MIN_SPEED + 1) + MIN_SPEED;
  m_type = type;
  turn = 1;
  status = ALIVE;
  symbol = sym;
  lastSeenX = 0; 
  lastSeenY = 0;
}

void monster::print_monster(){
  clear();
  printw("%s\n\n",name.c_str());
  printw("SYMBOL: ");
  this->setColor();
  printw("%c\n\n",symbol);
  attron(COLOR_PAIR(ILUM));
  printw("%s\n",desc.c_str());
  printw("SPEED: ");
  if(speed < 10){
    printw("SLOW");
  }
  else if(speed > 10 && speed < 20){
    printw("FAST");
  }
  else if(speed == 10){
    printw("MEDIUM");
  }
  else if(speed >= 20){
    printw("VERY FAST");
  }
}

void monster::setColor(){
  if(!colors.compare("YELLOW")){
    attron(COLOR_PAIR(YEL));
  }
  else if(!colors.compare("GREEN")){
    attron(COLOR_PAIR(GRAS));
  }
  else if(!colors.compare("CYAN")){
    attron(COLOR_PAIR(CY));
  }
  else if(!colors.compare("RED")){
    attron(COLOR_PAIR(MONST));
  }
  else if(!colors.compare("BLUE")){
    attron(COLOR_PAIR(BL));
  }
  else if(!colors.compare("MAGENTA")){
    attron(COLOR_PAIR(MAG));
  }
  else if(!colors.compare("BROWN")){
    attron(COLOR_PAIR(R_ILUM));
  }
  else{
    attron(COLOR_PAIR(ILUM));
  }
}

monster *monster_constructor(int type, char sym){
  return new monster(type, sym);
}

int monster_getX(monster *npc){
  return npc->xcoord;
}

int monster_getY(monster *npc){
  return npc->ycoord;
}

char monster_getSymbol(monster *npc){
  return npc->symbol;
}

int monster_getSpeed(monster *npc){
  return npc->speed;
}

void monster_setTurn(int turn, monster *npc){
  npc->turn = turn;
}

int monster_getTurn(monster *npc){
  return npc->turn;
}

void monster_setStatus(int status, monster *npc){
  npc->status = status;
}

int monster_getStatus(monster *npc){
  return npc->status;
}

void monster_move(monster *npc){
  npc->mon_move();
}

int monster_getType(monster *npc){
  return npc->m_type;
}


















