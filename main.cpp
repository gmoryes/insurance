#include "src/game.hpp"
#include "src/insurance.hpp"
#include "src/main_window.hpp"

#include "src/base.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);
  MainWindow win;

  win.setWindowTitle("Экономическая игра");
  win.show();

  return app.exec();
}
