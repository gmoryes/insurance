project(sources)

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
