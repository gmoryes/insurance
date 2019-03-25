#pragma once

#include <cstdint>
#include <limits>
#include <random>
#include <string>
#include <vector>

class RandomGenerator
{
public:
  explicit RandomGenerator(uint32_t seed = 738547485u): m_gen(seed) {}

  template<class T>
  std::vector<T> GenIntegralVector(size_t count, T from, T to)
  {
    std::uniform_int_distribution<T> dist(from, to);
    std::vector<T> result(count);
    for (auto & cur : result)
      cur = dist(m_gen);

    return result;
  }

  std::string GenString(size_t count, char from = 'a', char to = 'z');

  std::vector<double> GenRealVector(size_t count, double from, double to);

  std::vector<int> GenPermutation(size_t count);

  template<class T>
  T GenReal(T from, T to)
  {
    std::uniform_real_distribution<T> dist(from, to);
    return dist(m_gen);
  }

  template<class T>
  T GenInt(T from, T to)
  {
    std::uniform_int_distribution<T> dist(from, to);
    return dist(m_gen);
  }

  template<class T>
  T GenInt()
  {
    return GenInt(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
  }

  template<class Iterator>
  void Shuffle(Iterator first, Iterator last)
  {
    std::shuffle(first, last, m_gen);
  }

private:
  std::mt19937 m_gen;
};
