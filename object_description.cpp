#include "core.h"

object_description::object_description(){

}

object* object_description::getObject(){

  object* ret = new object();
  ret->name = name;
  ret->desc = desc;
  ret->type = type;
  ret->color = color;
  ret->damage = damage;
  ret->hit = hit->roll();
  ret->dodge = dodge->roll();
  ret->defence = defence->roll();
  ret->weight = weight->roll();
  ret->speed = speed->roll();
  ret->attribute = attribute->roll();
  ret->value = value->roll();
  ret->symbol = object_symbol[type];
  ret->str = str;
  ret->dex = dex;
  ret->intel = intel;
  return ret;
}

void object_description::printObjectDesc(){
/*  cout << name << endl;
  cout << desc;
  cout << symbol << endl;
  cout << color << endl;
  speed->print_dice();
  cout << endl;
  for(unsigned int i = 0; i < abil->size(); ++i){
    cout << abil->at(i) << " ";
  }
  cout << endl;
  hp->print_dice();
  cout << endl;
  dam->print_dice();
  cout << endl;
  cout << endl;*/
}

#define type_lu_entry(type) { #type, objtype_##type }
static const struct {
  const char *name;
  const object_type_t value;
} types_lookup[] = {
  type_lu_entry(WEAPON),
  type_lu_entry(OFFHAND),
  type_lu_entry(RANGED),
  type_lu_entry(LIGHT),
  type_lu_entry(ARMOR),
  type_lu_entry(HELMET),
  type_lu_entry(CLOAK),
  type_lu_entry(GLOVES),
  type_lu_entry(BOOTS),
  type_lu_entry(AMULET),
  type_lu_entry(RING),
  type_lu_entry(SCROLL),
  type_lu_entry(BOOK),
  type_lu_entry(FLASK),
  type_lu_entry(GOLD),
  type_lu_entry(AMMUNITION),
  type_lu_entry(CONSUMABLE),
  type_lu_entry(WAND),
  type_lu_entry(CONTAINER),
  { 0, objtype_no_type }
};

char object_symbol[] = {
  '*', /* objtype_no_type */
  '|', /* objtype_WEAPON */
  ')', /* objtype_OFFHAND */
  '}', /* objtype_RANGED */
  '_', /* objtype_LIGHT */
  '[', /* objtype_ARMOR */
  ']', /* objtype_HELMET */
  '(', /* objtype_CLOAK */
  '{', /* objtype_GLOVES */
  '\\', /* objtype_BOOTS */
  '"', /* objtype_AMULET */
  '=', /* objtype_RING */
  '~', /* objtype_SCROLL */
  '?', /* objtype_BOOK */
  '!', /* objtype_FLASK */
  '$', /* objtype_GOLD */
  '/', /* objtype_AMMUNITION */
  ',', /* objtype_CONSUMABLE */
  '-', /* objtype_WAND */
  '%', /* objtype_CONTAINER */
};


#define color_lu_entry(color) { #color, COLOR_##color }
static const struct {
  const char *name;
  const uint32_t value;
} colors_lookup[] = {
  /* Same deal here as above in abilities_lookup definition. */
  /* We can use this convenient macro here, but we can't use a *
   * similar macro above because of PASS and TELE.             */
  /* color_lu_entry(BLACK), Can't display COLOR_BLACK */
  "BLACK", COLOR_WHITE,
  color_lu_entry(BLUE),
  color_lu_entry(CYAN),
  color_lu_entry(GREEN),
  color_lu_entry(MAGENTA),
  color_lu_entry(RED),
  color_lu_entry(WHITE),
  color_lu_entry(YELLOW),
  { 0, 0 }
};

