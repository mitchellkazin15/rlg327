#ifndef CHARACTER_H
#define CHARACTER_H

class Character 
{
public:
  int xcoord;
  int ycoord;
  int speed;
  int status;
  int turn;
  char symbol;

public:
  Character(int x, int y);
  virtual void move();
  virtual void setXY(int x, int y);
  int getX() {return ycoord;}
  int getY() {return xcoord;}
  virtual void setSpeed(int speed);
  int getSpeed() {return this->speed;}
  virtual void setTurn(int turn);
  int getTurn() {return this->turn;}
  virtual void setSymbol(char symbol);
  char getSymbol()  {return this->symbol;}
  virtual void setStatus(int status);
  int getStatus()  {return this->status;}
  
};

#endif
