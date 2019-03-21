#pragma once

#include <iostream>
#include <sstream>
#include <string>

std::string CAT() { return ""; }

template <typename T>
std::string CAT(T arg)
{
  std::stringstream ss;
  ss << arg;
  return ss.str();
}

template <typename T, typename... Args>
std::string CAT(T current, Args... args)
{
  std::string result;
  result += CAT(current);
  result += " ";
  result += CAT(args...);
  return result;
}

#define CHECK(X, msg) do { if (X) {} else { \
  std::cerr << __FILE__ << ":" << __LINE__ << " " \
  << __FUNCTION__ << "() CHECK("#X"): " << CAT msg \
  << std::endl; std::abort(); } } while(false)