vector<object*>* read_obj_file(int numObj, int flag){
  vector<object_description*> ob_vec;
  vector<object*>* ret = new vector<object*>;
  stringstream object_filepath;
  object_filepath << getenv("HOME") << "/.rlg327/object_desc.txt";
  ifstream object_text(object_filepath.str().c_str());
  if(!object_text){
    cerr << "unable to open file object_desc.txt\n";
    exit(1);
  }
  string line;
  while(getline(object_text,line)){
    if(!line.compare("BEGIN OBJECT")){
      object_description* ob = new object_description();
      getline(object_text,line);
      while(line.compare("END")){
	int index = line.find(" ");
	if(!line.substr(0,3).compare("DESC")){
	  line += " ";
	}
	string sub = line.substr(0,index);
	if(!sub.compare("NAME")){
	  string name = line.substr(index + 1, string::npos);
	  ob->name = name;
	}
	else if(!sub.compare("COLOR")){
	  string color_s = line.substr(index + 1, string::npos);
          if(color_s == "BROWN"){
      	    ob->color = COLOR_ROCK_ILUM;
	  }
	  else{
  	    for (int i = 0; colors_lookup[i].name; i++) {
    	      if (color_s == colors_lookup[i].name) {
      	        ob->color = colors_lookup[i].value;
      	        break;
    	      }
              else if(color_s == "BROWN"){
      	        ob->color = R_ILUM;
      	        break;
	      }
  	    }
          }
	}
	else if(!sub.compare("DESC")){
	  string desc;
	  getline(object_text,line);
	  while(line.compare(".")){
	    desc += line; 
	    desc += "\n";
	    getline(object_text,line);
	  }
	  ob->desc = desc;
	}
	else if(!sub.compare("TYPE")){
	  string type = line.substr(index + 1, string::npos);
  	  for (int i = 0; types_lookup[i].name; i++) {
    	    if (type == types_lookup[i].name) {
      	      ob->type = types_lookup[i].value;
      	      break;
    	    }
  	  }
	}
	else if(!sub.compare("REQ")){
	  string req_str = line.substr(index + 1, string::npos);
	  int i = req_str.find(" ");
	  ob->str = atoi(req_str.substr(0,i).c_str());
	  string new_req_str = req_str.substr(i + 1, string::npos);
	  int d = new_req_str.find(" ");
	  ob->dex = atoi(new_req_str.substr(0,d).c_str());
	  ob->intel = atoi(new_req_str.substr(d+1,string::npos).c_str());
	}
	else if(!sub.compare("HIT")){
	  dice* speed_dice = new dice();
	  string dice_str = line.substr(index + 1, string::npos);
	  int i = dice_str.find("+");
	  speed_dice->base = atoi(dice_str.substr(0,i).c_str());
	  int d = dice_str.find("d");
	  speed_dice->numdice = atoi(dice_str.substr(i+1,d).c_str());
	  speed_dice->numsides = atoi(dice_str.substr(d+1,string::npos).c_str());
	  ob->hit = speed_dice;
	}
	else if(!sub.compare("DAM")){
	  dice* speed_dice = new dice();
	  string dice_str = line.substr(index + 1, string::npos);
	  int i = dice_str.find("+");
	  speed_dice->base = atoi(dice_str.substr(0,i).c_str());
	  int d = dice_str.find("d");
	  speed_dice->numdice = atoi(dice_str.substr(i+1,d).c_str());
	  speed_dice->numsides = atoi(dice_str.substr(d+1,string::npos).c_str());
	  ob->damage = speed_dice;
	}
	else if(!sub.compare("ATTR")){
	  dice* speed_dice = new dice();
	  string dice_str = line.substr(index + 1, string::npos);
	  int i = dice_str.find("+");
	  speed_dice->base = atoi(dice_str.substr(0,i).c_str());
	  int d = dice_str.find("d");
	  speed_dice->numdice = atoi(dice_str.substr(i+1,d).c_str());
	  speed_dice->numsides = atoi(dice_str.substr(d+1,string::npos).c_str());
	  ob->attribute = speed_dice;
	}
	else if(!sub.compare("VAL")){
	  dice* speed_dice = new dice();
	  string dice_str = line.substr(index + 1, string::npos);
	  int i = dice_str.find("+");
	  speed_dice->base = atoi(dice_str.substr(0,i).c_str());
	  int d = dice_str.find("d");
	  speed_dice->numdice = atoi(dice_str.substr(i+1,d).c_str());
	  speed_dice->numsides = atoi(dice_str.substr(d+1,string::npos).c_str());
	  ob->value = speed_dice;
	}
	else if(!sub.compare("DODGE")){
	  dice* speed_dice = new dice();
	  string dice_str = line.substr(index + 1, string::npos);
	  int i = dice_str.find("+");
	  speed_dice->base = atoi(dice_str.substr(0,i).c_str());
	  int d = dice_str.find("d");
	  speed_dice->numdice = atoi(dice_str.substr(i+1,d).c_str());
	  speed_dice->numsides = atoi(dice_str.substr(d+1,string::npos).c_str());
	  ob->dodge = speed_dice;
	}
	else if(!sub.compare("DEF")){
	  dice* speed_dice = new dice();
	  string dice_str = line.substr(index + 1, string::npos);
	  int i = dice_str.find("+");
	  speed_dice->base = atoi(dice_str.substr(0,i).c_str());
	  int d = dice_str.find("d");
	  speed_dice->numdice = atoi(dice_str.substr(i+1,d).c_str());
	  speed_dice->numsides = atoi(dice_str.substr(d+1,string::npos).c_str());
	  ob->defence = speed_dice;
	}
	else if(!sub.compare("SPEED")){
	  dice* speed_dice = new dice();
	  string dice_str = line.substr(index + 1, string::npos);
	  int i = dice_str.find("+");
	  speed_dice->base = atoi(dice_str.substr(0,i).c_str());
	  int d = dice_str.find("d");
	  speed_dice->numdice = atoi(dice_str.substr(i+1,d).c_str());
	  speed_dice->numsides = atoi(dice_str.substr(d+1,string::npos).c_str());
	  ob->speed = speed_dice;
	}
	else if(!sub.compare("WEIGHT")){
	  dice* speed_dice = new dice();
	  string dice_str = line.substr(index + 1, string::npos);
	  int i = dice_str.find("+");
	  speed_dice->base = atoi(dice_str.substr(0,i).c_str());
	  int d = dice_str.find("d");
	  speed_dice->numdice = atoi(dice_str.substr(i+1,d).c_str());
	  speed_dice->numsides = atoi(dice_str.substr(d+1,string::npos).c_str());
	  ob->weight = speed_dice;
	}
	getline(object_text,line);
      }
      ob_vec.push_back(ob);
      getline(object_text,line);
    }
  }
  int i = 0;
  while( i < numObj){
    int randob = rand() % ob_vec.size();
    if(flag == FLOOR_OBJS){
      if(dungeonLevel == 0 && (ob_vec.at(randob)->type == objtype_CONSUMABLE || 
		ob_vec.at(randob)->type == objtype_SCROLL || ob_vec.at(randob)->type == objtype_GOLD ||
		ob_vec.at(randob)->type == objtype_AMMUNITION)){
        ret->push_back(ob_vec.at(randob)->getObject());
        ++i;
      }
      else if(dungeonLevel != 0 && ob_vec.at(randob)->type != objtype_CONSUMABLE && (rand() % 10 == 0
				||(ob_vec.at(randob)->str <= OBJECT_SCALAR * abs(dungeonLevel) 
				&& ob_vec.at(randob)->dex <= OBJECT_SCALAR * abs(dungeonLevel)
				&& ob_vec.at(randob)->intel <= OBJECT_SCALAR * abs(dungeonLevel)))){
        ret->push_back(ob_vec.at(randob)->getObject());
        ++i;
      }
    }
    else if(flag == MON_DROP){
      if(rand() % 10 == 0 || (ob_vec.at(randob)->type == objtype_CONSUMABLE || 
		ob_vec.at(randob)->type == objtype_GOLD ||
		ob_vec.at(randob)->type == objtype_AMMUNITION)){
        ret->push_back(ob_vec.at(randob)->getObject());
        ++i;
      }
    }
  }
  object_text.close();
  return ret;
}
