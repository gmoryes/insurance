#pragma once

#include "company.hpp"
#include "insurance.hpp"

#include <cstdint>
#include <map>

class Game
{
public:

  static Game & Instance();

  void DoStep();
  void SetDemands(Company::Demands && demands) { m_baseDemand = std::move(demands); }
  void SetDemand(Insurance::Type insuranceType, uint32_t demand);
  void SetTaxPercent(double taxPercent) { m_taxPercent = taxPercent; }
  void SetStartFunc(double startFund) { m_startFund = startFund; }
  void SetGameDuration(uint32_t duration) { m_duration = duration; }

private:
  Company::Demands m_baseDemand;
  double m_taxPercent = 0.0;
  double m_startFund = 0.0;
  uint32_t m_duration = 0;

  Company m_company;
  // ? m_insuranceEventsRange;
};