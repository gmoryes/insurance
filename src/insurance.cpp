#include "insurance.hpp"

#include "base.hpp"

namespace
{
double ConertPaymentRangeToMonthsNumber(Insurance::PaymentType type)
{
  switch (type)
  {
  case Insurance::PaymentType::Month: return 1.0;
  case Insurance::PaymentType::Quarter: return 3.0;
  case Insurance::PaymentType::Year: return 12.0;
  case Insurance::PaymentType::MaxValue: CHECK(false, ());
  }
}
}  // namespace

double Insurance::GetDemandCoef() const
{
  double range = ConertPaymentRangeToMonthsNumber(m_paymentType);

  CHECK(m_maxSatisfactionValue > 0, ());
  // Чем меньше результат - тем больше спрос
  // Получается:
  // Чем больше месячный платеж - тем меньше спрос
  // Чем больше макс. возмещение - тем больше спрос
  // Чем меньше минимальная граница ушерба - тем больше спрос
  return (range * m_monthPaymentValue) / m_maxSatisfactionValue * 1 / m_minDamageValue;
}

std::string ToString(Insurance::Type type)
{
  switch (type)
  {
  case Insurance::Type::Car: return "авто";
  case Insurance::Type::Health: return "здоровье";
  case Insurance::Type::Home: return "недвижимость";
  case Insurance::Type::MaxValue: CHECK(false, ());
  }

  CHECK(false, ());
}

std::string ToString(Insurance insurance)
{
  return "m_monthPaymentValue = " + ToString(insurance.GetMonthPaymentValue()) + ", " +
         "m_contractTime = " + ToString(insurance.GetContractTime()) + ", " +
         "m_maxSatisfactionValue = " + ToString(insurance.GetMaxSatisfactionValue()) + ", " +
         "m_minDamageValue = " + ToString(insurance.GetMinDamage());
}
