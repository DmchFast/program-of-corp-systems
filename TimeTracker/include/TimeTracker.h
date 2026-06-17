#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include <vector>
#include "Task.h"
#include "Storage.h"

class TimeTracker
{
public:
   TimeTracker();
   ~TimeTracker();
   void run();

private:
   void showMenu();
   void handleCommand(int choice);
   void clearScreen();
   void waitForEnter();

   void showAllTasks();
   void addNewTask();
   void editTaskName();
   void deleteTaskById();
   void startTimer();
   void stopTimer();
   void showDailyReport();
   void showPeriodReport();

   std::vector<Task> tasks;
   int activeIndex;               // индекс активной задачи (-1 = нет активной)
   Storage& storage;              // ссылка на синглтон
};

#endif