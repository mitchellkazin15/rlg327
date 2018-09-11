#include "core.h"

monster_desc::monster_desc(){
  abil = new vector<string>;
  aggressive = true;
  boss = 0;
}

monster* monster_desc::getMonster(){
  monster* mon = new monster;
  mon->name = name;
  mon->desc = desc;
  mon->speed = speed->roll();
  mon->hp = hp->roll();
  mon->hpmax = mon->hp;
  mon->damage = dam;
  mon->symbol = symbol;
  mon->colors = color;
  mon->m_type = 0;
  mon->pass = false;
  mon->pickup = false;
  mon->destroy = false;
  mon->aggressive = aggressive;
  mon->boss = boss;
  mon->lastSeenX = 0;
  mon->lastSeenY = 0;
  mon->difficulty = mon->speed + mon->hp + mon->damage->average();
  mon->exp = mon->difficulty;
  if(mon->boss != 0){
    mon->exp = mon->exp * BOSS_EXP_MULTIPLIER;
  }
  for(unsigned int i = 0; i < abil->size(); ++i){
    if(abil->at(i) == "SMART"){
      mon->m_type += 1;
    }
    else if(abil->at(i) == "TELE"){
      mon->m_type += 2;
    }
    else if(abil->at(i) == "TUNNEL"){
      mon->m_type += 4;
    }
    else if(abil->at(i) == "ERRATIC"){
      mon->m_type += 8;
    }
    else if(abil->at(i) == "PASS"){
      mon->pass = true;
    }
  }
  return mon;
} 

int monster_desc::getDifficulty(){
  return hp->average() + speed->average() + dam->average();
}

void monster_desc::print_monster_desc(){
  cout << name << endl;
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
  cout << endl;
}

vector<monster*>* read_monster_file(int numMon){
  vector<monster_desc*> mon_vec;
  vector<monster*>* ret = new vector<monster*>;
  stringstream monster_filepath;
  monster_filepath << getenv("HOME") << "/.rlg327/";
  if(dungeonLevel == 0){
    monster_filepath << "villager_desc.txt";
  }
  else if(dungeonLevel > 0){
    monster_filepath << "forest_monster_desc.txt";
  }
  else if(dungeonLevel < 0){
    monster_filepath << "monster_desc.txt";
  }
  ifstream monster_text(monster_filepath.str().c_str());
  if(!monster_text){
    cerr << "unable to open file " << monster_filepath.str().c_str();
    exit(1);
  }
  string line;
  while(getline(monster_text,line)){
    if(!line.compare("BEGIN MONSTER")){
      monster_desc* mon = new monster_desc();
      getline(monster_text,line);
      while(line.compare("END")){
	int index = line.find(" ");
	if(!line.substr(0,3).compare("DESC")){
	  line += " ";
	}
	string sub = line.substr(0,index);
	if(!sub.compare("NAME")){
	  string name = line.substr(index + 1, string::npos);
	  mon->name = name;
	}
	else if(!sub.compare("SYMB")){
	  char symb = line.at(index + 1);
	  mon->symbol = symb;
	}
	else if(!sub.compare("PEACE")){
	  mon->aggressive = false;
	}
	else if(!sub.compare("COLOR")){
	  string color = line.substr(index + 1, string::npos);
	  mon->color = color;
	}
	else if(!sub.compare("ABIL")){
	  string abilities = line.substr(index + 1, string::npos);
	  abilities += " ";
	  while(abilities.size() > 0){
	    int i = abilities.find(" ");
	    string abil = abilities.substr(0,i);
	    mon->abil->push_back(abil);
	    abilities = abilities.substr(i + 1, string::npos);
	  }
	}
	else if(!sub.compare("DESC")){
	  string desc;
	  getline(monster_text,line);
	  while(line.compare(".")){
	    desc += line; 
	    desc += "\n";
	    getline(monster_text,line);
	  }
	  mon->desc = desc;
	}
	else if(!sub.compare("SPEED")){
	  dice* speed_dice = new dice();
	  string dice_str = line.substr(index + 1, string::npos);
	  int i = dice_str.find("+");
	  speed_dice->base = atoi(dice_str.substr(0,i).c_str());
	  int d = dice_str.find("d");
	  speed_dice->numdice = atoi(dice_str.substr(i+1,d).c_str());
	  speed_dice->numsides = atoi(dice_str.substr(d+1,string::npos).c_str());
	  mon->speed = speed_dice;
	}
	else if(!sub.compare("BOSS")){
	  int boss = 0;
	  string boss_str = line.substr(index + 1, string::npos);
	  boss = atoi(boss_str.c_str());
	  mon->boss = boss;
	}
	else if(!sub.compare("HP")){
	  dice* hp_dice = new dice();
	  string dice_str = line.substr(index + 1, string::npos);
	  int i = dice_str.find("+");
	  hp_dice->base = atoi(dice_str.substr(0,i).c_str());
	  int d = dice_str.find("d");
	  hp_dice->numdice = atoi(dice_str.substr(i+1,d).c_str());
	  hp_dice->numsides = atoi(dice_str.substr(d+1,string::npos).c_str());
	  mon->hp = hp_dice;
	}
	else if(!sub.compare("DAM")){
	  dice* dam_dice = new dice();
	  string dice_str = line.substr(index + 1, string::npos);
	  int i = dice_str.find("+");
	  dam_dice->base = atoi(dice_str.substr(0,i).c_str());
	  int d = dice_str.find("d");
	  dam_dice->numdice = atoi(dice_str.substr(i+1,d).c_str());
	  dam_dice->numsides = atoi(dice_str.substr(d+1,string::npos).c_str());
	  mon->dam = dam_dice;
	}
	getline(monster_text,line);
      }
      mon_vec.push_back(mon);
      getline(monster_text,line);
    }
  }
  int i = 0;
  bool bossSpawned = false;
  while(i < numMon){
    int randmon = rand() % mon_vec.size();
    if(dungeonLevel == 0){
      ret->push_back(mon_vec.at(randmon)->getMonster());
      ++i;
    }
    else if(isBossLevel()){
      if(mon_vec.at(randmon)->boss == 1 && !bossSpawned && dungeonLevel == firstBossLevel){
        boss = mon_vec.at(randmon)->getMonster();
        ret->push_back(boss);
        ++i;
	bossSpawned = true;
      }
      else if(mon_vec.at(randmon)->boss == 2 && !bossSpawned && dungeonLevel == secondBossLevel){
        boss = mon_vec.at(randmon)->getMonster();
        ret->push_back(boss);
        ++i;
	bossSpawned = true;
      }
      else if((i < numMon - 1 || bossSpawned) && numMon > 1){
        if(dungeonLevel == firstBossLevel && mon_vec.at(randmon)->symbol == 'a'){
          ret->push_back(mon_vec.at(randmon)->getMonster());
          ++i;
        }
      }
    }
    else if(mon_vec.at(randmon)->getDifficulty() <= levelDifficulty()){
      ret->push_back(mon_vec.at(randmon)->getMonster());
      ++i;
    }
  }
  monster_text.close();
  return ret;
}
