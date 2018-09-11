#include "core.h"

static int event_cmp(const void *key, const void *with){
  return ((event_t *) key)->time - ((event_t *) with)->time;
}

void quitGame(){
  endwin();
  exit(0);

}

void enterLookMode(int lookX, int lookY){
  ncursesPrint(lookX,lookY);
  move(0,0);
  attron(COLOR_PAIR(ILUM));
  printw("LOOK MODE:  (ESC TO EXIT)\n");
  int key;
  GET_KEY:key = getch();
  while(key != KEY_ESC && key != KEY_L){
    if(key == KEY_8 || key == KEY_k){
      if(lookY > 0){
        lookY = lookY - 1;
      }
    }
    else if(key == KEY_6 || key == KEY_l){
      if(lookX < WIDTH - 1){
        lookX = lookX + 1;
      }
    }
    else if(key == KEY_2 || key == KEY_j){
      if(lookY < HEIGHT - 1){
        lookY = lookY + 1;
      }
    }
    else if(key == KEY_4 || key == KEY_h){
      if(lookX > 0){
        lookX = lookX - 1;
      }
    }
    else if(key == KEY_i){
      if(dungeonLevel > 0){
        fogOfWar(FOREST_SIGHT_RADIUS);
      }
      else if(dungeonLevel < 0){
        fogOfWar(FOREST_SIGHT_RADIUS);
      }
      if(dungeonLevel != 0 && seen[lookX][lookY] == VISIBLE && mondun[lookX][lookY] != 0){
	move(0,0);
        mondun[lookX][lookY]->print_monster();
	getch();
      }
      else if(dungeonLevel == 0 && mondun[lookX][lookY] != 0){
	move(0,0);
	attron(COLOR_PAIR(ILUM));
        mondun[lookX][lookY]->print_monster();
	getch();
      }
      else if(objdun[lookX][lookY] != 0){
	move(0,0);
	attron(COLOR_PAIR(ILUM));
	objdun[lookX][lookY]->printObject();
        getch();
      }
    }
    else{
      goto GET_KEY;
    }
    ncursesPrint(lookX,lookY);
    move(0,0);
    printw("LOOK MODE:  (ESC TO EXIT)\n");
    key = getch();
  }
  printLevel();
}

int offsets[8][2] = 
  {
    {-1,-1},
    {0,-1},
    {1,-1},
    {-1,0},
    {1,0},
    {-1,1},
    {0,1},
    {1,1}
  };

void updateMonsterArrays(event_t *u){
  
  int xpos = u->npc->xcoord;
  int ypos = u->npc->ycoord;
  if(mondun[xpos][ypos] != 0){
    monster* displaced = mondun[xpos][ypos];
    int i = rand() % 8;
    for(;;){
      if(i == 8){
	i = 0;
      }
      int newx = xpos + offsets[i][0];
      int newy = ypos + offsets[i][1];
      if(mondun[newx][newy] == 0 && hardness[newx][newy] == 0 && !(newx == pcX && newy == pcY)){
	displaced->xcoord = newx;
	displaced->ycoord = newy;
	mondun[newx][newy] = displaced;
	characterMap[newx][newy] = displaced->symbol;
	break;
      }
      ++i;
    }
  }
  characterMap[xpos][ypos] = u->npc->symbol;
  mondun[xpos][ypos] = u->npc;
}

