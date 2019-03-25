#pragma once

#include "company.hpp"
#include "insurance.hpp"

#include <cstdint>
#include <map>

class Game
{
public:

  static Game & Instance();

  Company & GetCompany() { return m_company; }

  void DoStep();
  void SetDemands(Company::Demands && demands) { m_baseDemand = std::move(demands); }
  void SetDemand(Insurance::Type insuranceType, uint32_t demand);
  void SetTaxPercent(double taxPercent) { m_taxPercent = taxPercent; }

  void SetStartFunc(double startFund) { m_startFund = startFund; }

  void SetGameDuration(uint32_t duration) { m_duration = duration; }
  uint32_t GetDuration() const { return m_duration; }

private:
  // Базовые требования
  Company::Demands m_baseDemand;

  // Налоги государству
  double m_taxPercent = 0.0;

  // Начальный капитал
  double m_startFund = 0.0;

  // Время игры в месяцах
  uint32_t m_duration = 0;

  Company m_company;
};