#include "core.h"

static int compare(const void *key, const void *with){
  return ((cell_t *) key)->dist - ((cell_t *) with)->dist;
}

static int32_t corridor_path_cmp(const void *key, const void *with) {
  return ((corridor_path_t *) key)->dist - ((corridor_path_t *) with)->dist;
}

void playerDijkstra(int x, int y){

    int i,j;
    heap_t heap;
    corridor_path_t paths[WIDTH][HEIGHT];

    heap_init(&heap,corridor_path_cmp,NULL);

    for(j = 0; j < HEIGHT; ++j){
      for(i = 0; i < WIDTH; ++i){
        if(i == x && j == y){
	  paths[i][j].pos[1] = j;
          paths[i][j].pos[0] = i;
          paths[i][j].cost = 0;
	  paths[i][j].dist = 0;
	  paths[i][j].hn = heap_insert(&heap, &paths[i][j]);
        }
        else if(hardness[i][j] != 0){
	  paths[i][j].pos[0] = i;
          paths[i][j].pos[1] = j;
          paths[i][j].cost = INT_MAX;
          paths[i][j].dist = INT_MAX;
	  paths[i][j].hn = NULL;
        }
        else if(hardness[i][j] == 0){
	  paths[i][j].pos[1] = j;
          paths[i][j].pos[0] = i;
	  paths[i][j].cost = 1;
	  paths[i][j].dist = INT_MAX;
	  paths[i][j].hn = heap_insert(&heap, &paths[i][j]);
        }
      }
    }
 
    corridor_path_t *p;

    while((p = reinterpret_cast<corridor_path_t*>(heap_remove_min(&heap)))){
      
      int ended = 0;
      int xpos = p->pos[0];
      int ypos = p->pos[1];
      int drawn = 0;
      p->hn = NULL;

      if(xpos == x && ypos == y){
        while(drawn == 0){
	  if(roomAndPath[p->pos[0]][p->pos[1]] != ROOM){
	    roomAndPath[p->pos[0]][p->pos[1]] = PATH;
	    hardness[p->pos[0]][p->pos[1]] = 0;
          }
	  p = &paths[p->from[0]][p->from[1]];
	  if(p->pos[0] == x && p->pos[1] ==y){
	    drawn = 1;
	  }
        }
        heap_delete(&heap);
	ended = 1;
        goto END;
      }
      if(ypos > 0){
	if(paths[xpos][ypos-1].hn != NULL){
	  int alt = p->dist + paths[xpos][ypos-1].cost;
	  if(alt < paths[xpos][ypos-1].dist){
	    paths[xpos][ypos-1].dist = alt;
	    paths[xpos][ypos-1].from[0] = xpos;
	    paths[xpos][ypos-1].from[1] = ypos;
	    heap_decrease_key_no_replace(&heap,paths[xpos][ypos-1].hn);
	  }
        }
      }
      if(xpos > 0){
	if(paths[xpos-1][ypos].hn != NULL){
	  int alt = p->dist + paths[xpos-1][ypos].cost;
	  if(alt < paths[xpos-1][ypos].dist){
	    paths[xpos-1][ypos].dist = alt;
	    paths[xpos-1][ypos].from[0] = xpos;
	    paths[xpos-1][ypos].from[1] = ypos;
	    heap_decrease_key_no_replace(&heap,paths[xpos-1][ypos].hn);
	  }
        }
      }
      if(ypos < HEIGHT - 1){
	if(paths[xpos][ypos+1].hn != NULL){
	  int alt = p->dist + paths[xpos][ypos+1].cost;
	  if(alt < paths[xpos][ypos+1].dist){
	    paths[xpos][ypos+1].dist = alt;
	    paths[xpos][ypos+1].from[0] = xpos;
	    paths[xpos][ypos+1].from[1] = ypos;
	    heap_decrease_key_no_replace(&heap,paths[xpos][ypos+1].hn);
	  }
        }
      }
      if(xpos < WIDTH - 1){
	if(paths[xpos+1][ypos].hn != NULL){
	  int alt = p->dist + paths[xpos+1][ypos].cost;
	  if(alt < paths[xpos+1][ypos].dist){
	    paths[xpos+1][ypos].dist = alt;
	    paths[xpos+1][ypos].from[0] = xpos;
	    paths[xpos+1][ypos].from[1] = ypos;
	    heap_decrease_key_no_replace(&heap,paths[xpos+1][ypos].hn);
	  }
        }
      }
    END:if(ended == 1);
    }
}

