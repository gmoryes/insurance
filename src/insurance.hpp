#pragma once

#include <cstdint>

class Insurance
{
public:
  enum class Type
  {
    Home,
    Car,
    Health,

    MaxValue
  };

  enum class PaymentType
  {
    Month,
    Quarter,
    Year,

    MaxValue
  };

private:
  PaymentType m_paymentType = PaymentType::MaxValue;
  double m_paymentValue = 0.0;
  uint64_t m_contractTime = 0;
  double m_maxSatisfactionValue = 0.0;
  double m_minDamageValue = 0.0;
};
