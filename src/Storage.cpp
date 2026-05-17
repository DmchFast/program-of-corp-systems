#include "Storage.h"
#include "json.hpp"
#include <fstream>

using namespace std;
using json = nlohmann::json;

Storage::Storage(const std::string &fname) : filename(fname) {}

Storage& Storage::getInstance(const std::string &fname)
{
   static Storage instance(fname);
   return instance;
}

void Storage::save(const std::vector<Task> &tasks, int activeIndex)
{
   json j;
   j["active_task_index"] = activeIndex;   // сохранение индекса
   for (size_t i = 0; i < tasks.size(); ++i)
   {
      json jTask;
      jTask["name"] = tasks[i].getName();
      for (const auto &sess : tasks[i].getSessions())
      {
         json jSess;
         jSess["start"] = sess.getStartTimeStr();
         jSess["end"] = sess.getEndTimeStr(); //* пустая строка = сессия не завершена
         jTask["sessions"].push_back(jSess);
      }
      j["tasks"].push_back(jTask);
   }
   ofstream file(filename, std::ios::binary);
   if (file.is_open())
      file << j.dump(4);
}

std::vector<Task> Storage::load(int &activeIndex)
{
   activeIndex = -1;
   ifstream file(filename, std::ios::binary);
   if (!file.is_open())
      return {};

   json j;
   try
   {
      file >> j;
   }
   catch (...)
   {
      return {};
   }

   std::vector<Task> tasks;
   if (j.contains("tasks"))
   {
      for (const auto &jTask : j["tasks"])
      {
         Task task(jTask.value("name", ""));
         if (jTask.contains("sessions"))
         {
            for (const auto &jSess : jTask["sessions"])
            {
               Session sess;
               sess.setStartTime(jSess.value("start", ""));
               sess.setEndTime(jSess.value("end", ""));
               task.addSession(sess);
            }
         }
         tasks.push_back(task);
      }
   }

   //* восстановление активной задачи по индексу
   if (j.contains("active_task_index"))
   {
      int idx = j["active_task_index"];
      if (idx >= 0 && idx < (int)tasks.size())
         activeIndex = idx;
   }
   
   if (activeIndex == -1)
   {
      for (size_t i = 0; i < tasks.size(); ++i)
      {
         for (auto &s : tasks[i].getSessions())
         {
            if (!s.isFinished())
            {
               activeIndex = i;
               break;
            }
         }
         if (activeIndex != -1) break;
      }
   }
   return tasks;
}