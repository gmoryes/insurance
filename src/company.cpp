#include "company.hpp"

#include "random.hpp"

#include "base.hpp"

Company::Company()
{
  auto const configure = [&](Insurance::Type type, Insurance insurance, uint32_t demand) {
    m_insurancies[type] = insurance;
    m_demands[type] = demand;
  };

  configure(Insurance::Type::Car, {
    Insurance::PaymentType::Year,
    2000 /* monthPaymentValue */,
    3 /* contractTime */,
    5000 /* maxSatisfactionValue */,
    0.8 /* minDamageValue */
  }, 70 /* demand */);

  configure(Insurance::Type::Home, {
    Insurance::PaymentType::Year,
    3000 /* monthPaymentValue */,
    12 /* contractTime */,
    20000 /* maxSatisfactionValue */,
    0.7 /* minDamageValue */
  }, 30 /* demand */);

  configure(Insurance::Type::Health, {
    Insurance::PaymentType::Quarter,
    500 /* monthPaymentValue */,
    6 /* contractTime */,
    2000 /* maxSatisfactionValue */,
    0.6 /* minDamageValue */
  }, 100 /* demand */);
}

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
    auto amount = demand + shift;
    std::cout << "shift = " << shift << std::endl;
    m_currentFund += amount * insurance.GetMonthPaymentValue();

    m_insuranciesSold[type] += amount;

    std::cout << "+" << (demand + shift) * insurance.GetMonthPaymentValue()
              << " for: " << ToString(type) << std::endl;
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
