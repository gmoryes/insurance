#pragma once

#include "insurance.hpp"

#include <vector>
#include <map>

class Company
{
public:
  using Demands = std::map<Insurance::Type, uint32_t>;

  Demands & GetDemands() { return m_demands; }

private:
  double m_currentFund = 0.0;
  Demands m_demands;
  std::vector<Insurance> m_insurancies;
  size_t m_insuranceDeadline;
  size_t m_insuranceSalesCount = 0;
};