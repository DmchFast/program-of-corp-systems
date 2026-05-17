#include <catch2/catch_all.hpp>
#include "Session.h"
#include <thread>   // sleep
#include <chrono>

using namespace std::chrono_literals;

// Тест 1: Конструктор и состояние новой сессии
TEST_CASE("Session: new session is active", "[Session]") {
    Session s;
    CHECK(s.isFinished() == false);
    CHECK(s.getDuration() >= 0);
    CHECK(s.getStartTimeStr().length() > 0); // Строка начала не пуста
    CHECK(s.getEndTimeStr().empty()); // Строка конца пуста (активная сессия)
}

// Тест 2: Остановка сессии
TEST_CASE("Session: stop session", "[Session]") {
    Session s;
    std::this_thread::sleep_for(1100ms);
    s.stop();
    CHECK(s.isFinished() == true); // Остановлено  = true
    CHECK(s.getDuration() > 0);
    CHECK(s.getEndTimeStr().empty() == false); // Не остановлена = false
}

// Тест 3: Длительность активной сессии увеличивается со временем
TEST_CASE("Session: duration of active session increases", "[Session]") {
    Session s;
    auto dur1 = s.getDuration();
    std::this_thread::sleep_for(1200ms);
    auto dur2 = s.getDuration();
    CHECK(dur2 > dur1);
}

// Тест 4: Установка начала и конца через строки (проверка сериализации)
TEST_CASE("Session: set start and end from strings", "[Session]") {
    Session s;
    s.setStartTime("15-05-2026 12:00:00");
    CHECK(s.getStartTimeStr() == "15-05-2026 12:00:00");
    // После setStartTime сессия становится активной (isActive = true)
    CHECK(s.isFinished() == false); // isActive = true, isFinished = !isActive
    s.setEndTime("15-05-2026 12:30:00");
    CHECK(s.getEndTimeStr() == "15-05-2026 12:30:00");
    CHECK(s.isFinished() == true); // после установки конца сессия завершена
    CHECK(s.getDuration() == 30 * 60);
}

// Тест 5: Некорректные строки даты не должны менять состояние (в случае если данные загружаютсяиз файла data.json, а файл повреждён или что-то ещё)
TEST_CASE("Session: invalid date strings are ignored", "[Session]") {
    Session s;
    s.setStartTime("garbage");
    CHECK(s.getStartTimeStr() != "garbage"); // осталась прежней без понимания что было. Фактически setStartTime не сработает.
    s.setStartTime("10-05-2026 09:00:00");
    CHECK(s.getStartTimeStr() == "10-05-2026 09:00:00");
    s.setStartTime("bad");
    CHECK(s.getStartTimeStr() == "10-05-2026 09:00:00"); // не изменилась
    s.setEndTime("bad");
    CHECK(s.getEndTimeStr().empty());   // не установилась
    s.setEndTime("");                   // пустая строка – игнорируется
    CHECK(s.getEndTimeStr().empty());
}

// Тест 6: Повторная остановка не меняет время
TEST_CASE("Session: calling stop twice does nothing", "[Session]") {
    Session s;
    std::this_thread::sleep_for(1100ms);
    s.stop();
    auto dur = s.getDuration();
    s.stop();  // второй раз
    CHECK(s.getDuration() == dur); // длительность не изменилась
}

// Тест 7: Получение времени начала и конца
TEST_CASE("Session: get start and end time points", "[Session]") {
    Session s;
    auto start_tp = s.getStartTime();
    CHECK(start_tp != std::chrono::system_clock::time_point{});
    auto end_tp = s.getEndTime();
    // end time для активной сессии (0 по умл)
    CHECK(end_tp == std::chrono::system_clock::time_point{});
    s.stop();
    end_tp = s.getEndTime();
    CHECK(end_tp != std::chrono::system_clock::time_point{});
}