void pathDijkstra(int corners[numRooms][2], int path_type[4]){
  int r;
  for(r = 0; r < numRooms - 1; ++r){

    int i,j;
    heap_t heap;
    corridor_path_t paths[WIDTH][HEIGHT];

    heap_init(&heap,corridor_path_cmp,NULL);

    for(j = 0; j < HEIGHT; ++j){
      for(i = 0; i < WIDTH; ++i){
        if(i == corners[r][0] && j == corners[r][1]){
	  paths[i][j].pos[1] = j;
          paths[i][j].pos[0] = i;
          paths[i][j].cost = 0;
	  paths[i][j].dist = 0;
	  paths[i][j].hn = heap_insert(&heap, &paths[i][j]);
        }
        else if(hardness[i][j] == HARDEST){
	  paths[i][j].pos[0] = i;
          paths[i][j].pos[1] = j;
          paths[i][j].cost = INT_MAX;
          paths[i][j].dist = INT_MAX;
	  paths[i][j].hn = NULL;
        }
        else if(hardness[i][j] != HARDEST){
	  paths[i][j].pos[1] = j;
          paths[i][j].pos[0] = i;
	  if(hardness[i][j] == 0){
	    paths[i][j].cost = path_type[0];
	  }
          if(hardness[i][j] >= 1 && hardness[i][j]<= 84){
	    paths[i][j].cost = path_type[1];
	  }
	  if(hardness[i][j] >= 85 && hardness[i][j]<= 170){
	    paths[i][j].cost = path_type[2];
	  }
	  if(hardness[i][j] >= 171 && hardness[i][j]<= 254){
	    paths[i][j].cost = path_type[3];
	  }
	  paths[i][j].dist = INT_MAX;
	  paths[i][j].hn = heap_insert(&heap, &paths[i][j]);
        }
      }
    }
 
    corridor_path_t *p;

    while((p = reinterpret_cast<corridor_path_t*>(heap_remove_min(&heap)))){
      
      int ended = 0;
      int xpos = p->pos[0];
      int ypos = p->pos[1];
      int drawn = 0;
      p->hn = NULL;

      if(xpos == corners[r+1][0] && ypos == corners[r+1][1]){
        while(drawn == 0){
	  if(roomAndPath[p->pos[0]][p->pos[1]] != ROOM){
	    roomAndPath[p->pos[0]][p->pos[1]] = PATH;
	    hardness[p->pos[0]][p->pos[1]] = 0;
          }
	  p = &paths[p->from[0]][p->from[1]];
	  if(p->pos[0] == corners[r][0] && p->pos[1] == corners[r][1]){
	    drawn = 1;
	  }
        }
        heap_delete(&heap);
	ended = 1;
        goto END;
      }
      if(ypos > 0){
	if(paths[xpos][ypos-1].hn != NULL){
	  int alt = p->dist + paths[xpos][ypos-1].cost;
	  if(alt < paths[xpos][ypos-1].dist){
	    paths[xpos][ypos-1].dist = alt;
	    paths[xpos][ypos-1].from[0] = xpos;
	    paths[xpos][ypos-1].from[1] = ypos;
	    heap_decrease_key_no_replace(&heap,paths[xpos][ypos-1].hn);
	  }
        }
      }
      if(xpos > 0){
	if(paths[xpos-1][ypos].hn != NULL){
	  int alt = p->dist + paths[xpos-1][ypos].cost;
	  if(alt < paths[xpos-1][ypos].dist){
	    paths[xpos-1][ypos].dist = alt;
	    paths[xpos-1][ypos].from[0] = xpos;
	    paths[xpos-1][ypos].from[1] = ypos;
	    heap_decrease_key_no_replace(&heap,paths[xpos-1][ypos].hn);
	  }
        }
      }
      if(ypos < HEIGHT - 1){
	if(paths[xpos][ypos+1].hn != NULL){
	  int alt = p->dist + paths[xpos][ypos+1].cost;
	  if(alt < paths[xpos][ypos+1].dist){
	    paths[xpos][ypos+1].dist = alt;
	    paths[xpos][ypos+1].from[0] = xpos;
	    paths[xpos][ypos+1].from[1] = ypos;
	    heap_decrease_key_no_replace(&heap,paths[xpos][ypos+1].hn);
	  }
        }
      }
      if(xpos < WIDTH - 1){
	if(paths[xpos+1][ypos].hn != NULL){
	  int alt = p->dist + paths[xpos+1][ypos].cost;
	  if(alt < paths[xpos+1][ypos].dist){
	    paths[xpos+1][ypos].dist = alt;
	    paths[xpos+1][ypos].from[0] = xpos;
	    paths[xpos+1][ypos].from[1] = ypos;
	    heap_decrease_key_no_replace(&heap,paths[xpos+1][ypos].hn);
	  }
        }
      }
    END:if(ended == 1);
    }
  }
}

