#include "TimeTracker.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>

using namespace std;

TimeTracker::TimeTracker() : activeIndex(-1), storage(Storage::getInstance("data.json"))
{
   tasks = storage.load(activeIndex);
}
TimeTracker::~TimeTracker() {}

void TimeTracker::clearScreen()
{
#ifdef _WIN32
   system("cls");
#else
   system("clear");
#endif
}

void TimeTracker::waitForEnter()
{
   cout << "\nПодтвердить (enter)";
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   cin.get();
}

void TimeTracker::showMenu()
{
   clearScreen();
   cout << "~ TimeTracker ~\n";
   if (activeIndex != -1)
      cout << "Активная задача: " << tasks[activeIndex].getName() << "\n";
   else
      cout << "Нет активной задачи.\n";
   cout << "\nМеню:\n";
   cout << "1. Все задачи\n";
   cout << "2. Добавить задачу\n";
   cout << "3. Редактировать задачу\n";
   cout << "4. Удалить задачу\n";
   cout << "5. Запустить таймер\n";
   cout << "6. Остановить таймер\n";
   cout << "7. Отчёт за сегодня\n";
   cout << "8. Отчёт за период\n";
   cout << "0. Выход\n";
   cout << "> ";
}

void TimeTracker::showAllTasks()
{
   if (tasks.empty())
   {
      cout << "Список задач пока пуст.\n";
      return;
   }
   for (size_t i = 0; i < tasks.size(); ++i)
   {
      long long sec = tasks[i].totalDuration();
      long long h = sec / 3600, m = (sec % 3600) / 60, s = sec % 60;
      cout << i + 1 << ". " << tasks[i].getName()
           << " (всего: " << h << "ч " << m << "м " << s << "с)\n";
   }
}

void TimeTracker::addNewTask()
{
   cout << "Название задачи: ";
   string name;
   cin.ignore();
   getline(cin, name);
   if (name.empty())
   {
      cout << "Имя не может быть пустым.\n";
      return;
   }
   tasks.emplace_back(name);
   storage.save(tasks, activeIndex);
   cout << "Задача добавлена.\n";
}

void TimeTracker::editTaskName()
{
   showAllTasks();
   if (tasks.empty())
      return;
   int idx;
   cout << "Номер задачи для редактирования: ";
   cin >> idx;
   if (idx < 1 || idx > (int)tasks.size())
   {
      cout << "Неверный номер.\n";
      return;
   }
   string newName;
   cout << "Новое название: ";
   cin.ignore();
   getline(cin, newName);
   if (newName.empty())
   {
      cout << "Имя не может быть пустым.\n";
      return;
   }
   tasks[idx - 1].setName(newName);
   storage.save(tasks, activeIndex);
   cout << "Задача переименована.\n";
}

void TimeTracker::deleteTaskById()
{
   showAllTasks();
   if (tasks.empty())
      return;
   int idx;
   cout << "Номер задачи для удаления: ";
   cin >> idx;
   if (idx < 1 || idx > (int)tasks.size())
   {
      cout << "Неверный номер.\n";
      return;
   }
   //* коррекция индекса активной задачи
   if (idx - 1 == activeIndex)
      activeIndex = -1;
   else if (activeIndex > idx - 1)
      activeIndex--;   // смещение индекса после удаления более ранней задачи
   tasks.erase(tasks.begin() + idx - 1);
   storage.save(tasks, activeIndex);
   cout << "Задача удалена.\n";
}

void TimeTracker::startTimer()
{
   if (tasks.empty())
   {
      cout << "Сначала создайте задачу.\n";
      return;
   }
   //* если есть активная задача (завершаем текущую сессию)
   if (activeIndex != -1)
   {
      for (auto &s : tasks[activeIndex].getSessions())
         if (!s.isFinished())
         {
            s.stop();
            break;
         }
      activeIndex = -1;   // сброс индекса после остановки
   }
   showAllTasks();
   int idx;
   cout << "Номер задачи для запуска: ";
   cin >> idx;
   if (idx < 1 || idx > (int)tasks.size())
   {
      cout << "Неверный номер.\n";
      return;
   }
   // Проверка на наличие незавершённой сессии у выбранной задачи
   for (const auto &s : tasks[idx - 1].getSessions())
      if (!s.isFinished())
      {
         cout << "У этой задачи уже есть активная сессия.\n";
         return;
      }
   Session newSession;
   tasks[idx - 1].addSession(newSession);
   activeIndex = idx - 1;
   storage.save(tasks, activeIndex);
   cout << "Таймер запущен для \"" << tasks[activeIndex].getName() << "\".\n";
}

