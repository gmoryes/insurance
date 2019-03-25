#include "company.hpp"

#include "random.hpp"

#include "base.hpp"

void Company::PayTaxes(double tax)
{
  CHECK(0.0 <= tax && tax <= 1.0, ("Tax should be in range [0, 1]"));
  m_currentFund *= (1 - tax);
}

void Company::MakeDemandsCorrection()
{
  for (auto && [type, insurance] : m_insurancies)
  {
    double k = insurance.GetDemandCoef();
    CHECK(k > 0, ());

    auto prev = m_demands[type];
    m_demands[type] = static_cast<uint32_t>(prev * (1 / k));
  }
}

void Company::SellInsurances()
{
  // Производим корректировку спроса в зависимости от цены
  // на строховку и размера выплаты за нее
  MakeDemandsCorrection();

  RandomGenerator generator;
  for (auto && [type, insurance] : m_insurancies)
  {
    auto demand = m_demands[type];
    auto shift = generator.GenInt(kMinShiftForDemand, kMaxShiftForDemand);
    m_currentFund += (demand + shift) * insurance.GetMonthPaymentValue();
  }
}

void Company::PayForInsurances()
{
  RandomGenerator generator;
  for (auto && [_, insurance] : m_insurancies)
  {
    auto insurancesNumber = generator.GenInt<uint32_t>(kMinNumberOfInsurancesPerMonth,
                                                       kMaxNumberOfInsurancesPerMonth);

    for (auto const damage : generator.GenRealVector(insurancesNumber, 0, 1))
    {
      if (damage < insurance.GetMinDamage())
        continue;

      auto paymentValue = insurance.GetMaxSatisfactionValue() * damage;
      m_currentFund -= paymentValue;
    }
  }
}

