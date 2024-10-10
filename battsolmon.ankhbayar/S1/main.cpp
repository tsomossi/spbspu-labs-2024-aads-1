#include <iostream>
#include <cstddef>
#include "ListProcess.hpp"

int main()
{
  using namespace ankh;
  List< std::pair< std::string, List< size_t > > > list;
  List< size_t > sums;
  bool is_overflow = false;
  try
  {
    inputList(std::cin, list);
    sums = countSums(list, is_overflow);
    outputNames(std::cout, list);
    outputArgs(std::cout, list, is_overflow);
    outputSums(std::cout, sums);
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  return 0;
}
