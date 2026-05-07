#include "TimeTracker.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>

using namespace std;

TimeTracker::TimeTracker() : activeTask(nullptr)
{
   storage = Storage::getInstance("data.json");
   tasks = storage->load(activeTask);
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
   if (activeTask)
      cout << "Активная задача: " << activeTask->getName() << "\n";
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
      long long sec = tasks[i].totalDuration(); //* общее время в сек
      long long h = sec / 3600, m = (sec % 3600) / 60, s = sec % 60;
      cout << i + 1 << ". " << tasks[i].getName()
           << " (всего: " << h << "ч " << m << "м " << s << "с)\n";
   }
}

void TimeTracker::addNewTask()
{
   cout << "Название задачи: ";
   string name;
   cin.ignore(); //* очистка буфер перед getline
   getline(cin, name);
   if (name.empty())
   {
      cout << "Имя не может быть пустым.\n";
      return;
   }
   tasks.emplace_back(name); //* добавление задачи с именем
   storage->save(tasks, activeTask);
   cout << "Задача добавлена.\n";
}

void TimeTracker::editTaskName()
{
   showAllTasks(); // список задач
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
   tasks[idx - 1].setName(newName);  // переименование задачи
   storage->save(tasks, activeTask); // сохранение
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
   //* сброс указателя activeTask при удалении активной задачи
   if (&tasks[idx - 1] == activeTask)
      activeTask = nullptr;
   tasks.erase(tasks.begin() + idx - 1); // удаление из вектора
   storage->save(tasks, activeTask);
   cout << "Задача удалена.\n";
}

void TimeTracker::startTimer()
{
   if (tasks.empty())
   {
      cout << "Сначала создайте задачу.\n";
      return;
   }
   if (activeTask)
   {
      //* автоматическое завершение активной задачи
      for (auto &s : activeTask->getSessions())
         if (!s.isFinished())
         {
            s.stop();
            break;
         }
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
   // Проверка на завершённость сессии
   for (const auto &s : tasks[idx - 1].getSessions())
      if (!s.isFinished())
      {
         cout << "У этой задачи уже есть активная сессия.\n";
         return;
      }
   Session newSession; //* старт => начало новой сессии
   tasks[idx - 1].addSession(newSession);
   activeTask = &tasks[idx - 1];
   storage->save(tasks, activeTask);
   cout << "Таймер запущен для \"" << activeTask->getName() << "\".\n";
}

void TimeTracker::stopTimer()
{
   if (!activeTask)
   {
      cout << "Нет активной задачи.\n";
      return;
   }
   //* поиск активной сессис => завершить
   for (auto &s : activeTask->getSessions())
      if (!s.isFinished())
      {
         s.stop();
         break;
      }
   cout << "Таймер для \"" << activeTask->getName() << "\" остановлен.\n";
   activeTask = nullptr; // сброс указателя контроля
   storage->save(tasks, activeTask);
}

void TimeTracker::showDailyReport()
{
   cout << "\n~ Отчёт за сегодня ~\n";
   // суммарное время по каждой задаче
   for (const auto &task : tasks)
   {
      long long sec = task.totalDuration();
      long long h = sec / 3600, m = (sec % 3600) / 60, s = sec % 60;
      cout << task.getName() << ": "
           << h << "ч " << setfill('0') << setw(2) << m << "м "
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

   // парсинг дат
   tm tmFrom = {}, tmTo = {};
   istringstream sf(from), st(to);
   sf >> get_time(&tmFrom, "%d-%m-%Y");
   st >> get_time(&tmTo, "%d-%m-%Y");
   if (sf.fail() || st.fail())
   {
      cout << "Неверный формат даты.\n";
      return;
   }

   //* преаброзование начала и конца суток
   tmFrom.tm_hour = 0;
   tmFrom.tm_min = 0;
   tmFrom.tm_sec = 0;
   tmTo.tm_hour = 23;
   tmTo.tm_min = 59;
   tmTo.tm_sec = 59;

   time_t periodStart = mktime(&tmFrom);
   time_t periodEnd = mktime(&tmTo);

   cout << "\n~ Отчёт за период ~\n";
   for (const auto &task : tasks)
   {
      long long totalSec = 0;

      // Суммирование только сессий в диапазоне
      for (const auto &s : task.getSessions())
      {
         if (!s.isFinished())
            continue;

         string sStart = s.getStartTimeStr(), sEnd = s.getEndTimeStr();
         tm tmS = {}, tmE = {};
         istringstream ss(sStart), se(sEnd);
         ss >> get_time(&tmS, "%d-%m-%Y %H:%M:%S");
         se >> get_time(&tmE, "%d-%m-%Y %H:%M:%S");
         if (ss.fail() || se.fail())
            continue;

         time_t sessStart = mktime(&tmS);
         time_t sessEnd = mktime(&tmE);

         // скип сессии за пределами диапазона
         if (sessEnd < periodStart || sessStart > periodEnd)
            continue;

         time_t overlapStart = max(periodStart, sessStart);
         time_t overlapEnd = min(periodEnd, sessEnd);
         totalSec += static_cast<long long>(difftime(overlapEnd, overlapStart));
      }

      if (totalSec == 0)
         continue;

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