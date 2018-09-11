#include "core.h"

dice::dice(){

}

void dice::print_dice(){
  cout << base << "+" << numdice << "d" << numsides;
}

int dice::roll(){
  int rolled = base;
  for(int i = 0; i < numdice; ++i){
    rolled += rand() % numsides;
  }
  return rolled;
}

int dice::average(){
  return numdice * ((numsides + 1) / 2) + base; 
}
