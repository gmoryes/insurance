#include "random.hpp"

std::string RandomGenerator::GenString(size_t count, char from, char to)
{
  std::uniform_int_distribution<char> dist(from, to);
  std::string result(count, from);
  for (char& x : result) {
    x = dist(m_gen);
  }
  return result;
}

std::vector<double> RandomGenerator::GenRealVector(size_t count, double from, double to)
{
  std::uniform_real_distribution<double> dist(from, to);
  std::vector<double> result(count);
  for (auto & cur : result)
    cur = dist(m_gen);

  return result;
}

std::vector<int> RandomGenerator::GenPermutation(size_t count)
{
  std::vector<int> result(count);
  std::iota(result.begin(), result.end(), 0);
  std::shuffle(result.begin(), result.end(), m_gen);
  return result;
}
