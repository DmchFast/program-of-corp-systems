#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>
#include "Task.h"

class Storage
{
public:
   static Storage& getInstance(const std::string &fname = "data.json");
   
   void save(const std::vector<Task> &tasks, int activeIndex);   // индекс
   std::vector<Task> load(int &activeIndex);

private:
   Storage(const std::string &fname = "data.json");
   std::string filename;
};

#endif