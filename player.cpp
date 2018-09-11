#include "core.h"

player::player(){
  str = dex = intel = endur = def = vit = 10;
  level = experience = 0;
  name = "MITCH";
  hp = hpmax = stamina = stammax = 100;
  gold = ammo = 0;
  color = ILUM;
  weapon = offhand = ranged = armor = helmet = cloak = 0;
  gloves = boots = amulet = light = ring1 = ring2 = 0;
  for(int i = 0; i < CARRY_SLOTS; ++i){
    carry[i] = 0;
  }
  for(int i = 0; i < CONSUMABLE_SLOTS; ++i){
    consumables[i] = 0;
  }
}

player::player(int x, int y)
{
  xcoord = x;
  ycoord = y;
  speed = PC_SPEED;
  turn = 0;
  symbol = PC;
  status = ALIVE;
}

void player::initStats(){
  hpmax = vit * VIT_SCALAR;
  stammax = 10 * endur + 2 * vit;
}

void player::pc_move()
{
  int key;
  GET_KEY:
  key = getch();
  characterMap[xcoord][ycoord] = (char) 0;
  if(key == KEY_7 || key == KEY_y){
    if(hardness[pcX - 1][pcY - 1] == 0){
      pcX = pcX - 1;
      pcY = pcY - 1;
    }
  }
  else if(key == KEY_8){
    if(hardness[pcX][pcY - 1] == 0){
      pcY = pcY - 1;
    }
  }
  else if(key == KEY_9){
    if(hardness[pcX + 1][pcY - 1] == 0){
      pcX = pcX + 1;
      pcY = pcY - 1;
    }
  }
  else if(key == KEY_6){
    if(hardness[pcX + 1][pcY] == 0){
      pcX = pcX + 1;
    }
  }
  else if(key == KEY_3){
    if(hardness[pcX + 1][pcY + 1] == 0){
      pcX = pcX + 1;
      pcY = pcY + 1;
    }
  }
  else if(key == KEY_2){
    if(hardness[pcX][pcY + 1] == 0){
      pcY = pcY + 1;
    }
  }
  else if(key == KEY_1){
    if(hardness[pcX - 1][pcY + 1] == 0){
      pcX = pcX - 1;
      pcY = pcY + 1;
    }
  }
  else if(key == KEY_4){
    if(hardness[pcX - 1][pcY] == 0){
      pcX = pcX - 1;
    }
  }
  else if(key == KEY_5 || key == KEY_SPACE){
    
  }
  else if(key == KEY_DOWNSTAIRS && staircases[pcX][pcY] == DOWN_STAIRCASE){
    if(boss == 0){
      nextDungeon = 1;
      writeToFile();
      --dungeonLevel;
    }
    else{
      move(0,0);
      clrtoeol();
      printw("UNABLE TO LEAVE DURING A BOSS BATTLE");
      goto GET_KEY;
    }
  }
  else if(key == KEY_UPSTAIRS && staircases[pcX][pcY] == UP_STAIRCASE){
    if(boss == 0){
      nextDungeon = 1;
      writeToFile();
      ++dungeonLevel;
    }
    else{
      move(0,0);
      clrtoeol();
      printw("UNABLE TO LEAVE DURING A BOSS BATTLE");
      goto GET_KEY;
    }
  }
  else if(key == KEY_r){
    characterMap[pcX][pcY] = PC;
    int didEsc = rangeAttack(pcX,pcY);
    ncursesPrint(pcX,pcY);
    if(didEsc == 1) goto GET_KEY;
  }
  else if(key == KEY_s){
    characterMap[pcX][pcY] = PC;
    int didEsc = useScroll();
    ncursesPrint(pcX,pcY);
    if(didEsc == 1) goto GET_KEY;
  }
  else if(key == KEY_L){
    characterMap[pcX][pcY] = PC;
    enterLookMode(pcX,pcY);
    ncursesPrint(pcX,pcY);
    printLevel();
    if (quit == 0)goto GET_KEY;
  }
  else if(key == KEY_l){
    levelUp();
    characterMap[xcoord][ycoord] = symbol;
    ncursesPrint(pcX,pcY);
    characterMap[xcoord][ycoord] = (char) 0;
    goto GET_KEY;
  }
  else if(key == KEY_Q){
    move(0,0);
    clrtoeol();
    printw("ARE YOU SURE YOU WANT TO QUIT? (y/n)");
    int yorn = 0;
    while(yorn != KEY_y && yorn != KEY_n){
      yorn = getch();
      if(yorn == KEY_y){
        quit = 1;
      }
    }
    if(yorn == KEY_n){
      printLevel();
      goto GET_KEY;
    }
  }
/*  else if(key == KEY_i){
    printInventory();
    getch();
    characterMap[xcoord][ycoord] = symbol;
    ncursesPrint(pcX,pcY);
    printLevel();
    characterMap[xcoord][ycoord] = (char) 0;
    goto GET_KEY;
  }*/
  else if(key == KEY_e){
    printEquipment();
    getch();
    characterMap[xcoord][ycoord] = symbol;
    ncursesPrint(pcX,pcY);
    printLevel();
    characterMap[xcoord][ycoord] = (char) 0;
    goto GET_KEY;
  }
  else if(key == KEY_h){
    printControls();
    characterMap[xcoord][ycoord] = symbol;
    ncursesPrint(pcX,pcY);
    printLevel();
    characterMap[xcoord][ycoord] = (char) 0;
    goto GET_KEY;
  }
  else if(key == KEY_c){
    consumeItem();
    characterMap[xcoord][ycoord] = symbol;
    ncursesPrint(pcX,pcY);
    characterMap[xcoord][ycoord] = (char) 0;
    goto GET_KEY;
  }
  else if(key == KEY_w){
    wearItem();
    characterMap[xcoord][ycoord] = symbol;
    ncursesPrint(pcX,pcY);
    characterMap[xcoord][ycoord] = (char) 0;
    goto GET_KEY;
  }
  else if(key == KEY_i){
    inspectItem();
    characterMap[xcoord][ycoord] = symbol;
    ncursesPrint(pcX,pcY);
    characterMap[xcoord][ycoord] = (char) 0;
    goto GET_KEY;
  }
  else if(key == KEY_d){
    dropItem();
    characterMap[xcoord][ycoord] = symbol;
    ncursesPrint(pcX,pcY);
    characterMap[xcoord][ycoord] = (char) 0;
    goto GET_KEY;
  }
  else if(key == KEY_x){
    expungeItem();
    characterMap[xcoord][ycoord] = symbol;
    ncursesPrint(pcX,pcY);
    characterMap[xcoord][ycoord] = (char) 0;
    goto GET_KEY;
  }
  else if(key == KEY_t){
    takeOff();
    characterMap[xcoord][ycoord] = symbol;
    ncursesPrint(pcX,pcY);
    characterMap[xcoord][ycoord] = (char) 0;
    goto GET_KEY;
  }
  else{
    goto GET_KEY;
  }
  if(pcX == xcoord && pcY == ycoord){
    stamina += endur;
  }
  if(mondun[pcX][pcY] == 0){
    xcoord = pcX;
    ycoord = pcY;
  }
  else{
    attack(pcX,pcY);
    pcX = xcoord;
    pcY = ycoord;
  }
  ++stamina;
  characterMap[xcoord][ycoord] = symbol;
  if(stamina > stammax){
    stamina = stammax;
  }
  if(stamina < 0){
    stamina = 0;
  }
  if(objdun[pcX][pcY] != 0){
    pickUpItem();
  }
  if(isBossLevel() && boss != 0){
    printLevel();
  }
  if(dungeonLevel == secondBossLevel && hp > 1 &&
	!(roomAndPath[pcX][pcY] == ROOM || roomAndPath[pcX][pcY] == PATH)){
    hp -= rand() % 2;
  }
}

