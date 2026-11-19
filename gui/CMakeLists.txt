cmake_minimum_required(VERSION 3.16)
project(FlowShopGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(flowshop_gui
    main.cpp
    MainWindow.hpp
    MainWindow.cpp
    GanttWidget.hpp
    GanttWidget.cpp
    GanttWindow.hpp
    GanttWindow.cpp
)

target_include_directories(flowshop_gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(flowshop_gui PRIVATE Qt6::Widgets)
