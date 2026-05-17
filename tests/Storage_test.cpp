#include <catch2/catch_all.hpp>
#include "Storage.h"
#include <fstream>
#include <filesystem>
#include <random> 
#include <vector>

// Для работы с временным файлом
struct TempFile {
    std::string path;
    TempFile() {
        std::random_device rd; // Генерация имени
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 999999);
        path = "test_storage_" + std::to_string(dis(gen)) + ".json";
    }
    ~TempFile() {
        std::filesystem::remove(path); // удаление файла после теста
    }
};

// Тест 1: всегда один экземпляр
TEST_CASE("Storage: singleton returns same instance", "[Storage]") {
    Storage& s1 = Storage::getInstance();
    Storage& s2 = Storage::getInstance();
    CHECK(&s1 == &s2); // совпадение
}

// Тест 2: Сохранение и загрузка пустого списка задач
TEST_CASE("Storage: save and load empty tasks", "[Storage]") {
    TempFile tmp;
    Storage& storage = Storage::getInstance(tmp.path);
    std::vector<Task> tasks;          // пустой вектор
    int active = -1;
    storage.save(tasks, active);

    int loadedActive = -2;
    auto loaded = storage.load(loadedActive);
    CHECK(loaded.empty());            // загруженный список тоже пуст
    CHECK(loadedActive == -1);        // активной задачи нет
}

// Тест 3: Сохранение и загрузка одной активной задачи без сессий
TEST_CASE("Storage: save and load one task without sessions", "[Storage]") {
    TempFile tmp;
    Storage& storage = Storage::getInstance(tmp.path);
    std::vector<Task> tasks;
    tasks.emplace_back("Одиночная задача");
    int active = 0;   // активна
    storage.save(tasks, active);

    int loadedActive;
    auto loaded = storage.load(loadedActive);
    REQUIRE(loaded.size() == 1);
    CHECK(loaded[0].getName() == "Одиночная задача");
    CHECK(loaded[0].getSessions().empty());
    CHECK(loadedActive == 0); // востановление активной задачи (индекс)
}

// Тест 4: Сохранение и загрузка задачи с завершённой сессией
TEST_CASE("Storage: save and load task with completed session", "[Storage]") {
    TempFile tmp;
    Storage& storage = Storage::getInstance(tmp.path);
    std::vector<Task> tasks;
    Task t("Завершённая задача");
    Session s;
    s.setStartTime("10-05-2026 10:00:00");
    s.setEndTime("10-05-2026 10:05:00");
    t.addSession(s);
    tasks.push_back(t);
    int active = -1;  // завершены
    storage.save(tasks, active);

    int loadedActive;
    auto loaded = storage.load(loadedActive);
    REQUIRE(loaded.size() == 1);
    REQUIRE(loaded[0].getSessions().size() == 1);
    auto& loadedSess = loaded[0].getSessions()[0];
    CHECK(loadedSess.getStartTimeStr() == "10-05-2026 10:00:00");
    CHECK(loadedSess.getEndTimeStr() == "10-05-2026 10:05:00");
    CHECK(loadedSess.isFinished() == true);
    CHECK(loadedActive == -1);
}

// Тест 5: Сохранение и загрузка задачи с активной сессией
TEST_CASE("Storage: save and load task with active session", "[Storage]") {
    TempFile tmp;
    Storage& storage = Storage::getInstance(tmp.path);
    std::vector<Task> tasks;
    Task t("Активная задача");
    Session s;
    s.setStartTime("10-05-2026 12:00:00");
    // сессия остаётся активной
    t.addSession(s);
    tasks.push_back(t);
    int active = 0;   // эпометка активности
    storage.save(tasks, active);

    int loadedActive;
    auto loaded = storage.load(loadedActive);
    REQUIRE(loaded.size() == 1);
    REQUIRE(loaded[0].getSessions().size() == 1);
    auto& loadedSess = loaded[0].getSessions()[0];
    CHECK(loadedSess.getStartTimeStr() == "10-05-2026 12:00:00");
    CHECK(loadedSess.getEndTimeStr().empty());
    CHECK(loadedSess.isFinished() == false);
    CHECK(loadedActive == 0); // индекс активной задачи восстановлен
}

// Тест 6: Сохранение и загрузка нескольких задач
TEST_CASE("Storage: save and load multiple tasks", "[Storage]") {
    TempFile tmp;
    Storage& storage = Storage::getInstance(tmp.path); // экземпляр хранилища
    std::vector<Task> tasks;
    tasks.emplace_back("Задача 1");
    tasks.emplace_back("Задача 2");
    tasks.emplace_back("Задача 3");
    int active = 1; // вторая задача активна
    storage.save(tasks, active); // сохранение в файл

    int loadedActive;
    auto loaded = storage.load(loadedActive);
    REQUIRE(loaded.size() == 3); // проверка
    CHECK(loaded[0].getName() == "Задача 1");
    CHECK(loaded[1].getName() == "Задача 2");
    CHECK(loaded[2].getName() == "Задача 3");
    CHECK(loadedActive == 1);
}