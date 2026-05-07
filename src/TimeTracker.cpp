#include "TimeTracker.h"
#include <iostream>
#include <limits>

using namespace std;

TimeTracker::TimeTracker() : activeTask(nullptr)
{
   storage = Storage::getInstance("data.json");
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

void TimeTracker::showDailyReport() { cout << "Отчёт за сегодня\n"; }
void TimeTracker::showPeriodReport() { cout << "Отчёт за период\n"; }

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