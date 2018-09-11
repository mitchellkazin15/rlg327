typedef enum dim {
  dim_x,
  dim_y,
  num_dims
} dim_t;

typedef struct cell{
  int x;
  int y;
  int dist;
  int weight;
  heap_node_t *hn;
} cell_t;

typedef struct corridor_path {
  heap_node_t *hn;
  uint8_t pos[2];
  uint8_t from[2];
  int32_t cost;
  int dist;
} corridor_path_t;

void playerDijkstra(int x, int y);
void pathDijkstra(int corners[numRooms][2], int path_type[4]);
void fullDijkstra(int valueX, int valueY);
void rpDijkstra(int valueX, int valueY);


