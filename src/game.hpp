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

  void SetStartFunc(double startFund) { m_company.GetCurrentFund() = startFund; }

  void SetGameDuration(uint32_t duration) { m_duration = duration; }
  uint32_t GetCurrentMonth() const { return m_currentMonth; }
  uint32_t GetDuration() const { return m_duration; }

  bool IsLose() const { return m_company.GetCurrentFund() <= 0; }
  bool IsWin() const { return m_currentMonth == m_duration; }

  std::vector<Insurance::Type> const & GetOutdatedInsurances() const { return m_outdatedInsurances; }

private:
  // Базовые требования
  Company::Demands m_baseDemand;

  // Налоги государству
  double m_taxPercent = 0.0;

  // Текущий месяц
  uint32_t m_currentMonth = 0;

  // Время игры в месяцах
  uint32_t m_duration = 0;

  Company m_company;

  std::vector<Insurance::Type> m_outdatedInsurances;
};