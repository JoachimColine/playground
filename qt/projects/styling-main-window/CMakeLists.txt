cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME minimal_styling_mainwindow_app)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME}
    resources/resources.qrc
    src/main.cpp
    src/mainwindow.cpp
    include/mainwindow.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
