#include "Storage.h"
#include "json.hpp"
#include <fstream>

using namespace std;
using json = nlohmann::json;

Storage *Storage::instance = nullptr;

Storage::Storage(const std::string &fname) : filename(fname) {}

Storage *Storage::getInstance(const std::string &fname)
{
   if (!instance)
      instance = new Storage(fname);
   return instance;
}

void Storage::save(const std::vector<Task> &tasks, const Task *activeTask)
{
   json j;
   j["active_task_index"] = -1;
   for (size_t i = 0; i < tasks.size(); ++i)
   {
      if (activeTask && &tasks[i] == activeTask)
         j["active_task_index"] = static_cast<int>(i);

      json jTask;
      jTask["name"] = tasks[i].getName();
      for (const auto &sess : tasks[i].getSessions())
      {
         json jSess;
         jSess["start"] = sess.getStartTimeStr();
         jSess["end"] = sess.getEndTimeStr(); //* пустая строка = сессия не завершина
         jTask["sessions"].push_back(jSess);
      }
      j["tasks"].push_back(jTask);
   }
   ofstream file(filename, std::ios::binary);
   if (file.is_open())
      file << j.dump(4); // читаемость (отступы)
}

std::vector<Task> Storage::load(Task *&activeTask)
{
   activeTask = nullptr;
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
   } //* повреждённые файлы игонорируются

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
               sess.setEndTime(jSess.value("end", "")); //* пустая строка = сессия акт
               task.addSession(sess);
            }
         }
         tasks.push_back(task);
      }
   }
   //* по индексу востанавливается актив сессия
   if (j.contains("active_task_index"))
   {
      int idx = j["active_task_index"];
      if (idx >= 0 && idx < (int)tasks.size())
         activeTask = &tasks[idx];
   }
   
   if (!activeTask)
   {
      for (size_t i = 0; i < tasks.size(); ++i)
      {
         for (auto &s : tasks[i].getSessions())
         {
            if (!s.isFinished())
            {
               activeTask = &tasks[i];
               break;
            }
         }
         if (activeTask)
            break;
      }
   }
   return tasks;
}