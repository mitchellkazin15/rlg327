#ifndef DESCRIPTION_H
#define DESCRIPTION_H

extern char object_symbol[];

class object_description {
public:
  std::string name, desc;
  object_type_t type;
  uint32_t color;
  dice *hit, *damage, *dodge, *defence, *weight, *speed, *attribute, *value;
  int str,dex,intel;
public:
  object_description();
  object* getObject();
  void printObjectDesc();
};

vector<object*>* read_obj_file(int numObj, int flag);

#endif
 
