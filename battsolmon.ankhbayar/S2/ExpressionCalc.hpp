#ifndef EXPRESSIONCALC_HPP
#define EXPRESSIONCALC_HPP
#include "queue.hpp"
#include "stack.hpp"
#include <string>

namespace ankh
{
  class ExpressionCalc
  {
    std::string InfixToPostfix(const std::string& expr);
    long long ParseNum(const std::string& str, size_t& pos);
    long long Calculate(const std::string& postfix);

  public:
    ExpressionCalc() = default;
    void readExpression(std::istream& input);
    void CalculateExpressions(Stack< long long >&);
  private:
    Queue< std::string > expressions;
  };
}
#endif
