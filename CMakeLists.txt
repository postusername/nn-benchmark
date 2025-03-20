cmake_minimum_required(VERSION 3.16)
project(NearestNeighborSearch VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

# Включаем поддержку тестирования
enable_testing()

# Создаем все исходные директории, чтобы не было проблем с поиском файлов
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/metrics)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/algorithms)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/benchmark)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/data_generation)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Находим все исходные файлы
file(GLOB METRICS_SOURCES "src/metrics/*.cpp")
file(GLOB ALGORITHM_SOURCES "src/algorithms/*.cpp")
file(GLOB BENCHMARK_SOURCES "src/benchmark/*.cpp")
file(GLOB DATA_GENERATION_SOURCES "src/data_generation/*.cpp")
file(GLOB TEST_SOURCES "tests/*.cpp")

# Объединяем источники
set(SOURCE_FILES
    src/main.cpp
    ${METRICS_SOURCES}
    ${ALGORITHM_SOURCES}
    ${BENCHMARK_SOURCES}
    ${DATA_GENERATION_SOURCES}
)

# Заголовочные файлы
include_directories(include)

# Основной исполняемый файл
add_executable(nn_search ${SOURCE_FILES})

# Тестовый исполняемый файл
add_executable(nn_tests 
    ${TEST_SOURCES}
    ${METRICS_SOURCES}
    ${ALGORITHM_SOURCES}
)

# Добавляем тесты
add_test(NAME AllTests COMMAND nn_tests) 