int player::useScroll(){
  bool hasScroll = false;
  for(int i = 0; i < SCROLL_SLOTS; ++i){
    if(scrolls[i] != 0){
      hasScroll = true;
    }
  }
  move(0,0);
  clrtoeol();
  if(!hasScroll){
    printw("NO SCROLLS IN INVENTORY");
    return 1;
  }
  move(0,0);
  clear();
  printw("SELECT SCROLL YOU WISH TO USE\n\n");
  for(int i = 0; i < SCROLL_SLOTS; ++i){
    attron(COLOR_PAIR(ILUM));
    printw("%d: [SLOT %d] - (",i,i);
    if(scrolls[i] != 0){
      printw("%s",scrolls[i]->name.c_str());
    }
    attron(COLOR_PAIR(ILUM));
    printw(")\n");
  }
  bool chosen = false;
  int key;
  int scroll;
  while(!chosen){
    key = getch();
    if(key == KEY_0){
      scroll = 0;
      chosen = true;
    }
    else if(key == KEY_1){
      scroll = 1;
      chosen = true;
    }
    else if(key == KEY_2){
      scroll = 2;
      chosen = true;
    }
    else if(key == KEY_3){
      scroll = 3;
      chosen = true;
    }
    else if(key == KEY_4){
      scroll = 4;
      chosen = true;
    }
    else if(key == KEY_5){
      scroll = 5;
      chosen = true;
    }
    else if(key == KEY_6){
      scroll = 6;
      chosen = true;
    }
    else if(key == KEY_7){
      scroll = 7;
      chosen = true;
    }
    else if(key == KEY_8){
      scroll = 8;
      chosen = true;
    }
    else if(key == KEY_9){
      scroll = 9;
      chosen = true;
    }
    else if(key == KEY_ESC){
      printLevel();
      return 1;
    }
  }
  move(0,0);
  clrtoeol();
  if(scrolls[scroll] == 0){
    printw("NO SCROLL SELECTED");
    return 1;
  }
  else if(scrolls[scroll]->type != objtype_SCROLL){
    printw("NO SCROLL SELECTED");
    return 1;
  }
  else if(scrolls[scroll]->intel > intel){
    printw("YOU LACK THE INTELLIGENCE TO USE THIS SCROLL");
    return 1;
  }
  else{
    printw("ARE YOU SURE YOU WANT TO USE %s? (y/n)",scrolls[scroll]->name.c_str());
    int key = 0;
    while(key != KEY_y && key != KEY_n){
      key = getch();
      if(key == KEY_n){
	printLevel();
        return 1;
      }
      else if(key == KEY_y){
	move(0,0);
	clrtoeol();
	printw("YOU USE %s",scrolls[scroll]->name.c_str());
	if(scrolls[scroll]->attribute == 1){
	  for(int i = 1; i < WIDTH - 1; ++i){
	    for(int j = 1; j < HEIGHT - 1; ++j){
	      if(hardness[i][j] == 0){
	        seen[i][j] = SEEN;
	      }
	      else if(isBorder(i,j)){
	        seen[i][j] = SEEN;
	      }
	    }
	  }
        }
	else if(scrolls[scroll]->attribute == 2){
	  if(dungeonLevel >= 0){
	    move(0,0);
	    clrtoeol();
	    printw("UNABLE TO USE %s",scrolls[scroll]->name.c_str());
	    return 1;  
	  }
	  for(int i = 0; i < WIDTH; ++i){
	    for(int j = 0; j < HEIGHT; ++j){
	      if(staircases[i][j] == UP_STAIRCASE){
		characterMap[pcX][pcY] = (char) 0;
		pcX = i;
		pcY = j;
		xcoord = i;
		ycoord = j;
		characterMap[pcX][pcY] = symbol;
		writeToFile();
		++dungeonLevel;
		nextDungeon = 1;
	      }
	    }
	  }
	  while(dungeonLevel != 0){
	    monsleft = readFromFile();
	    for(int i = 0; i < WIDTH; ++i){
	      for(int j = 0; j < HEIGHT; ++j){
		if(staircases[i][j] == UP_STAIRCASE){
		  characterMap[pcX][pcY] = (char) 0;
		  pcX = i;
		  pcY = j;
		  xcoord = i;
		  ycoord = j;
		  characterMap[pcX][pcY] = symbol;
		}
	      }
	    }
	    writeToFile();
	    ++dungeonLevel;
	  }
	}
	else if(scrolls[scroll]->attribute == 3){
	  if(isBossLevel()){
	    move(0,0);
	    clrtoeol();
	    printw("UNABLE TO USE %s IN A BOSS FIGHT",scrolls[scroll]->name.c_str());
	  }
	  int injured = 0;
	  int youKilled = 0;
	  for(int i = 0; i < WIDTH; ++i){
	    for(int j = 0; j < HEIGHT; ++j){
	      if(mondun[i][j] != 0){
		mondun[i][j]->hp -= scrolls[scroll]->damage->roll();
		if(mondun[i][j]->hp <= 0){
		  mondun[i][j]->status = DEAD;
		  --monsleft;
		  ++killed;
		  ++youKilled;
		  characterMap[i][j] = char(0);
		  experience += mondun[i][j]->exp;
		  mondun[i][j] = 0;
		}
		else{
		  ++injured;
		}
	      }
	    }
	  }
	  move(0,0);
	  clrtoeol();
	  printw("YOU KILLED %d AND INJURED %d MONSTER WITH YOUR %s",
				youKilled,injured,scrolls[scroll]->name.c_str());
	}
	else if(scrolls[scroll]->attribute == 4){
	  isFrozen = true;
	  frozenTurn = 0;
	  move(0,0);
	  clrtoeol();
	  printw("TIME IS NOW FROZEN");
	}
      }
    }
    delete scrolls[scroll];
    scrolls[scroll] = 0;
  }
  return 0;
}

bool isBorder(int x, int y){
  int i,j;
  bool ret = false;
  for(i = x - 1; i <= x + 1; ++i){
    for(j = y - 1; j <= y + 1; ++j){
      if(hardness[i][j] == 0 && hardness[x][y] != 0){
	ret = true;
      }
    }
  }
  return ret;
}

int calculateWeight(){
  int ret = 0;
  if(main_character->weapon != 0){
    ret += main_character->weapon->weight;
  }
  if(main_character->offhand != 0){
    ret += main_character->offhand->weight;
  }
  if(main_character->armor != 0){
    ret += main_character->armor->weight;
  }
  if(main_character->helmet != 0){
    ret += main_character->helmet->weight;
  }
  if(main_character->boots != 0){
    ret += main_character->boots->weight;
  }
  if(main_character->gloves != 0){
    ret += main_character->gloves->weight;
  }
  return ret;
}

void player::pickUpItem(){
  int open = 0;
  bool hasOpen = false;
  if(objdun[pcX][pcY]->type == objtype_CONSUMABLE){
    for(int i = 0; i < CONSUMABLE_SLOTS; ++i){
      if(consumables[i] == 0){
        open = i;
        hasOpen = true;
        break;
      }
    }
    if(hasOpen){
      consumables[open] = objdun[pcX][pcY];
      if(consumables[open]->next != 0){
        objdun[pcX][pcY] = consumables[open]->next;
        consumables[open]->next = 0;
        objectMap[pcX][pcY] = objdun[pcX][pcY]->symbol;
      }
      else{
        objdun[pcX][pcY] = 0;
        objectMap[pcX][pcY] = (char) 0;
      }
      move(0,0);
      clrtoeol();
      printw("YOU PICKED UP %s\n",consumables[open]->name.c_str());
      --numObjs;
    }
    else{
      move(0,0);
      clrtoeol();
      printw("YOU HAVE NO ROOM FOR %s",objdun[pcX][pcY]->name.c_str());
    }
  }
  else if(objdun[pcX][pcY]->type == objtype_SCROLL){
    for(int i = 0; i < SCROLL_SLOTS; ++i){
      if(scrolls[i] == 0){
        open = i;
        hasOpen = true;
        break;
      }
    }
    if(hasOpen){
      scrolls[open] = objdun[pcX][pcY];
      if(scrolls[open]->next != 0){
        objdun[pcX][pcY] = scrolls[open]->next;
        scrolls[open]->next = 0;
        objectMap[pcX][pcY] = objdun[pcX][pcY]->symbol;
      }
      else{
        objdun[pcX][pcY] = 0;
        objectMap[pcX][pcY] = (char) 0;
      }
      move(0,0);
      clrtoeol();
      printw("YOU PICKED UP %s\n",scrolls[open]->name.c_str());
      --numObjs;
    }
    else{
      move(0,0);
      clrtoeol();
      printw("YOU HAVE NO ROOM FOR %s",objdun[pcX][pcY]->name.c_str());
    }
  }
  else if(objdun[pcX][pcY]->type == objtype_GOLD){
    gold += objdun[pcX][pcY]->value;
    move(0,0);
    clrtoeol();
    printw("YOU PICK UP A BAG OF %d GOLD",objdun[pcX][pcY]->value);
    delete objdun[pcX][pcY];
    objdun[pcX][pcY] = 0;
    objectMap[pcX][pcY] = (char) 0;
  }
  else if(objdun[pcX][pcY]->type == objtype_AMMUNITION){
    ammo += objdun[pcX][pcY]->value;
    move(0,0);
    clrtoeol();
    printw("YOU PICK UP A BUNDLE OF %d ARROWS",objdun[pcX][pcY]->value);
    delete objdun[pcX][pcY];
    objdun[pcX][pcY] = 0;
    objectMap[pcX][pcY] = (char) 0;
  }
  else{
    for(int i = 0; i < CARRY_SLOTS; ++i){
      if(carry[i] == 0){
        open = i;
        hasOpen = true;
        break;
      }
    }
    if(hasOpen){
      carry[open] = objdun[pcX][pcY];
      if(carry[open]->next != 0){
        objdun[pcX][pcY] = carry[open]->next;
        carry[open]->next = 0;
        objectMap[pcX][pcY] = objdun[pcX][pcY]->symbol;
      }
      else{
        objdun[pcX][pcY] = 0;
        objectMap[pcX][pcY] = (char) 0;
      }
      move(0,0);
      clrtoeol();
      printw("YOU PICKED UP %s\n",carry[open]->name.c_str());
      --numObjs;
    }
    else{
      move(0,0);
      clrtoeol();
      printw("YOU HAVE NO ROOM FOR %s",objdun[pcX][pcY]->name.c_str());
    }
  }
}