void TimeTracker::stopTimer()
{
   if (activeIndex == -1)
   {
      cout << "Нет активной задачи.\n";
      return;
   }
   for (auto &s : tasks[activeIndex].getSessions())
      if (!s.isFinished())
      {
         s.stop();
         break;
      }
   cout << "Таймер для \"" << tasks[activeIndex].getName() << "\" остановлен.\n";
   activeIndex = -1;
   storage.save(tasks, activeIndex);
}

// вспомогательная функция для подсчёта времени сессии в пределах [startBound, endBound]
static long long getDurationInRange(const Session& s, time_t startBound, time_t endBound)
{
   auto tStart = chrono::system_clock::to_time_t(s.getStartTime());
   // для активной сессии конец = текущее время
   auto tEnd = s.isFinished() ? chrono::system_clock::to_time_t(s.getEndTime()) : chrono::system_clock::to_time_t(chrono::system_clock::now());
   if (tEnd < startBound || tStart > endBound) return 0;
   time_t overlapStart = max(startBound, tStart);
   time_t overlapEnd = min(endBound, tEnd);
   return static_cast<long long>(difftime(overlapEnd, overlapStart));
}

void TimeTracker::showDailyReport()
{
   cout << "\n~ Отчёт за сегодня ~\n";
   // получаем сегодняшнюю дату с 00:00 до 23:59:59
   auto now = chrono::system_clock::now();
   time_t now_t = chrono::system_clock::to_time_t(now);
   tm today = *localtime(&now_t);
   today.tm_hour = 0; today.tm_min = 0; today.tm_sec = 0;
   time_t startDay = mktime(&today);
   time_t endDay = startDay + 24 * 3600 - 1;  // 23:59:59
   
   for (const auto &task : tasks)
   {
      long long totalSec = 0;
      for (const auto &s : task.getSessions())
      {
         totalSec += getDurationInRange(s, startDay, endDay);
      }
      if (totalSec == 0) continue;
      long long h = totalSec / 3600, m = (totalSec % 3600) / 60, s = totalSec % 60;
      cout << task.getName() << ": " << h << "ч " 
           << setfill('0') << setw(2) << m << "м "
           << setfill('0') << setw(2) << s << "с\n";
   }
}

void TimeTracker::showPeriodReport()
{
   string from, to;
   cout << "Начальная дата (ДД-ММ-ГГГГ): ";
   cin >> from;
   cout << "Конечная дата (ДД-ММ-ГГГГ): ";
   cin >> to;

   tm tmFrom = {}, tmTo = {};
   istringstream sf(from), st(to);
   sf >> get_time(&tmFrom, "%d-%m-%Y");
   st >> get_time(&tmTo, "%d-%m-%Y");
   if (sf.fail() || st.fail())
   {
      cout << "Неверный формат даты.\n";
      return;
   }

   tmFrom.tm_hour = 0; tmFrom.tm_min = 0; tmFrom.tm_sec = 0;
   tmTo.tm_hour = 23; tmTo.tm_min = 59; tmTo.tm_sec = 59;
   time_t periodStart = mktime(&tmFrom);
   time_t periodEnd = mktime(&tmTo);

   cout << "\n~ Отчёт за период ~\n";
   for (const auto &task : tasks)
   {
      long long totalSec = 0;
      for (const auto &s : task.getSessions())
      {
         totalSec += getDurationInRange(s, periodStart, periodEnd);
      }
      if (totalSec == 0) continue;
      long long h = totalSec / 3600, m = (totalSec % 3600) / 60, s = totalSec % 60;
      cout << task.getName() << ": " << h << "ч "
           << setfill('0') << setw(2) << m << "м "
           << setfill('0') << setw(2) << s << "с\n";
   }
}

void TimeTracker::handleCommand(int choice)
{
   switch (choice)
   {
   case 1:
      showAllTasks();
      break;
   case 2:
      addNewTask();
      break;
   case 3:
      editTaskName();
      break;
   case 4:
      deleteTaskById();
      break;
   case 5:
      startTimer();
      break;
   case 6:
      stopTimer();
      break;
   case 7:
      showDailyReport();
      break;
   case 8:
      showPeriodReport();
      break;
   case 0:
      break;
   default:
      cout << "Неверный пункт.\n";
   }
   if (choice != 0 && choice != 5 && choice != 6)
      waitForEnter();
}

void TimeTracker::run()
{
   int choice;
   do
   {
      showMenu();
      cin >> choice;
      handleCommand(choice);
   } while (choice != 0);
}