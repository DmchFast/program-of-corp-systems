cmake_minimum_required(VERSION 3.10)

# Имя проекта
set(PROJECT_NAME TimeTracker)

project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

# Установка стандарта C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Настройка выходных директорий
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Список исходных файлов
set(SOURCES
   src/main.cpp
   src/TimeTracker.cpp
   src/Session.cpp
   src/Task.cpp
   src/Storage.cpp
)

# Объявление исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES})

# Подключение директорий с заголовочными файлами
target_include_directories(${PROJECT_NAME} PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}   # для json.hpp
)

add_subdirectory(tests)