void fullDijkstra(int valueX, int valueY){

  int i,j;
  heap_t heap;
  cell_t cells[WIDTH][HEIGHT];

  heap_init(&heap,compare,NULL);
  

  for(j = 0; j < HEIGHT; ++j){
    for(i = 0; i < WIDTH; ++i){
      if(i == valueX && j == valueY){
	cells[i][j].x = i;
        cells[i][j].y = j;
        cells[i][j].dist = 0;
	cells[i][j].weight = 0;
	cells[i][j].hn = heap_insert(&heap, &cells[i][j]);
      }
      else if(hardness[i][j] != HARDEST){
        cells[i][j].x = i;
        cells[i][j].y = j;
        cells[i][j].dist = INT_MAX;

	if(hardness[i][j] >= 0 && hardness[i][j]<= 84){
	  cells[i][j].weight = 1;
	}
	if(hardness[i][j] >= 85 && hardness[i][j]<= 170){
	  cells[i][j].weight = 2;
	}
	if(hardness[i][j] >= 171 && hardness[i][j]<= 254){
	  cells[i][j].weight = 3;
	}
	cells[i][j].hn = heap_insert(&heap, &cells[i][j]);
      }
      else if(hardness[i][j] == HARDEST){
	cells[i][j].x = i;
        cells[i][j].y = j;
        cells[i][j].dist = INT_MAX;
	cells[i][j].hn = NULL;
      }
    }
  }
 
  cell_t *u;
  while((u = reinterpret_cast<cell_t*>(heap_remove_min(&heap)))){
    
    int xpos = u->x;
    int ypos = u->y;
    u->hn = NULL;

    for(j = ypos - 1; j <= ypos + 1; ++j){
      for(i = xpos - 1; i <= xpos + 1; ++i){
        if(i < WIDTH - 1 && i > 0 && j < HEIGHT - 1 && j > 0){
	  if(cells[i][j].hn != NULL){
            int alt = u->dist + cells[i][j].weight;
            if(alt < cells[i][j].dist){
              cells[i][j].dist = alt;
              heap_decrease_key_no_replace(&heap,cells[i][j].hn);
            }
	  }
	}
      }
    }
  }

  for(j = 0; j < HEIGHT; ++j){
    for(i = 0; i < WIDTH; ++i){
      all_distances[i][j] = cells[i][j].dist;
    }
  }
}

void rpDijkstra(int valueX, int valueY){

  int i,j;
  heap_t heap;
  cell_t cells[WIDTH][HEIGHT];

  heap_init(&heap,compare,NULL);
  

  for(j = 0; j < HEIGHT; ++j){
    for(i = 0; i < WIDTH; ++i){
      if(i == valueX && j == valueY){
	cells[i][j].x = i;
        cells[i][j].y = j;
        cells[i][j].dist = 0;
	cells[i][j].weight = 0;
	cells[i][j].hn = heap_insert(&heap, &cells[i][j]);
      }
      else if(hardness[i][j] == 0){
        cells[i][j].x = i;
        cells[i][j].y = j;
        cells[i][j].dist = INT_MAX;
	cells[i][j].weight = 1;
	cells[i][j].hn = heap_insert(&heap, &cells[i][j]);
      }
      else if(hardness[i][j] != 0){
	cells[i][j].x = i;
        cells[i][j].y = j;
        cells[i][j].dist = INT_MAX;
	cells[i][j].hn = NULL;
      }
    }
  }
 
  cell_t *u;
  while((u = reinterpret_cast<cell_t*>(heap_remove_min(&heap)))){
    
    int xpos = u->x;
    int ypos = u->y;
    u->hn = NULL;

    for(j = ypos - 1; j <= ypos + 1; ++j){
      for(i = xpos - 1; i <= xpos + 1; ++i){
        if(i < WIDTH - 1 && i > 0 && j < HEIGHT - 1 && j > 0){
	  if(cells[i][j].hn != NULL){
            int alt = u->dist + cells[i][j].weight;
            if(alt < cells[i][j].dist){
              cells[i][j].dist = alt;
              heap_decrease_key_no_replace(&heap,cells[i][j].hn);
            }
	  }
	}
      }
    }
  }

  for(j = 0; j < HEIGHT; ++j){
    for(i = 0; i < WIDTH; ++i){
      rp_distances[i][j] = cells[i][j].dist;
    }
  }
}
