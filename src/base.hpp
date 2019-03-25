#pragma once

#include <iostream>
#include <sstream>
#include <string>

inline std::string ToString() { return ""; }

template <typename T>
inline std::string ToString(T arg)
{
  std::stringstream ss;
  ss << arg;
  return ss.str();
}

template <typename T, typename... Args>
inline std::string ToString(T current, Args... args)
{
  std::string result;
  result += ToString(current);
  result += " ";
  result += ToString(args...);
  return result;
}

template <typename T>
inline T FromString(std::string const & input)
{
  T res = T();
  std::stringstream ss;
  ss << input;
  ss >> res;
  return res;
}

#define CHECK(X, msg) do { if (X) {} else { \
  std::cerr << __FILE__ << ":" << __LINE__ << " " \
  << __FUNCTION__ << "() CHECK("#X"): " << ToString msg \
  << std::endl; std::abort(); } } while(false)
