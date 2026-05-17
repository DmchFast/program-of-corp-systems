#include <catch2/catch_all.hpp>
#include "TimeTracker.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <fstream>
#include <filesystem>

// Тестовый файл: имя в рабочей директории тестов (как в Storage тесте)
const std::string &testFilePath()
{
  static const std::string kTestFile = "test_timetracker.json";
  return kTestFile;
}

// Удаление сатрого сетового файла
void resetStorageFile()
{
  const std::string &path = testFilePath();
  std::filesystem::remove(path);
  Storage::getInstance(path);
}

// Enter
void appendPause(std::ostringstream &input)
{
  input << "\n\n";
}

// Для перенаправления ввода/вывода
class RedirectIO
{
  std::stringstream fakeCin;
  std::streambuf *oldCin;
  std::stringstream fakeCout;
  std::streambuf *oldCout;

public:
  // Имитация ввода пользователя (с'\n')
  RedirectIO(const std::string &input) : fakeCin(input)
  {
    oldCin = std::cin.rdbuf(fakeCin.rdbuf());
    oldCout = std::cout.rdbuf(fakeCout.rdbuf());
  }
  ~RedirectIO()
  {
    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);
  }
  std::string out() const { return fakeCout.str(); }
};

// Сценарий 1: Добавить задачу, запуск таймера, его остановка и проверка завершённости сессии
TEST_CASE("Scenario 1: Add task, start timer, stop, check session", "[scenario]")
{
  resetStorageFile();
  TimeTracker tracker;

  std::ostringstream input;
  input << "2\nTaskOne\n"; // TaskOne
  appendPause(input);
  input << "5\n1\n"; // запуск таймера с TaskOne
  input << "6\n";    // остановка
  input << "0\n";

  RedirectIO io(input.str());
  tracker.run();

  // Прверка данных
  int active = -1;
  auto tasks = Storage::getInstance(testFilePath()).load(active);
  REQUIRE(tasks.size() == 1);
  REQUIRE(tasks[0].getSessions().size() == 1);
  CHECK(tasks[0].getSessions()[0].isFinished() == true); // сессия остановлена
  CHECK(active == -1);                                   // активной задачи нет
}

// Сценарий 2: Редактирование имени задачи
TEST_CASE("Scenario 2: Edit task name", "[scenario]")
{
  resetStorageFile();
  TimeTracker tracker;

  std::ostringstream input;
  input << "2\nOldName\n";
  appendPause(input);
  input << "3\n1\nNewName\n"; // переименование
  appendPause(input);
  input << "0\n";
  RedirectIO io(input.str());
  tracker.run();

  int active = -1;
  auto tasks = Storage::getInstance(testFilePath()).load(active);
  REQUIRE(tasks.size() == 1);
  CHECK(tasks[0].getName() == "NewName"); // новое имя
}

// Сценарий 3: Удаление задачи, активная задача сбрасывается
TEST_CASE("Scenario 3: Delete task, active reset", "[scenario]")
{
  resetStorageFile();
  TimeTracker tracker;

  std::ostringstream input;
  input << "2\nToDelete\n";
  appendPause(input);
  input << "5\n1\n";
  input << "4\n1\n";
  appendPause(input);
  input << "0\n";

  RedirectIO io(input.str());
  tracker.run();

  int active = -1;
  auto tasks = Storage::getInstance(testFilePath()).load(active);
  CHECK(tasks.empty()); // нет задачи
  CHECK(active == -1);  // активной задачи нет
}

// Сценарий 4: Активная сессия сохраняется и восстанавливается после перезапуска
TEST_CASE("Scenario 4: Active session persists after restart", "[scenario]")
{
  resetStorageFile();

  // Первый запуск: (создание задачи и запуск таймера безостанавки)
  {
    TimeTracker tracker;
    std::ostringstream input;
    input << "2\nPersistTask\n";
    appendPause(input);
    input << "5\n1\n";
    input << "0\n";
    RedirectIO io(input.str());
    tracker.run();
  }

  int active = -1;
  auto tasks = Storage::getInstance(testFilePath()).load(active);
  REQUIRE(tasks.size() == 1);
  REQUIRE(tasks[0].getSessions().size() == 1);
  CHECK(tasks[0].getSessions()[0].isFinished() == false); // сессия активна
  CHECK(active == 0);                                     // индекс активной задачи

  // Второй запуск (проверка)
  {
    TimeTracker tracker2;
    RedirectIO io("0\n");
    tracker2.run();
  }
}

// Сценарий 5: Отчёт за период
TEST_CASE("Scenario 5: Period report includes task name", "[scenario]")
{
  resetStorageFile();

  // Задачи с двумя периодами
  {
    Storage &storage = Storage::getInstance(testFilePath());
    std::vector<Task> tasks;
    Task t("PeriodTask");
    Session s1;
    s1.setStartTime("10-05-2026 10:00:00");
    s1.setEndTime("10-05-2026 12:00:00");
    Session s2;
    s2.setStartTime("15-05-2026 14:00:00");
    s2.setEndTime("15-05-2026 15:00:00");
    t.addSession(s1);
    t.addSession(s2);
    tasks.push_back(t);
    storage.save(tasks, -1);
  }

  TimeTracker tracker;
  std::ostringstream input;
  input << "8\n09-05-2026\n11-05-2026\n"; // отчёт за период
  appendPause(input);
  input << "0\n";

  RedirectIO io(input.str());
  tracker.run();
  std::string output = io.out();

  // Проверка вывода отчёта
  CHECK(output.find("PeriodTask") != std::string::npos);
}