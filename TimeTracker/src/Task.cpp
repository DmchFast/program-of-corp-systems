#include "Task.h"

Task::Task(const std::string &taskName) : name(taskName) {}

void Task::addSession(const Session &s)
{
   sessions.push_back(s);
}

long long Task::totalDuration() const
{
   long long total = 0;
   for (const auto &s : sessions)
      total += s.getDuration();
   return total;
}

void Task::setName(const std::string &newName) { name = newName; }
std::string Task::getName() const { return name; }
const std::vector<Session> &Task::getSessions() const { return sessions; }
std::vector<Session> &Task::getSessions() { return sessions; }