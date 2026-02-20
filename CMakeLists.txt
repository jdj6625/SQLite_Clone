CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

project(SQLite C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(sqlite_core
    src/input_buffer.c
    src/input_processing.c
    src/row.c
    src/table.c
)

target_include_directories(sqlite_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(SQLite
    src/main.c
)

target_link_libraries(SQLite
    PRIVATE sqlite_core
)

include(CTest)
enable_testing()
add_subdirectory(tests)