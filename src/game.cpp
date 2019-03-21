#include "game.hpp"
#include "insurance.hpp"

#include "base.hpp"

Game & Game::Instance()
{
  static Game instance;
  return instance;
}

void Game::SetDemand(Insurance::Type insuranceType, uint32_t demand)
{
  CHECK(insuranceType < Insurance::Type::MaxValue, ());
  CHECK(insuranceType != Insurance::Type::MaxValue, ("Bad insuranceType"));

  m_company.GetDemands()[insuranceType] = demand;
}

void Game::DoStep()
{
  //PayTaxes();

}

