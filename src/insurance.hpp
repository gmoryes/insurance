#pragma once

#include <cstdint>
#include <string>

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

  Insurance() = default;
  Insurance(PaymentType paymentType, double monthPaymentValue, uint32_t contractTime,
            double maxSatisfactionValue, double minDamageValue)
    : m_paymentType(paymentType),
      m_monthPaymentValue(monthPaymentValue),
      m_contractTime(contractTime),
      m_maxSatisfactionValue(maxSatisfactionValue),
      m_minDamageValue(minDamageValue) {}

  double GetDemandCoef() const;

  double GetMinDamage() const { return m_minDamageValue; }
  double & GetMinDamage() { return m_minDamageValue; }

  uint32_t GetContractTime() const { return m_contractTime; }
  uint32_t & GetContractTime() { return m_contractTime; }

  uint32_t GetPassedTime() const { return m_passedTime; }
  uint32_t & GetPassedTime() { return m_passedTime; }

  double GetMonthPaymentValue() const { return m_monthPaymentValue; }
  double & GetMonthPaymentValue() { return m_monthPaymentValue; }

  double GetMaxSatisfactionValue() const { return m_maxSatisfactionValue; }
  double & GetMaxSatisfactionValue() { return m_maxSatisfactionValue; }

  PaymentType GetPaymentType() const { return m_paymentType; }

  bool operator==(Insurance const & rhs) const
  {
    return m_paymentType == rhs.m_paymentType && m_monthPaymentValue == rhs.m_monthPaymentValue &&
           m_contractTime == rhs.m_contractTime && m_maxSatisfactionValue == rhs.m_maxSatisfactionValue &&
           m_minDamageValue == rhs.m_minDamageValue;
  }

  bool operator!=(Insurance const & rhs) const
  {
    return !(*this == rhs);
  }

private:
  PaymentType m_paymentType = PaymentType::MaxValue;

  // Размер страхового взноса за 1 месяц
  double m_monthPaymentValue = 0.0;

  // Длительность строховки в месяцах
  uint32_t m_contractTime = 0;

  uint32_t m_passedTime = 0;

  // Сумма макс. возмещения ущерба
  double m_maxSatisfactionValue = 0.0;

  // Минимальный ущерб с которого начинается возмещение страховки
  double m_minDamageValue = 0.0;
};

std::string ToString(Insurance insurance);
std::string ToString(Insurance::Type type);
