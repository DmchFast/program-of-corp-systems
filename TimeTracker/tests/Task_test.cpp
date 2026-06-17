#include <catch2/catch_all.hpp>
#include "Task.h"
#include <thread>   // sleep_for
#include <chrono>

using namespace std::chrono_literals;

// Тест 1: Создание задачи с именем и изменение имени
TEST_CASE("Task: creation and naming", "[Task]") {
    SECTION("Задача без имени") {
        Task t;
        CHECK(t.getName().empty()); // пустое имя
    }
    SECTION("Задача с именем") {
        Task t("Работа");
        CHECK(t.getName() == "Работа");
    }
    SECTION("Переименование") {
        Task t("Старое");
        t.setName("Новое");
        CHECK(t.getName() == "Новое");
    }
}

// Тест 2: Добавление сессий в задачу
TEST_CASE("Task: add sessions", "[Task]") {
    Task t;
    SECTION("Добавление одной сессии") {
        Session s;
        s.stop();               // завершение чтобы время не росло
        t.addSession(s);
        CHECK(t.getSessions().size() == 1);
    }
    SECTION("Добавление нескольких сессий") {
        for (int i = 0; i < 3; ++i) {
            Session s;
            s.stop();
            t.addSession(s);
        }
        CHECK(t.getSessions().size() == 3);
    }
}

// Тест 3: Общая длительность для нескольких сессий
TEST_CASE("Task: total duration sums sessions", "[Task]") {
    Task t;
    // Первая сессия: 2 секунды
    Session s1;
    std::this_thread::sleep_for(2s);
    s1.stop();
    t.addSession(s1);
    // Вторая сессия: 3 секунды
    Session s2;
    std::this_thread::sleep_for(3s);
    s2.stop();
    t.addSession(s2);
    // Ожидаем ~5 секунд ±1 секунда
    REQUIRE(t.totalDuration() == Catch::Approx(5).margin(1));
}

// Тест 4: Активная сессия увеличивает общую длительность(totalDuration)
TEST_CASE("Task: active session contributes to total duration", "[Task]") {
    Task t;
    Session active;
    t.addSession(active);           // таймер идёт
    auto dur1 = t.totalDuration();
    std::this_thread::sleep_for(1100ms);
    auto dur2 = t.totalDuration();
    CHECK(dur2 > dur1);             // длительность  изменилась (увел)
}

// Тест 5: Доступ к сессиям для изменения getSessions()
TEST_CASE("Task: modify sessions via non-const getSessions", "[Task]") {
    Task t;
    Session s;
    s.stop();
    t.addSession(s);
    auto& sessions = t.getSessions(); // неконстантная ссылка
    REQUIRE(sessions.size() == 1);
    // Останавливаем сессию (хотя она уже остановлена, проверка доступа)
    sessions[0].stop();
    CHECK(sessions[0].isFinished() == true);
}

// Тест 6: totalDuration для задачи без сессий равен 0?
TEST_CASE("Task: total duration with no sessions is zero", "[Task]") {
    Task t;
    CHECK(t.totalDuration() == 0);
}

// Тест 7: Активная сессия даёт вклад в totalDuration() (значение растёт со временем) и totalDuration() правильно пересчитывается после изменений
TEST_CASE("Task: stop active session via reference", "[Task]") {
    Task t;
    Session active;
    t.addSession(active);
    auto before = t.totalDuration();
    std::this_thread::sleep_for(1100ms);
    auto& sessions = t.getSessions();
    sessions[0].stop();             // остановка активной сессии
    auto after = t.totalDuration();
    CHECK(after > before);          // время фиксации
    std::this_thread::sleep_for(1100ms);
    CHECK(t.totalDuration() == after); // не увеличивается после остановки
}