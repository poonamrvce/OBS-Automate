cmake_minimum_required(VERSION 2.8)
project(obs_automate)

# Qt
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5Widgets ${FIND_MODE})
if(NOT Qt5Widgets_FOUND)
    message(FATAL_ERROR "Failed to find Qt5")
endif()
set(qtlibs Qt5::Widgets)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# simple_obs_app
add_executable(simple_obs_app
    examples/simple_obs_app.cpp
)

target_link_libraries(simple_obs_app
    Qt5::Widgets
    obs)