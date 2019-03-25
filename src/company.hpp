#pragma once

#include "insurance.hpp"

#include <vector>
#include <map>

class Company
{
public:
  using Demands = std::map<Insurance::Type, uint32_t>;

  uint32_t GetDemand(Insurance::Type type) const  { return m_demands.at(type); }
  uint32_t & GetDemand(Insurance::Type type)  { return m_demands[type]; }
  Demands & GetDemands() { return m_demands; }

  void PayTaxes(double tax);
  void SellInsurances();
  void PayForInsurances();

  Insurance & GetInsurance(Insurance::Type type) { return m_insurancies[type]; }

  inline static uint32_t constexpr kMinShiftForDemand = 0;
  inline static uint32_t constexpr kMaxShiftForDemand = 10;

  inline static uint32_t constexpr kMinNumberOfInsurancesPerMonth = 1;
  inline static uint32_t constexpr kMaxNumberOfInsurancesPerMonth = 25;

private:

  /// \brief Производит перерасчет спроса в зависимости от того сколько стоит страховка
  /// и какая у нее будет страховая выплата.
  void MakeDemandsCorrection();

  // Текущий капитал
  double m_currentFund = 0.0;

  // Текущий спрос на разные виды страховок
  Demands m_demands;

  // Текущий набор страховок
  std::map<Insurance::Type, Insurance> m_insurancies;

  size_t m_insuranceDeadline;
  size_t m_insuranceSalesCount = 0;
};