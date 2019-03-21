#include "src/game.hpp"
#include "src/insurance.hpp"
#include "src/main_window.hpp"

#include <iostream>

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);
  MainWindow win;

  QDesktopWidget dw;

  int x = static_cast<int>(dw.width() * 0.7);
  int y = static_cast<int>(dw.height() * 0.7);

  win.setFixedSize(x,y);
  win.setWindowTitle("MISHA APPLICATION");
  win.show();

  Game & game = Game::Instance();

  // Set base demand to different type of insurancies
  game.SetDemands({
    {Insurance::Type::Home, 50 /* demand */},
    {Insurance::Type::Car, 100 /* demand */},
    {Insurance::Type::Health, 150, /* demand */}
  });

  /*
   * Set just one demand from qt callback
   * game.SetDemand(Insurance::Type::Car, 1500);
   */

  game.SetGameDuration(24 /* duration in months */);
  game.SetStartFunc(30000.0 /* start fund */);
  game.SetTaxPercent(0.09 /* tax percent */);

  return app.exec();
}
