enum event_type{
  PLAYER,
  MONSTER
};

typedef struct event{
  unsigned int time;
  enum event_type type;
  union{
    player *pc;
    monster *npc;
  };
} event_t;

void moveOnGradient(event_t *u);
void moveOnGradientTunneling(event_t *u);
int canSee(event_t *u);
void randomMovePC(event_t *u);
void quitGame();
void enterLookMode(int lookX, int lookY);
void movePC(event_t *u);
void move_0(event_t *u);
void move_1(event_t *u);
void move_2(event_t *u);
void move_3(event_t *u);
void move_4(event_t *u);
void move_5(event_t *u);
void move_6(event_t *u);
void move_7(event_t *u);
void move_8(event_t *u);
void move_9(event_t *u);
void move_a(event_t *u);
void move_b(event_t *u);
void move_c(event_t *u);
void move_d(event_t *u);
void move_e(event_t *u);
void move_f(event_t *u);
int isDwarf(int mtype);
void simulateGame(int numMon);
void fogOfWar(int radius);

