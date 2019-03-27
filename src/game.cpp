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
  CHECK(m_currentMonth < m_duration, ("The game is end."));

  // Шаг 1. Платим налоги государству
  m_company.PayTaxes(m_taxPercent);

  // Шаг 2. Продажа населению страховок
  m_company.SellInsurances();

  // Шаг 3. Выплата страховок согласно произошедшим страховым случаям
  m_company.PayForInsurances();

  // Шаг 4. Находим просроченные страховки
  m_outdatedInsurances = m_company.GetOutdatedInsurances();

  ++m_currentMonth;
}