void rangeBresenham(int x0, int y0, int x1, int y1){
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;
  for(;;){
    if(hardness[x0][y0] != 0 || mondun[x0][y0] != 0){
      break;
    }
    seen[x0][y0] = VISIBLE;
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
  if(mondun[x0][y0] == 0){
    move(0,0);
    clrtoeol();
    printw("YOU MISSED");
  }
  else if(mondun[x0][y0] != 0){
    int dam = main_character->ranged->damage->roll();
    mondun[x0][y0]->hp -= dam;
    move(0,0);
    clrtoeol();
    if(!mondun[x0][y0]->aggressive){
      printw("\"Hey! Be careful. You could hurt someone\"");
      return;
    }
    printw("YOU DO %d DAMAGE WITH YOUR %s TO %s",
		dam,main_character->ranged->name.c_str(),mondun[x0][y0]->name.c_str());
    if(mondun[x0][y0]->hp >= 0 && dungeonLevel == secondBossLevel && boss != 0 && rand() % BOSS_TELE == 0){
      characterMap[x0][y0] = (char) 0;
      int newX = (rand() % (WIDTH - 10)) + 5;
      int newY = (rand() % (HEIGHT - 10)) + 5;
      while(newX == pcX && newY == pcY){
        newX = (rand() % (WIDTH - 10)) + 5;
        newY = (rand() % (HEIGHT - 10)) + 5;
      }
      mondun[newX][newY] = mondun[x0][y0];
      mondun[newX][newY]->xcoord = newX;
      mondun[newX][newY]->ycoord = newY;
      mondun[newX][newY]->lastSeenX = 0;
      mondun[newX][newY]->lastSeenY = 0;
      mondun[x0][y0] = 0;
      characterMap[newX][newY] = mondun[newX][newY]->symbol;
      characterMap[pcX][pcY] = main_character->symbol;
      ncursesPrint(pcX,pcY);
      characterMap[x0][x0] = (char) 0;
      move(0,0);
      clrtoeol();
      printw("%s TELEPORTS AWAY",boss->name.c_str());
      getch();
      x0 = newX;
      y0 = newY;
    }
    else if(mondun[x0][y0]->hp <= 0){
      mondun[x0][y0]->status = DEAD;
      main_character->experience += mondun[x0][y0]->exp;
      characterMap[x0][y0] = (char) 0;
      ++killed;
      --monsleft;
      if(rand() % DROP_RATE == 0 && mondun[x0][y0]->boss == 0){
        vector<object*>* drop = read_obj_file(1,MON_DROP);
        drop->at(0)->next = objdun[x0][y0];
        objdun[x0][y0] = drop->at(0);
        objectMap[x0][y0] = objdun[x0][y0]->symbol;
      }
      move(0,0);
      clrtoeol();
      printw("YOU KILLED %s WITH YOUR %s",
		mondun[x0][y0]->name.c_str(),main_character->ranged->name.c_str());
      mondun[x0][y0] = 0;
    }
  }
}

int player::rangeAttack(int rangeX, int rangeY){
  ncursesPrint(rangeX,rangeY);
  move(0,0);
  clrtoeol();
  attron(COLOR_PAIR(ILUM));
  if(ranged == 0){
    printw("NO RANGED WEAPON EQUIPPED");
    return 1;
  }
  if(ammo <= 0 && ranged->type == objtype_RANGED){
    move(0,0);
    clrtoeol();
    printw("YOU ARE OUT OF AMMO");
    return 1;
  }
  int range = ranged->attribute;
  printw("RANGE ATTACK WITH %s\n",ranged->name.c_str());
  int key;
  GET_KEY:key = getch();
  int xdist;
  int ydist;
  while(key != KEY_ESC && key != KEY_r){
    if(key == KEY_8){
      xdist = abs(rangeX - pcX);
      ydist = abs((rangeY-1) - pcY);
      if(rangeY > 0 && (xdist * xdist + ydist * ydist <= range * range)){
        rangeY = rangeY - 1;
      }
    }
    else if(key == KEY_6){
      xdist = abs((rangeX+1) - pcX);
      ydist = abs(rangeY - pcY);
      if(rangeX < WIDTH - 1 && (xdist * xdist + ydist * ydist <= range * range)){
        rangeX = rangeX + 1;
      }
    }
    else if(key == KEY_2){
      xdist = abs(rangeX - pcX);
      ydist = abs((rangeY+1) - pcY);
      if(rangeY < HEIGHT - 1 && (xdist * xdist + ydist * ydist <= range * range)){
        rangeY = rangeY + 1;
      }
    }
    else if(key == KEY_4){
      xdist = abs((rangeX-1) - pcX);
      ydist = abs(rangeY - pcY);
      if(rangeX > 0 && (xdist * xdist + ydist * ydist <= range * range)){
        rangeX = rangeX - 1;
      }
    }
    else if(key == KEY_Q){
      quit = 1;
      return 0;
    }
    else{
      goto GET_KEY;
    }
    ncursesPrint(rangeX,rangeY);
    key = getch();
  }
  if(key == KEY_ESC){
    printLevel();
    return 1;
  }
  else if(key == KEY_r){
    if(rangeX == pcX && rangeY == pcY){
      printLevel();
      return 1;
    }
    if(stamina > (main_character->ranged->weight + main_character->armor->weight) + endur + 5){
      rangeBresenham(pcX,pcY,rangeX,rangeY);
      stamina -= (main_character->ranged->weight + main_character->armor->weight) + endur + 5;
      if(ranged->type == objtype_RANGED){
        --ammo;
      }
    }
    else{
      move(0,0);
      clrtoeol();
      printw("YOU ARE TO EXHAUSTED TO USE %s",main_character->ranged->name.c_str());
    }
  }
  return 0;
}

void player::attack(int x, int y){
  move(0,0);
  clrtoeol();
  int dam;
  if(!mondun[x][y]->aggressive){
    printw("%s",mondun[x][y]->desc.c_str());
    getch();
    printLevel();
    return;
  }
  else if(weapon == 0){
    dam = ((str + dex) / 4) + (rand() % 10) - 5;
    printw("YOU DO %d DAMAGE WITH YOUR BARE HANDS TO %s",dam,mondun[x][y]->name.c_str());
    mondun[x][y]->hp -= dam;
  }
  else if(weapon != 0){
    dam = weapon->damage->roll();
    int weight = calculateWeight();
    if(stamina < weight){
      printw("YOU ARE TO EXHAUSTED TO ATTACK");
      stamina += endur;
    }
    else{
      stamina -= weight;
      printw("YOU DO %d DAMAGE WITH YOUR %s TO %s",dam,weapon->name.c_str(),mondun[x][y]->name.c_str());
      mondun[x][y]->hp -= dam;
    }
  }
  if(mondun[x][y]->hp >= 0 && dungeonLevel == secondBossLevel && boss != 0 && rand() % BOSS_TELE == 0){
    characterMap[x][y] = (char) 0;
    int newX = (rand() % (WIDTH - 10)) + 5;
    int newY = (rand() % (HEIGHT - 10)) + 5;
    while(newX == pcX && newY == pcY){
      newX = (rand() % (WIDTH - 10)) + 5;
      newY = (rand() % (HEIGHT - 10)) + 5;
    }
    mondun[newX][newY] = mondun[x][y];
    mondun[newX][newY]->xcoord = newX;
    mondun[newX][newY]->ycoord = newY;
    mondun[newX][newY]->lastSeenX = 0;
    mondun[newX][newY]->lastSeenY = 0;
    mondun[x][y] = 0;
    characterMap[newX][newY] = mondun[newX][newY]->symbol;
    characterMap[pcX][pcY] = symbol;
    ncursesPrint(pcX,pcY);
    characterMap[pcX][pcY] = (char) 0;
    move(0,0);
    clrtoeol();
    printw("%s TELEPORTS AWAY",boss->name.c_str());
    getch();
    x = newX;
    y = newY;
  }
  else if(mondun[x][y]->hp <= 0){
    mondun[x][y]->status = DEAD;
    experience += mondun[x][y]->exp;
    characterMap[x][y] = (char) 0;
    ++killed;
    --monsleft;
    if(rand() % DROP_RATE == 0 && mondun[x][y]->boss == 0){
      vector<object*>* drop = read_obj_file(1,MON_DROP);
      drop->at(0)->next = objdun[x][y];
      objdun[x][y] = drop->at(0);
      objectMap[x][y] = objdun[x][y]->symbol;
    }
    move(0,0);
    clrtoeol();
    if(weapon != 0){
      printw("YOU KILLED %s WITH YOUR %s",mondun[x][y]->name.c_str(),weapon->name.c_str());
    }
    else{
      printw("YOU KILLED %s WITH YOUR BARE HANDS",mondun[x][y]->name.c_str());
    }
    mondun[x][y] = 0;
  }
}

void player::printStats(){
  attron(COLOR_PAIR(ILUM));
  clrtoeol();
  printw("%s: LEVEL-%d VIT-%d STR-%d DEX-%d INT-%d END-%d DEF-%d ",name.c_str()
		,level,vit,str,dex,intel,endur,def);
  clrtoeol();
  printw("\nEXP-");
  if(experience >= LEVEL_SCALAR + level * LEVEL_SCALAR){
    attron(COLOR_PAIR(GRAS));
  }
  printw("%d ",experience);
  attron(COLOR_PAIR(ILUM));
  printw("HP-");
  if(hp > hpmax / 2){
    attron(COLOR_PAIR(GRAS));
  }
  else if(hp > hpmax / 4){
    attron(COLOR_PAIR(YEL));
  }
  else{
    attron(COLOR_PAIR(MONST));
  }
  printw("%d",hp);
  attron(COLOR_PAIR(ILUM));
  printw("(%d) ",hpmax);
  printw("STAMINA-");
  if(stamina > stammax / 2){
    attron(COLOR_PAIR(GRAS));
  }
  else if(stamina > stammax / 4){
    attron(COLOR_PAIR(YEL));
  }
  else{
    attron(COLOR_PAIR(MONST));
  }
  printw("%d",stamina);
  attron(COLOR_PAIR(ILUM));
  printw("(%d) ",stammax);
  attron(COLOR_PAIR(YEL));
  printw("GOLD");
  attron(COLOR_PAIR(ILUM));
  printw("-%d ",gold);
  attron(COLOR_PAIR(R_ILUM));
  printw("AMMO");
  attron(COLOR_PAIR(ILUM));
  printw("-%d ",ammo);
}

void player::levelUp(){
  move(0,0);
  clear();
  if(experience < LEVEL_SCALAR + level * LEVEL_SCALAR){
    printw("YOU LACK THE EXPERIENCE TO LEVEL UP");
  }
  else if(experience >= LEVEL_SCALAR + level * LEVEL_SCALAR){
    printw("WHICH STAT WOULD YOU LIKE TO LEVEL UP\n\n");
    printw("1: VITALITY\n2: STRENGTH\n3: DEXTERITY\n4: INTELLIGENCE\n5: ENDURANCE\n6: DEFENSE");
    int key = getch();
    bool picked = false;
    while(!picked){
      if(key == KEY_1){
        ++vit;
        experience -= LEVEL_SCALAR + level * LEVEL_SCALAR;
	++level;
	picked = true;
	move(0,0);
	clrtoeol();
        printw("YOU LEVELED UP VITALITY");
        hp += VIT_SCALAR;
      }
      else if(key == KEY_2){
        ++str;
        experience -= LEVEL_SCALAR + level * LEVEL_SCALAR;
	++level;
	picked = true;
	move(0,0);
	clrtoeol();
        printw("YOU LEVELED UP STRENGTH");
      }
      else if(key == KEY_3){
        ++dex;
        experience -= LEVEL_SCALAR + level * LEVEL_SCALAR;
	++level;
	picked = true;
	move(0,0);
	clrtoeol();
        printw("YOU LEVELED UP DEXTERITY");
      }
      else if(key == KEY_4){
        ++intel;
        experience -= LEVEL_SCALAR + level * LEVEL_SCALAR;
	++level;
	picked = true;
	move(0,0);
	clrtoeol();
        printw("YOU LEVELED UP INTELLIGENCE");
      }
      else if(key == KEY_5){
        ++endur;
        experience -= LEVEL_SCALAR + level * LEVEL_SCALAR;
	++level;
	picked = true;
	move(0,0);
	clrtoeol();
        printw("YOU LEVELED UP ENDURANCE");
      }
      else if(key == KEY_6){
        ++def;
        experience -= LEVEL_SCALAR + level * LEVEL_SCALAR;
	++level;
	picked = true;
	move(0,0);
	clrtoeol();
        printw("YOU LEVELED UP DEFENSE");
      }
      else if(key == KEY_ESC){
        printLevel();
	break;
      }
    }
  }
  initStats();
}

void player::takeOff(){
  printEquipment();
  move(0,0);
  clrtoeol();
  printw("WHICH ITEM WOULD YOU LIKE TO UNEQUIP?");
  bool hasOpen = false;
  int open = 0;
  for(int i = 0; i < CARRY_SLOTS; ++i){
    if(carry[i] == 0){
      hasOpen = true;
      open = i;
      break;
    }
  }
  if(hasOpen == true){
    int key;
    bool pickedItem = false;
    key = getch();
    while(!pickedItem){
      if(key == KEY_a){
        if(weapon != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",weapon->name.c_str());
          carry[open] = weapon;
	  weapon = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_b){
        if(offhand != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",offhand->name.c_str());
          carry[open] = offhand;
	  offhand = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_c){
        if(ranged != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",ranged->name.c_str());
          carry[open] = ranged;
	  ranged = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_d){
        if(armor != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",armor->name.c_str());
          carry[open] = armor;
	  armor = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_e){
        if(helmet != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",helmet->name.c_str());
          carry[open] = helmet;
	  helmet = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_f){
        if(cloak != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",cloak->name.c_str());
          carry[open] = cloak;
	  cloak = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_g){
        if(gloves != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",gloves->name.c_str());
          carry[open] = gloves;
	  gloves = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_h){
        if(boots != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",boots->name.c_str());
          carry[open] = boots;
	  boots = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_i){
        if(amulet != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",amulet->name.c_str());
          carry[open] = amulet;
	  amulet = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_j){
        if(light != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",light->name.c_str());
          carry[open] = light;
	  light = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_k){
        if(ring1 != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",ring1->name.c_str());
          carry[open] = ring1;
	  ring1 = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_l){
        if(ring2 != 0){
          move(0,0);
          clrtoeol();
          printw("UNEQUIPPED %s",ring2->name.c_str());
          carry[open] = ring2;
	  ring2 = 0;
	  break;
        }
        else{
          move(0,0);
          clrtoeol();
          printw("NO ITEM TO TAKE OFF");
	  key = getch();
        }
      }
      else if(key == KEY_ESC){
        printLevel();
        break;
      }
      else{
        key = getch();
      }
    }
  }
  else{
    move(0,0);
    clrtoeol();
    printw("NO ROOM IN INVENTORY");
  }
}

void player::dropItem(){
  printInventory();
  move(0,0);
  clrtoeol();
  printw("WHICH ITEM WOULD YOU LIKE TO DROP?");
  int key;
  int item = 0;
  bool pickedItem = false;
  key = getch();
  while(!pickedItem){
    if(key == KEY_0){
      item = 0;
      pickedItem = true;
    }
    else if(key == KEY_1){
      item = 1;
      pickedItem = true;
    }
    else if(key == KEY_2){
      item = 2;
      pickedItem = true;
    }
    else if(key == KEY_3){
      item = 3;
      pickedItem = true;
    }
    else if(key == KEY_4){
      item = 4;
      pickedItem = true;
    }
    else if(key == KEY_5){
      item = 5;
      pickedItem = true;
    }
    else if(key == KEY_6){
      item = 6;
      pickedItem = true;
    }
    else if(key == KEY_7){
      item = 7;
      pickedItem = true;
    }
    else if(key == KEY_8){
      item = 8;
      pickedItem = true;
    }
    else if(key == KEY_9){
      item = 9;
      pickedItem = true;
    }
    else if(key == KEY_ESC){
      printLevel();
      break;
    }
    else{
      key = getch();
    }
  }
  if(pickedItem){
    if(carry[item] == 0){
      move(0,0);
      clrtoeol();
      printw("NO ITEM TO DROP");
    }
    else if(carry[item] != 0){
      move(0,0);
      clrtoeol();
      printw("DROPPED %s",carry[item]->name.c_str());
      if(objdun[pcX][pcY] == 0){
        objdun[pcX][pcY] = carry[item];
	objectMap[pcX][pcY] = carry[item]->symbol;
        carry[item] = 0;
      }
      else if(objdun[pcX][pcY] != 0){
        carry[item]->next = objdun[pcX][pcY];
        objdun[pcX][pcY] = carry[item];
	objectMap[pcX][pcY] = carry[item]->symbol;
        carry[item] = 0;
      }
    }
  }
}

void player::expungeItem(){
  printInventory();
  move(0,0);
  clrtoeol();
  printw("WHICH ITEM WOULD YOU LIKE TO EXPUNGE?");
  int key;
  int item = 0;
  bool pickedItem = false;
  key = getch();
  while(!pickedItem){
    if(key == KEY_0){
      item = 0;
      pickedItem = true;
    }
    else if(key == KEY_1){
      item = 1;
      pickedItem = true;
    }
    else if(key == KEY_2){
      item = 2;
      pickedItem = true;
    }
    else if(key == KEY_3){
      item = 3;
      pickedItem = true;
    }
    else if(key == KEY_4){
      item = 4;
      pickedItem = true;
    }
    else if(key == KEY_5){
      item = 5;
      pickedItem = true;
    }
    else if(key == KEY_6){
      item = 6;
      pickedItem = true;
    }
    else if(key == KEY_7){
      item = 7;
      pickedItem = true;
    }
    else if(key == KEY_8){
      item = 8;
      pickedItem = true;
    }
    else if(key == KEY_9){
      item = 9;
      pickedItem = true;
    }
    else if(key == KEY_ESC){
      printLevel();
      break;
    }
    else{
      key = getch();
    }
  }
  if(pickedItem){
    if(carry[item] == 0){
      move(0,0);
      clrtoeol();
      printw("NO ITEM TO EXPUNGE");
    }
    else if(carry[item] != 0){
      move(0,0);
      clrtoeol();
      printw("EXPUNGED %s",carry[item]->name.c_str());
      delete carry[item];
      carry[item] = 0;
    }
  }
}

void player::inspectItem(){
  printInventory();
  move(0,0);
  clrtoeol();
  printw("INVENTORY: (PRESS A NUMBER TO INSPECT THE ITEM)");
  int key;
  int item = 0;
  bool pickedItem = false;
  key = getch();
  while(!pickedItem){
    if(key == KEY_0){
      item = 0;
      pickedItem = true;
    }
    else if(key == KEY_1){
      item = 1;
      pickedItem = true;
    }
    else if(key == KEY_2){
      item = 2;
      pickedItem = true;
    }
    else if(key == KEY_3){
      item = 3;
      pickedItem = true;
    }
    else if(key == KEY_4){
      item = 4;
      pickedItem = true;
    }
    else if(key == KEY_5){
      item = 5;
      pickedItem = true;
    }
    else if(key == KEY_6){
      item = 6;
      pickedItem = true;
    }
    else if(key == KEY_7){
      item = 7;
      pickedItem = true;
    }
    else if(key == KEY_8){
      item = 8;
      pickedItem = true;
    }
    else if(key == KEY_9){
      item = 9;
      pickedItem = true;
    }
    else if(key == KEY_ESC){
      printLevel();
      break;
    }
    else{
      key = getch();
    }
  }
  if(pickedItem && carry[item] != 0){
    carry[item]->printObject();
    getch();
    printLevel();
  }
  else{
    printLevel();
  }
}

void player::wearItem(){
  printInventory();
  move(0,0);
  clrtoeol();
  printw("WHICH ITEM WOULD YOU LIKE TO EQUIP?");
  int key;
  int item = 0;
  bool pickedItem = false;
  key = getch();
  while(!pickedItem){
    if(key == KEY_0){
      item = 0;
      pickedItem = true;
    }
    else if(key == KEY_1){
      item = 1;
      pickedItem = true;
    }
    else if(key == KEY_2){
      item = 2;
      pickedItem = true;
    }
    else if(key == KEY_3){
      item = 3;
      pickedItem = true;
    }
    else if(key == KEY_4){
      item = 4;
      pickedItem = true;
    }
    else if(key == KEY_5){
      item = 5;
      pickedItem = true;
    }
    else if(key == KEY_6){
      item = 6;
      pickedItem = true;
    }
    else if(key == KEY_7){
      item = 7;
      pickedItem = true;
    }
    else if(key == KEY_8){
      item = 8;
      pickedItem = true;
    }
    else if(key == KEY_9){
      item = 9;
      pickedItem = true;
    }
    else if(key == KEY_ESC){
      printLevel();
      break;
    }
    else{
      key = getch();
    }
  }
  if(pickedItem && carry[item] != 0){
    if(carry[item]->type == objtype_WEAPON 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
      object* temp = weapon;
      weapon = carry[item];
      carry[item] = temp;
      move(0,0);
      clrtoeol();
      printw("YOU EQUIP %s",weapon->name.c_str());
    }
    else if(carry[item]->type == objtype_OFFHAND 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
      object* temp = offhand;
      offhand = carry[item];
      carry[item] = temp;
      move(0,0);
      clrtoeol();
      printw("YOU EQUIP %s",offhand->name.c_str());
    }
    else if((carry[item]->type == objtype_RANGED || carry[item]->type == objtype_WAND)
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
      object* temp = ranged;
      ranged = carry[item];
      carry[item] = temp;
      move(0,0);
      clrtoeol();
      printw("YOU EQUIP %s",ranged->name.c_str());
    }
    else if(carry[item]->type == objtype_ARMOR 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
      object* temp = armor;
      armor = carry[item];
      carry[item] = temp;
      move(0,0);
      clrtoeol();
      printw("YOU EQUIP %s",armor->name.c_str());
    }
    else if(carry[item]->type == objtype_HELMET 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
      object* temp = helmet;
      helmet = carry[item];
      carry[item] = temp;
      move(0,0);
      clrtoeol();
      printw("YOU EQUIP %s",helmet->name.c_str());
    }
    else if(carry[item]->type == objtype_CLOAK 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
      object* temp = cloak;
      cloak = carry[item];
      carry[item] = temp;
      move(0,0);
      clrtoeol();
      printw("YOU EQUIP %s",cloak->name.c_str());
    }
    else if(carry[item]->type == objtype_GLOVES 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
      object* temp = gloves;
      gloves = carry[item];
      carry[item] = temp;
      move(0,0);
      clrtoeol();
      printw("YOU EQUIP %s",gloves->name.c_str());
    }
    else if(carry[item]->type == objtype_BOOTS 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
      object* temp = boots;
      boots = carry[item];
      carry[item] = temp;
      move(0,0);
      clrtoeol();
      printw("YOU EQUIP %s",boots->name.c_str());
    }
    else if(carry[item]->type == objtype_AMULET 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
      object* temp = amulet;
      amulet = carry[item];
      carry[item] = temp;
      move(0,0);
      clrtoeol();
      printw("YOU EQUIP %s",amulet->name.c_str());
    }
    else if(carry[item]->type == objtype_LIGHT 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
      object* temp = light;
      light = carry[item];
      carry[item] = temp;
      move(0,0);
      clrtoeol();
      printw("YOU EQUIP %s",light->name.c_str());
    }
    else if(carry[item]->type == objtype_RING 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
      if(ring1 == 0){
        ring1 = carry[item];
        carry[item] = 0;
      }
      else if(ring2 == 0){
	ring2 = carry[item];
        carry[item] = 0;
      }
      else{
	move(0,0);
	clrtoeol();
	printw("WHICH RING WOULD YOU LIKE TO REPLACE?");
        int ring;
        while(ring != KEY_1 && ring != KEY_2){
	  ring = getch();
          if(ring == KEY_1 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
	    object* temp = ring1;
	    ring1 = carry[item];
	    carry[item] = temp;
            move(0,0);
            clrtoeol();
            printw("YOU EQUIP %s",ring1->name.c_str());
	  }
          if(ring == KEY_2 
		&& str >= carry[item]->str && dex >= carry[item]->dex && intel >= carry[item]->intel){
	    object* temp = ring2;
	    ring2 = carry[item];
	    carry[item] = temp;
            move(0,0);
            clrtoeol();
            printw("YOU EQUIP %s",ring2->name.c_str());
	  }
	}
      }
    }
    else{
      move(0,0);
      clrtoeol();
      printw("YOU DO NOT HAVE THE STATS TO WIELD THIS ITEM");
    }
  }
  else{
    printLevel();
  }
}

char letters[CONSUMABLE_SLOTS] =
{
'a',
'b',
'c',
'd',
'e',
'f',
'g',
'h',
'i',
'j',
'k',
'l',
'm',
'n',
'o',
'p',
'q',
'r',
's',
't'
};

int keys[CONSUMABLE_SLOTS] =
{
KEY_a,
KEY_b,
KEY_c,
KEY_d,
KEY_e,
KEY_f,
KEY_g,
KEY_h,
KEY_i,
KEY_j,
KEY_k,
KEY_l,
KEY_m,
KEY_n,
KEY_o,
KEY_p,
KEY_q,
KEY_r,
KEY_s,
KEY_t
};

void player::consumeItem(){
  move(0,0);
  clear();
  printw("WHICH ITEM WOULD YOU LIKE TO CONSUME?\n\n");
  for(int i = 0; i < CONSUMABLE_SLOTS; ++i){
    printw("%c: (",letters[i]);
    if(consumables[i] != 0){
      printw("%s",consumables[i]->name.c_str());
    }
    printw(")\n");
  }
  int key;
  int item = 0;
  bool pickedItem = false;
  key = getch();
  while(!pickedItem){
    for(int i = 0; i < CONSUMABLE_SLOTS; ++i){
      if(key == keys[i]){
        item = i;
        pickedItem = true;
      } 
    }
    if(key == KEY_ESC) {
      break;
    }
    else if(!pickedItem){
      key = getch();
    }
  }
  if(pickedItem && consumables[item] != 0){
    move(0,0);
    clrtoeol();
    int health = consumables[item]->attribute;
    if(hp + health > hpmax){
      health = hpmax - hp;
    }
    printw("YOU CONSUME %s AND RESTORE %d HEALTH",consumables[item]->name.c_str(),health);
    hp += health;
    stamina += health;
    delete consumables[item];
    consumables[item] = 0;
  }
  else{
    printLevel();
  }
}

void player::printEquipment(){
  clear();
  move(0,0);
  printw("EQUIPMENT:\n\n");
  printw("a: [WEAPON]  - (");
  if(weapon != 0){
    printw("%s",weapon->name.c_str());
  }
  printw(")\n");
  printw("b: [OFFHAND] - (");
  if(offhand != 0){
    printw("%s",offhand->name.c_str());
  }
  printw(")\n");
  printw("c: [RANGED]  - (");
  if(ranged != 0){
    printw("%s",ranged->name.c_str());
  }
  printw(")\n");
  printw("d: [ARMOR]   - (");
  if(armor != 0){
    printw("%s",armor->name.c_str());
  }
  printw(")\n");
  printw("e: [HELMET]  - (");
  if(helmet != 0){
    printw("%s",helmet->name.c_str());
  }
  printw(")\n");
  printw("f: [CLOAK]   - (");
  if(cloak != 0){
    printw("%s",cloak->name.c_str());
  }
  printw(")\n");
  printw("g: [GLOVES]  - (");
  if(gloves != 0){
    printw("%s",gloves->name.c_str());
  }
  printw(")\n");
  printw("h: [BOOTS]   - (");
  if(boots != 0){
    printw("%s",boots->name.c_str());
  }
  printw(")\n");
  printw("i: [AMULET]  - (");
  if(amulet != 0){
    printw("%s",amulet->name.c_str());
  }
  printw(")\n");
  printw("j: [LIGHT]   - (");
  if(light != 0){
    printw("%s",light->name.c_str());
  }
  printw(")\n");
  printw("k: [RING1]   - (");
  if(ring1 != 0){
    printw("%s",ring1->name.c_str());
  }
  printw(")\n");
  printw("l: [RING2]   - (");
  if(ring2 != 0){
    printw("%s",ring2->name.c_str());
  }
  printw(")\n");
}

void player::compareItem(object* ob){
  if(ob->type == objtype_WEAPON){
    if(weapon != 0){
      if(ob->damage->average() > weapon->damage->average()){
        attron(COLOR_PAIR(GRAS));
      }
      else if(ob->damage->average() < weapon->damage->average()){
        attron(COLOR_PAIR(MONST));
      }
    }
    else{
      attron(COLOR_PAIR(GRAS));
    }
  }
  else if((ob->type == objtype_RANGED || ob->type == objtype_WAND)){
    if(ranged != 0){
      if(ob->damage->average() > ranged->damage->average()){
        attron(COLOR_PAIR(GRAS));
      }
      else if(ob->damage->average() < ranged->damage->average()){
        attron(COLOR_PAIR(MONST));
      }
    }
    else{
      attron(COLOR_PAIR(GRAS));
    }
  }
  else if(ob->type == objtype_OFFHAND){
    if(offhand != 0){
      if(ob->defence > offhand->defence){
        attron(COLOR_PAIR(GRAS));
      }
      else if(ob->defence < offhand->defence){
        attron(COLOR_PAIR(MONST));
      }
    }
    else{
      attron(COLOR_PAIR(GRAS));
    }
  }
  else if(ob->type == objtype_ARMOR){
    if(armor != 0){
      if(ob->defence > armor->defence){
        attron(COLOR_PAIR(GRAS));
      }
      else if(ob->defence < armor->defence){
        attron(COLOR_PAIR(MONST));
      }
    }
    else{
      attron(COLOR_PAIR(GRAS));
    }
  }
  else if(ob->type == objtype_HELMET){
    if(helmet != 0){
      if(ob->defence > helmet->defence){
        attron(COLOR_PAIR(GRAS));
      }
      else if(ob->defence < helmet->defence){
        attron(COLOR_PAIR(MONST));
      }
    }
    else{
      attron(COLOR_PAIR(GRAS));
    }
  }
  else if(ob->type == objtype_GLOVES){
    if(gloves != 0){
      if(ob->defence > gloves->defence){
        attron(COLOR_PAIR(GRAS));
      }
      else if(ob->defence < gloves->defence){
        attron(COLOR_PAIR(MONST));
      }
    }
    else{
      attron(COLOR_PAIR(GRAS));
    }
  }
  else if(ob->type == objtype_BOOTS){
    if(boots != 0){
      if(ob->defence > boots->defence){
        attron(COLOR_PAIR(GRAS));
      }
      else if(ob->defence < boots->defence){
        attron(COLOR_PAIR(MONST));
      }
    }
    else{
      attron(COLOR_PAIR(GRAS));
    }
  }
  else if(ob->type == objtype_LIGHT){
    if(light != 0){
      if(ob->attribute > light->attribute){
        attron(COLOR_PAIR(GRAS));
      }
      else if(ob->attribute < light->attribute){
        attron(COLOR_PAIR(MONST));
      }
    }
    else{
      attron(COLOR_PAIR(GRAS));
    }
  }
}

void player::printInventory(){
  clear();
  move(0,0);
  printw("INVENTORY:\n\n");
  for(int i = 0; i < CARRY_SLOTS; ++i){
    attron(COLOR_PAIR(ILUM));
    printw("%d: [SLOT %d] - (",i,i);
    if(carry[i] != 0){
      compareItem(carry[i]);
      printw("%s",carry[i]->name.c_str());
    }
    attron(COLOR_PAIR(ILUM));
    printw(")\n");
  }
}

void initKnight(){
  main_character->weapon = new object;
  main_character->weapon->type = objtype_WEAPON;
  main_character->weapon->symbol = object_symbol[main_character->weapon->type];
  main_character->weapon->name = "LONG SWORD";
  main_character->weapon->desc = "Reliable and versatile";
  main_character->weapon->weight = 4;
  main_character->weapon->str = 10;
  main_character->weapon->dex = 10;
  main_character->weapon->damage = new dice;
  main_character->weapon->damage->base = 10;
  main_character->weapon->damage->numdice = 1;
  main_character->weapon->damage->numsides = 5;

  main_character->offhand = new object;
  main_character->offhand->type = objtype_OFFHAND;
  main_character->offhand->symbol = object_symbol[main_character->offhand->type];
  main_character->offhand->name = "KITE SHIELD";
  main_character->offhand->desc = "Light weight yet durable";
  main_character->offhand->weight = 2;
  main_character->offhand->str = 10;
  main_character->offhand->dex = 10;
  main_character->offhand->defence = 5;

  main_character->armor = new object;
  main_character->armor->type = objtype_ARMOR;
  main_character->armor->symbol = object_symbol[main_character->armor->type];
  main_character->armor->name = "IRON PLATE ARMOR";
  main_character->armor->desc = "Heavy iron armor";
  main_character->armor->weight = 5;
  main_character->armor->str = 12;
  main_character->armor->dex = 0;
  main_character->armor->defence = 10;

  main_character->helmet = new object;
  main_character->helmet->type = objtype_HELMET;
  main_character->helmet->symbol = object_symbol[main_character->helmet->type];
  main_character->helmet->name = "KNIGHT HELM";
  main_character->helmet->desc = "Complete head protection";
  main_character->helmet->weight = 2;
  main_character->helmet->str = 0;
  main_character->helmet->dex = 0;
  main_character->helmet->defence = 2;

  main_character->gloves = new object;
  main_character->gloves->type = objtype_GLOVES;
  main_character->gloves->symbol = object_symbol[main_character->gloves->type];
  main_character->gloves->name = "KNIGHT GAUNTLETS";
  main_character->gloves->desc = "Pretty handy gauntlets";
  main_character->gloves->weight = 1;
  main_character->gloves->str = 0;
  main_character->gloves->dex = 0;
  main_character->gloves->defence = 1;

  main_character->boots = new object;
  main_character->boots->type = objtype_BOOTS;
  main_character->boots->symbol = object_symbol[main_character->boots->type];
  main_character->boots->name = "IRON BOOTS";
  main_character->boots->desc = "Can't be very comfortable";
  main_character->boots->weight = 2;
  main_character->boots->str = 0;
  main_character->boots->dex = 0;
  main_character->boots->defence = 2;
}

void initBarbarian(){
  main_character->weapon = new object;
  main_character->weapon->type = objtype_WEAPON;
  main_character->weapon->symbol = object_symbol[main_character->weapon->type];
  main_character->weapon->name = "DOUBLE BLADED AXE";
  main_character->weapon->desc = "Difficult to wield yet deals decent damage";
  main_character->weapon->weight = 5;
  main_character->weapon->str = 14;
  main_character->weapon->dex = 8;
  main_character->weapon->damage = new dice;
  main_character->weapon->damage->base = 15;
  main_character->weapon->damage->numdice = 1;
  main_character->weapon->damage->numsides = 8;

  main_character->offhand = new object;
  main_character->offhand->type = objtype_OFFHAND;
  main_character->offhand->symbol = object_symbol[main_character->offhand->type];
  main_character->offhand->name = "WOODEN ROUND SHIELD";
  main_character->offhand->desc = "More for show than for protection";
  main_character->offhand->weight = 2;
  main_character->offhand->str = 5;
  main_character->offhand->dex = 5;
  main_character->offhand->defence = 3;

  main_character->armor = new object;
  main_character->armor->type = objtype_ARMOR;
  main_character->armor->symbol = object_symbol[main_character->armor->type];
  main_character->armor->name = "LEATHER ARMOR";
  main_character->armor->desc = "Light weight and very mobile";
  main_character->armor->weight = 3;
  main_character->armor->str = 6;
  main_character->armor->dex = 0;
  main_character->armor->defence = 7;

  main_character->helmet = new object;
  main_character->helmet->type = objtype_HELMET;
  main_character->helmet->symbol = object_symbol[main_character->helmet->type];
  main_character->helmet->name = "HORNED HELMET";
  main_character->helmet->desc = "What it lacks in protection, it makes up in intimidation";
  main_character->helmet->weight = 1;
  main_character->helmet->str = 0;
  main_character->helmet->dex = 0;
  main_character->helmet->defence = 1;

  main_character->gloves = new object;
  main_character->gloves->type = objtype_GLOVES;
  main_character->gloves->symbol = object_symbol[main_character->gloves->type];
  main_character->gloves->name = "LEATHER GLOVES";
  main_character->gloves->desc = "Protects against calluses";
  main_character->gloves->weight = 1;
  main_character->gloves->str = 0;
  main_character->gloves->dex = 0;
  main_character->gloves->defence = 1;

  main_character->boots = new object;
  main_character->boots->type = objtype_BOOTS;
  main_character->boots->symbol = object_symbol[main_character->boots->type];
  main_character->boots->name = "LEATHER BOOTS";
  main_character->boots->desc = "Stylish and functional";
  main_character->boots->weight = 1;
  main_character->boots->str = 0;
  main_character->boots->dex = 0;
  main_character->boots->defence = 1;
}

void initElf(){
  main_character->weapon = new object;
  main_character->weapon->type = objtype_WEAPON;
  main_character->weapon->symbol = object_symbol[main_character->weapon->type];
  main_character->weapon->name = "ELVEN SHORT SWORD";
  main_character->weapon->desc = "Don't let its size fool you. It can be deadly in the hands of an expert";
  main_character->weapon->weight = 3;
  main_character->weapon->str = 6;
  main_character->weapon->dex = 12;
  main_character->weapon->damage = new dice;
  main_character->weapon->damage->base = 8;
  main_character->weapon->damage->numdice = 1;
  main_character->weapon->damage->numsides = 4;

  main_character->ranged = new object;
  main_character->ranged->type = objtype_RANGED;
  main_character->ranged->symbol = object_symbol[main_character->ranged->type];
  main_character->ranged->name = "SHORT BOW";
  main_character->ranged->desc = "Perfect for mid-range combat";
  main_character->ranged->weight = 2;
  main_character->ranged->damage = new dice;
  main_character->ranged->damage->base = 6;
  main_character->ranged->damage->numdice = 1;
  main_character->ranged->damage->numsides = 10;
  main_character->ranged->attribute = 5;
  main_character->ranged->str = 5;
  main_character->ranged->dex = 12;
  main_character->ranged->defence = 0;

  main_character->armor = new object;
  main_character->armor->type = objtype_ARMOR;
  main_character->armor->symbol = object_symbol[main_character->armor->type];
  main_character->armor->name = "ELVEN LIGHT ARMOR";
  main_character->armor->desc = "Expertly crafted to be the perfect blend of light weight and protection";
  main_character->armor->weight = 2;
  main_character->armor->str = 6;
  main_character->armor->dex = 0;
  main_character->armor->defence = 6;

  main_character->helmet = new object;
  main_character->helmet->type = objtype_HELMET;
  main_character->helmet->symbol = object_symbol[main_character->helmet->type];
  main_character->helmet->name = "ELVEN HELMET";
  main_character->helmet->desc = "Beautiful golden head armor";
  main_character->helmet->weight = 1;
  main_character->helmet->str = 0;
  main_character->helmet->dex = 0;
  main_character->helmet->defence = 2;

  main_character->gloves = new object;
  main_character->gloves->type = objtype_GLOVES;
  main_character->gloves->symbol = object_symbol[main_character->gloves->type];
  main_character->gloves->name = "ARCHER GLOVES";
  main_character->gloves->desc = "You don't want cuts on you fingers";
  main_character->gloves->weight = 1;
  main_character->gloves->str = 0;
  main_character->gloves->dex = 0;
  main_character->gloves->defence = 1;

  main_character->boots = new object;
  main_character->boots->type = objtype_BOOTS;
  main_character->boots->symbol = object_symbol[main_character->boots->type];
  main_character->boots->name = "ELVEN BOOTS";
  main_character->boots->desc = "Master crafted for speed";
  main_character->boots->weight = 1;
  main_character->boots->str = 0;
  main_character->boots->dex = 0;
  main_character->boots->defence = 1;
}

void initMage(){

  main_character->ranged = new object;
  main_character->ranged->type = objtype_WAND;
  main_character->ranged->symbol = object_symbol[main_character->ranged->type];
  main_character->ranged->name = "APPRENTICE WAND";
  main_character->ranged->desc = "Recieved when you enrolled in the Mage College";
  main_character->ranged->weight = 1;
  main_character->ranged->damage = new dice;
  main_character->ranged->damage->base = 6;
  main_character->ranged->damage->numdice = 1;
  main_character->ranged->damage->numsides = 5;
  main_character->ranged->attribute = 5;
  main_character->ranged->str = 0;
  main_character->ranged->dex = 0;
  main_character->ranged->intel = 15;
  main_character->ranged->defence = 0;

  main_character->armor = new object;
  main_character->armor->type = objtype_ARMOR;
  main_character->armor->symbol = object_symbol[main_character->armor->type];
  main_character->armor->name = "APPRENTICE ROBES";
  main_character->armor->desc = "Reinforced with magic to withstand physical attacks";
  main_character->armor->weight = 2;
  main_character->armor->str = 0;
  main_character->armor->dex = 0;
  main_character->armor->intel = 12;
  main_character->armor->defence = 6;

  main_character->gloves = new object;
  main_character->gloves->type = objtype_GLOVES;
  main_character->gloves->symbol = object_symbol[main_character->gloves->type];
  main_character->gloves->name = "CASTER'S GLOVES";
  main_character->gloves->desc = "Don't want to drop your wand";
  main_character->gloves->weight = 1;
  main_character->gloves->str = 0;
  main_character->gloves->dex = 0;
  main_character->gloves->intel = 12;
  main_character->gloves->defence = 1;

  main_character->boots = new object;
  main_character->boots->type = objtype_BOOTS;
  main_character->boots->symbol = object_symbol[main_character->boots->type];
  main_character->boots->name = "MAGE BOOTS";
  main_character->boots->desc = "Comfortable in any casting stance";
  main_character->boots->weight = 1;
  main_character->boots->str = 0;
  main_character->boots->dex = 0;
  main_character->boots->intel = 12;
  main_character->boots->defence = 1;
}

void initOrc(){
  main_character->weapon = new object;
  main_character->weapon->type = objtype_WEAPON;
  main_character->weapon->symbol = object_symbol[main_character->weapon->type];
  main_character->weapon->name = "ORCISH WAR HAMMER";
  main_character->weapon->desc = "Insanely heavy and deals massive damage";
  main_character->weapon->weight = 10;
  main_character->weapon->str = 16;
  main_character->weapon->dex = 0;
  main_character->weapon->damage = new dice;
  main_character->weapon->damage->base = 20;
  main_character->weapon->damage->numdice = 1;
  main_character->weapon->damage->numsides = 10;

  main_character->armor = new object;
  main_character->armor->type = objtype_ARMOR;
  main_character->armor->symbol = object_symbol[main_character->armor->type];
  main_character->armor->name = "ORCISH PLATE ARMOR";
  main_character->armor->desc = "Crude heavy armor";
  main_character->armor->weight = 6;
  main_character->armor->str = 15;
  main_character->armor->dex = 0;
  main_character->armor->defence = 9;

  main_character->helmet = new object;
  main_character->helmet->type = objtype_HELMET;
  main_character->helmet->symbol = object_symbol[main_character->helmet->type];
  main_character->helmet->name = "ORCISH HELMET";
  main_character->helmet->desc = "Intimidating iron helmet";
  main_character->helmet->weight = 4;
  main_character->helmet->str = 0;
  main_character->helmet->dex = 0;
  main_character->helmet->defence = 4;

  main_character->boots = new object;
  main_character->boots->type = objtype_BOOTS;
  main_character->boots->symbol = object_symbol[main_character->boots->type];
  main_character->boots->name = "HEAVY BOOTS";
  main_character->boots->desc = "Made from various, heavy materials";
  main_character->boots->weight = 3;
  main_character->boots->str = 5;
  main_character->boots->dex = 0;
  main_character->boots->defence = 2;
}

void initPeasant(){
  main_character->weapon = new object;
  main_character->weapon->type = objtype_WEAPON;
  main_character->weapon->symbol = object_symbol[main_character->weapon->type];
  main_character->weapon->name = "PITCHFORK";
  main_character->weapon->desc = "Start an angry mob once you're done working on the farm";
  main_character->weapon->weight = 3;
  main_character->weapon->str = 6;
  main_character->weapon->dex = 6;
  main_character->weapon->damage = new dice;
  main_character->weapon->damage->base = 3;
  main_character->weapon->damage->numdice = 1;
  main_character->weapon->damage->numsides = 5;

  main_character->armor = new object;
  main_character->armor->type = objtype_ARMOR;
  main_character->armor->symbol = object_symbol[main_character->armor->type];
  main_character->armor->name = "CLOTH TUNIC";
  main_character->armor->desc = "Is this even considered armor?";
  main_character->armor->weight = 0;
  main_character->armor->str = 6;
  main_character->armor->dex = 0;
  main_character->armor->defence = 1;

  main_character->helmet = new object;
  main_character->helmet->type = objtype_HELMET;
  main_character->helmet->symbol = object_symbol[main_character->helmet->type];
  main_character->helmet->name = "WORKERS HAT";
  main_character->helmet->desc = "Gotta keep the sun out of your eyes somehow";
  main_character->helmet->weight = 1;
  main_character->helmet->str = 0;
  main_character->helmet->dex = 0;
  main_character->helmet->defence = 1;

  main_character->light = new object;
  main_character->light->type = objtype_LIGHT;
  main_character->light->symbol = object_symbol[main_character->light->type];
  main_character->light->name = "TORCH";
  main_character->light->desc = "See farther in the dark";
  main_character->light->weight = 1;
  main_character->light->str = 0;
  main_character->light->dex = 0;
  main_character->light->attribute = 3;

}

void initAutobot(){
  main_character->weapon = new object;
  main_character->weapon->type = objtype_WEAPON;
  main_character->weapon->symbol = object_symbol[main_character->weapon->type];
  main_character->weapon->name = "GIANT CHAINSAW";
  main_character->weapon->desc = "Just not even fair. I thought this game was medieval fantasy";
  main_character->weapon->weight = 10;
  main_character->weapon->str = 40;
  main_character->weapon->dex = 40;
  main_character->weapon->damage = new dice;
  main_character->weapon->damage->base = 100;
  main_character->weapon->damage->numdice = 5;
  main_character->weapon->damage->numsides = 20;

  main_character->ranged = new object;
  main_character->ranged->type = objtype_RANGED;
  main_character->ranged->symbol = object_symbol[main_character->ranged->type];
  main_character->ranged->name = "ROCKET LAUNCHER";
  main_character->ranged->desc = "Again just not even fair";
  main_character->ranged->weight = 25;
  main_character->ranged->damage = new dice;
  main_character->ranged->damage->base = 30;
  main_character->ranged->damage->numdice = 1;
  main_character->ranged->damage->numsides = 10;
  main_character->ranged->attribute = 20;
  main_character->ranged->str = 50;
  main_character->ranged->dex = 50;
  main_character->ranged->defence = 0;

  main_character->armor = new object;
  main_character->armor->type = objtype_ARMOR;
  main_character->armor->symbol = object_symbol[main_character->armor->type];
  main_character->armor->name = "SEMI-TRUCK ARMOR";
  main_character->armor->desc = "Diesel powered chest protection";
  main_character->armor->weight = 20;
  main_character->armor->str = 40;
  main_character->armor->dex = 40;
  main_character->armor->defence = 40;

  main_character->helmet = new object;
  main_character->helmet->type = objtype_HELMET;
  main_character->helmet->symbol = object_symbol[main_character->helmet->type];
  main_character->helmet->name = "ENGINE HELMET";
  main_character->helmet->desc = "Why does it have eyes?";
  main_character->helmet->weight = 10;
  main_character->helmet->str = 20;
  main_character->helmet->dex = 20;
  main_character->helmet->defence = 20;

  main_character->gloves = new object;
  main_character->gloves->type = objtype_GLOVES;
  main_character->gloves->symbol = object_symbol[main_character->gloves->type];
  main_character->gloves->name = "WHEEL WELL GLOVES";
  main_character->gloves->desc = "Comes with stylish wheel bracelets";
  main_character->gloves->weight = 15;
  main_character->gloves->str = 10;
  main_character->gloves->dex = 10;
  main_character->gloves->defence = 15;

  main_character->boots = new object;
  main_character->boots->type = objtype_BOOTS;
  main_character->boots->symbol = object_symbol[main_character->boots->type];
  main_character->boots->name = "WHEEL WELL BOOTS";
  main_character->boots->desc = "I thought heelys went out of style years ago";
  main_character->boots->weight = 10;
  main_character->boots->str = 10;
  main_character->boots->dex = 10;
  main_character->boots->defence = 18;

  main_character->light = new object;
  main_character->light->type = objtype_LIGHT;
  main_character->light->symbol = object_symbol[main_character->light->type];
  main_character->light->name = "HEAD LIGHTS";
  main_character->light->desc = "Massive range light";
  main_character->light->weight = 1;
  main_character->light->str = 0;
  main_character->light->dex = 0;
  main_character->light->attribute = 20;
}

void player::setColor(){
  attron(COLOR_PAIR(color));
}

player *player_constructor(){
  return new player();
}

void player_setXY(int x, int y, player *pc){
  pc->xcoord = x;
  pc->ycoord = y;
}

int player_getX(player *pc) {
  return pc->xcoord;
}

int player_getY(player *pc) {
  return pc->ycoord;
}

void player_setSpeed(int speed, player *pc){
  pc->speed = speed;
}

int player_getSpeed(player *pc){
  return pc->speed;
}

void player_setTurn(int turn, player *pc){
  pc->turn = turn;
}

int player_getTurn(player *pc){
  return pc->turn;
}

void player_setSymbol(char symbol, player *pc){
  pc->symbol = symbol;
}

char player_getSymbol(player *pc){
  return pc->symbol;
}

void player_setStatus(int status, player *pc){
  pc->status = status;
}

int player_getStatus(player *pc){
  return pc->status;
}

void player_move(player *pc){
  pc->pc_move();
}









