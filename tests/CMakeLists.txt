cmake_minimum_required(VERSION 3.10)

# Имя проекта тестов
project(TimeTrackerTests)

# Подключение Catch2 через FetchContent
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(Catch2)

# Стандартная система тестирования CMake
include(CTest)
# Макросы Catch2 для интеграции с CTest
include(Catch)

# Пути к файлам проекта
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR})   # для json.hpp

# Файлы cpp
set(PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Session.cpp
    ${CMAKE_SOURCE_DIR}/src/Task.cpp
    ${CMAKE_SOURCE_DIR}/src/Storage.cpp
    ${CMAKE_SOURCE_DIR}/src/TimeTracker.cpp
)

# Тесты для Session
add_executable(session_tests Session_test.cpp ${PROJECT_SOURCES})
target_link_libraries(session_tests PRIVATE Catch2::Catch2WithMain)

# Тесты для Task
add_executable(task_tests Task_test.cpp ${PROJECT_SOURCES})
target_link_libraries(task_tests PRIVATE Catch2::Catch2WithMain)

# Тесты для Storage
add_executable(storage_tests Storage_test.cpp ${PROJECT_SOURCES})
target_link_libraries(storage_tests PRIVATE Catch2::Catch2WithMain)

# Регистрация тестов для CTest (запуск через ctest)
add_test(NAME SessionTests COMMAND session_tests)
add_test(NAME TaskTests COMMAND task_tests)
add_test(NAME StorageTests COMMAND storage_tests)