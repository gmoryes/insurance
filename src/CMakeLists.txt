cmake_minimum_required(VERSION 3.13)

set (CMAKE_PREFIX_PATH "/usr/local/Cellar/qt/5.9.1/")
# find the location of Qt header files and libraries
find_package(Qt5Widgets REQUIRED)

set(SRC
  company.cpp
  company.hpp
  game.cpp
  game.hpp
  insurance.cpp
  insurance.hpp
  main_window.cpp
  main_window.hpp
  random.cpp
  random.hpp
  base.hpp)

add_library(sources ${SRC})
qt5_use_modules(sources Widgets Core)
