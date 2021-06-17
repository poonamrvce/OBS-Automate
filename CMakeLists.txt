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

include_directories(./src)

# all_src_app
add_executable(all_src_app 
                src/Encoder.cpp
                src/Show.cpp
                src/Scene.cpp
                src/Source.cpp
                src/ObsUtils.cpp
                src/Output.cpp
                examples/all_src_app.cpp)

target_link_libraries(all_src_app
    Qt5::Widgets
    obs)