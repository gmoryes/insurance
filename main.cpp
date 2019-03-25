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

  Game & game = Game::Instance();

  /*
   * Set just one demand from qt callback
   * game.SetDemand(Insurance::Type::Car, 1500);
   */

  game.SetGameDuration(24 /* duration in months */);
  game.SetStartFunc(30000.0 /* start fund */);
  game.SetTaxPercent(0.09 /* tax percent */);

  return app.exec();
}