void bresenham(int radius, int x0, int y0, int x1, int y1){
  if(main_character->light != 0){
    radius += main_character->light->attribute;
  }
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  if((dx * dx + dy * dy) > radius * radius){
    return;
  }
  int err = (dx>dy ? dx : -dy)/2, e2;
  for(;;){
    if(hardness[x0][y0] != 0){
      seen[x0][y0] = VISIBLE;
      return;
    }
    seen[x0][y0] = VISIBLE;
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void fogOfWar(int radius){
  int i, j;
  for(i = 0; i < WIDTH; i++){
    for(j = 0; j < HEIGHT; ++j){
      bresenham(radius,pcX,pcY,i,j);
    }
  }
}

string dwarfNames[41] = 
{
  "GOMO",
  "CHALRAK",
  "DURUNN",
  "BILUR",
  "GORAK",
  "GIMO",
  "STORGOL",
  "RUNUM",
  "THRLIN",
  "ALDIN",
  "ARNGRIM",
  "BALDRICK",
  "BALIN",
  "BALZUD",
  "DADRIN",
  "DURAK",
  "DURGRIM",
  "FALAGRIM",
  "FODRIN",
  "GOREK",
  "GROGAN",
  "GORRIN",
  "HURGIN",
  "HEGANBOR",
  "HOLGAR",
  "KADRIN",
  "KIMRIL",
  "KURGAN",
  "LOTHOR",
  "MORDIN",
  "OKRI",
  "OLDOR",
  "OREK",
  "ROREK",
  "RAREK",
  "SKALDOR",
  "THOREK",
  "THORGRIM",
  "ULRIK",
  "ULFAR",
  "YORRI"
};

string dwarfLands[10] =
{
  "DORDUUM",
  "NORVOGRIM",
  "BELHAGAK",
  "KILMEK",
  "DORUL",
  "OLUHM",
  "TARUM",
  "THURUM",
  "DHALGRAN",
  "KUL"
};

string monsterNames[NUM_M_TYPES] = 
{
  " TROLL",               //no attributes (random motion)
  "N ORC",                //only smart
  " ZOMBIE",              //only telepathic
  " HELLHOUND",           //smart + telepathic
  " DWARF",               //only tunneling
  " DWARVEN WARRIOR",     //smart + tunneling
  " GIANT MOLE",          //telepathic + tunneling
  " DWARVEN SCOUT",       //smart + telepathic + tunneling
  " CAVE TROLL",          //only erratic
  " GOBLIN",              //smart + erratic
  " VAMPIRE BAT",         //telepathic + erratic
  " GIANT SPIDER",        //smart + telepathic + erratic
  " DWARVEN MINER",       //tunneling + erratic
  " DWARVEN SENTRY",      //smart + tunneling + erratic
  " GIANT CAVE WORM",     //telepathic + tunneling + erratic
  "N UNDEAD DWARVEN SCOUT"//smart + telepathic + tunneling + erratic
};

char monCharArray[NUM_M_TYPES] =
{
  '0', //"TROLL",               //no attributes (random motion)
  '1', //"ORC",                 //only smart
  '2', //"ZOMBIE",              //only telepathic
  '3', //"HELLHOUND",           //smart + telepathic
  '4', //"DWARF",               //only tunneling
  '5', //"DWARVEN WARRIOR",     //smart + tunneling
  '6', //"GIANT MOLE",          //telepathic + tunneling
  '7', //"DWARVEN SCOUT"        //smart + telepathic + tunneling
  '8', //"CAVE TROLL",          //only erratic
  '9', //"GOBLIN",              //smart + erratic
  'a', //"VAMPIRE BAT",         //telepathic + erratic
  'b', //"GIANT SPIDER",        //smart + telepathic + erratic
  'c', //"DWARVEN MINER",       //tunneling + erratic
  'd', //"DWARVEN SENTRY",      //smart + tunneling + erratic
  'e', //"GIANT CAVE WORM",     //telepathic + tunneling + erratic
  'f'  //"UNDEAD DWARVEN SCOUT" //smart + telepathic + tunneling + erratic
};

int isDwarf(int mtype){
  if(mtype == 4 || mtype == 5 || mtype == 7 || mtype == 12 || mtype == 13){
    return 1;
  }
  else{
    return 0;
  }
}

object* getBossObject(){
  return read_obj_file(1,MON_DROP)->at(0);
}

void simulateGame(int numMon){
  int i,j;
  vector<monster*>* monvec = read_monster_file(numMon);
  vector<object*>* objvec = read_obj_file(numObjs,FLOOR_OBJS);
  printLevel();
  objvec->size();
  for(i = 0; i < WIDTH; ++i){
    for(j = 0; j < HEIGHT; ++j){
      characterMap[i][j] = (char) 0;
      objectMap[i][j] = (char) 0;
      mondun[i][j] = 0;
      objdun[i][j] = 0;
    }
  }
  i = 0;
  while(i < numMon){
    int monRoom = (rand() % (numRooms - 1)) + 1;
    if(dungeonLevel != 0 || monvec->at(i)->symbol == '@'){
      monvec->at(i)->xcoord = (rand() % (roomDimensions[monRoom][2] - 2)) + 
		      			    roomDimensions[monRoom][0] + 1;
      monvec->at(i)->ycoord = (rand() % (roomDimensions[monRoom][3] - 2)) + 
		      			    roomDimensions[monRoom][1] + 1;
    }
    else{
      monvec->at(i)->xcoord = (rand() % (WIDTH - 10)) + 5;
      monvec->at(i)->ycoord = (rand() % (HEIGHT - 10)) + 5;
    }
    if(characterMap[monvec->at(i)->xcoord][monvec->at(i)->ycoord] == (char) 0 
        && hardness[monvec->at(i)->xcoord][monvec->at(i)->ycoord] == 0
	&& monvec->at(i)->xcoord != pcX && monvec->at(i)->ycoord != pcY){
      characterMap[monvec->at(i)->xcoord][monvec->at(i)->ycoord] = monvec->at(i)->symbol;
      mondun[monvec->at(i)->xcoord][monvec->at(i)->ycoord] = monvec->at(i);
      monvec->at(i)->status = ALIVE;
      monvec->at(i)->turn = 1;
      ++i;
    }
  }
  i = 0;
  while(i < numObjs){
    int monRoom = (rand() % (numRooms - 1)) + 1;
    objvec->at(i)->xcoord = (rand() % (roomDimensions[monRoom][2] - 2)) + 
		      			    roomDimensions[monRoom][0] + 1;
    objvec->at(i)->ycoord = (rand() % (roomDimensions[monRoom][3] - 2)) + 
		      			    roomDimensions[monRoom][1] + 1;
    if(objectMap[objvec->at(i)->xcoord][objvec->at(i)->ycoord] == (char) 0
	&& staircases[objvec->at(i)->xcoord][objvec->at(i)->ycoord] == (char) 0 
        && hardness[objvec->at(i)->xcoord][objvec->at(i)->ycoord] == 0
	&& objvec->at(i)->xcoord != pcX && objvec->at(i)->ycoord != pcY){
      objectMap[objvec->at(i)->xcoord][objvec->at(i)->ycoord] = objvec->at(i)->symbol;
      objdun[objvec->at(i)->xcoord][objvec->at(i)->ycoord] = objvec->at(i);
      ++i;
    }
  }

  monsleft = numMon;

  main_character->xcoord = pcX;
  main_character->ycoord = pcY;
  main_character->symbol = PC;
  main_character->status = ALIVE;
  main_character->turn = 0;
  characterMap[pcX][pcY] = PC;

  heap_t eventHeap;
  heap_init(&eventHeap,event_cmp,0);
  
  event_t pc_event;
  pc_event.type = PLAYER;
  pc_event.pc = main_character;
  pc_event.time = (1000 / main_character->speed) * main_character->turn;
  ++main_character->turn;
  heap_insert(&eventHeap, &pc_event);

  event_t mon_events[numMon];
  for(i = 0; i < numMon; ++i){
    mon_events[i].type = MONSTER;
    mon_events[i].npc = monvec->at(i);
    mon_events[i].time = (1000 / monvec->at(i)->speed) * monvec->at(i)->turn;
    ++monvec->at(i)->turn;
    heap_insert(&eventHeap,&mon_events[i]);
  }

  int won = 0;
  event_t *u;
  while((u = reinterpret_cast<event_t*>(heap_remove_min(&eventHeap)))){
    if(u->type == PLAYER){
      if(player_getStatus(u->pc) == ALIVE){
	  ncursesPrint(pcX,pcY);
          main_character->pc_move();
	  if(!isFrozen){
	    ++main_character->turn;
	  }
	  else{
	    ++frozenTurn;
	    if(frozenTurn == FROZEN_LENGTH){
	      isFrozen = false;
	      main_character->turn += FROZEN_LENGTH;
	      move(0,0);
	      clrtoeol();
	      printw("TIME UNFREEZES");
	    }
	  }
	  u->time = (1000 / main_character->speed) * main_character->turn;
	  refresh();
	  if(dungeonLevel >= 0 && staircases[pcX][pcY] == UP_STAIRCASE && nextDungeon != 1){
	    nextDungeon = 1;
	    writeToFile();
	    ++dungeonLevel;
	    
          }
	  else if(dungeonLevel > 0 && staircases[pcX][pcY] == DOWN_STAIRCASE && nextDungeon != 1){
	    nextDungeon = 1;
	    writeToFile();
	    --dungeonLevel;
          }
	  if(nextDungeon == 1 || quit == 1){
	    heap_delete(&eventHeap);
	  }
	  else if(mondun[pcX][pcY] != 0){
	    main_character->experience += mondun[pcX][pcY]->exp;
	    mondun[pcX][pcY]->status = DEAD;
	    mondun[pcX][pcY] = 0;
	    ++killed;
	    --monsleft;
          }
	  if(won == 0 && nextDungeon == 0 && quit == 0){ 
            heap_insert(&eventHeap,u);
            fullDijkstra(pcX,pcY);
            rpDijkstra(pcX,pcY);
	    refresh();
	  }
      }
    }
    else{
      if(monster_getStatus(u->npc) == ALIVE){
	mondun[u->npc->xcoord][u->npc->ycoord] = 0;
        u->npc->mon_move();
	updateMonsterArrays(u);
	monster_setTurn(monster_getTurn(u->npc) + 1, u->npc);
        u->time = (1000 / monster_getSpeed(u->npc)) * monster_getTurn(u->npc);
        heap_insert(&eventHeap,u);
        if(main_character->hp <= 0){
          main_character->hp = 0;
	  main_character->status = DEAD;
          clear();
	  //characterMap[pcX][pcY] = (char) 0;
          ncursesPrint(pcX,pcY);
          refresh();
	  attron(COLOR_PAIR(ILUM));
          move(0,0);
	  printw("YOU WERE KILLED BY %s..."
		"YOU KILLED %d MONSTERS"
			,u->npc->name.c_str(),
				killed);
	  refresh();
          heap_delete(&eventHeap);
        }
      }
      else{
        if(u->npc->boss != 0){
	  objdun[boss->xcoord][boss->ycoord] = getBossObject();
          objectMap[boss->xcoord][boss->ycoord] = objdun[boss->xcoord][boss->ycoord]->symbol;
	  ncursesPrint(pcX,pcY);
	  move(0,0);
	  clrtoeol();
	  printw("YOU HAVE DEFEATED %s",boss->name.c_str());
	  getch();
	  if(dungeonLevel == firstBossLevel){
	    move(0,0);
	    clrtoeol();
	    printw("THE COLONY DETERIORATES");
	    for(i = 0; i < WIDTH; ++i){
	      for(j = 0; j < HEIGHT; ++j){
		if(mondun[i][j] != 0){
		  characterMap[i][j] = (char) 0;
		  mondun[i][j]->status = DEAD;
	          mondun[i][j] = 0;
		}
	      }
	    }
	    monsleft = 0;
	    getch();
	  }
	  move(0,0);
	  clrtoeol();
	  printw("%s DROPS %s",boss->name.c_str(),objdun[boss->xcoord][boss->ycoord]->name.c_str());
	  getch();
	  move(0,0);
	  clrtoeol();
	  if(dungeonLevel == firstBossLevel){
            if(staircases[pcX][pcY] != UP_STAIRCASE){
	      printw("A STAIR CASE APPEARS BENEATH YOU");
	      staircases[pcX][pcY] = DOWN_STAIRCASE;
            }
	    else{
	      bool broken = false;
	      for(i = pcX - 1; i < pcX + 1; ++i){
	        for(j = pcY - 1; j < pcY + 1; ++j){
		  if(hardness[i][j] == 0 && !(i == pcX && j == pcY)){
	    	    printw("A STAIR CASE APPEARS NEAR YOU");
	  	    staircases[i][j] = DOWN_STAIRCASE;
		    broken = true;
                    break;
                  }
	        }
	        if(broken){
		  break;
	        }
	      }
            }
	    getch();
	  }
	  boss = 0;
	  printLevel();
        }
	delete u->npc;
      }
    }
  }
}

