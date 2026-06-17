#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include "Session.h"

class Task
{
public:
   Task(const std::string &taskName = "");
   void addSession(const Session &s);
   long long totalDuration() const; //* суммарное время всех завершённых сессий
   void setName(const std::string &newName);
   std::string getName() const;
   const std::vector<Session> &getSessions() const;
   std::vector<Session> &getSessions(); //* модификации

private:
   std::string name;
   std::vector<Session> sessions;
};

#endif