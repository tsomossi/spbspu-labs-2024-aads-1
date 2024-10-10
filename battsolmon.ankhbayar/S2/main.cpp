#include "ExpressionCalc.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
  ankh::ExpressionCalc calculator;
  ankh::Stack< long long > results;
  if (argc == 1)
  {
    calculator.readExpression(std::cin);
  }
  else if (argc == 2)
  {
    std::ifstream stream(argv[1]);
    calculator.readExpression(stream);
  }
  else
  {
    std::cerr << "Invalid number of command line arguments";
  }
  try
  {
    calculator.CalculateExpressions(results);
  }
  catch (const std::runtime_error& err)
  {
    std::cout << err.what();
    return 1;
  }
  while (!results.empty())
  {
    std::cout << results.top();
    if (results.size() != 1)
    {
      std::cout << ' ';
    }
    results.pop();
  }
  std::cout << "\n";
  return 0;
}
