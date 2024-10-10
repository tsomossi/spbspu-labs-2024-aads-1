#include "ExpressionCalc.hpp"
#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>
#include <climits>

namespace ankh
{
  std::unordered_map< char, int > priority = { {'-', 0}, {'+', 0}, {'*', 1}, {'/', 1}, {'%', 1}, {'(', -1}, {')', -1} };
  using Func = long long (*)(long long, long long);
  long long sum(long long a, long long b)
  {
    long long max = LLONG_MAX;
    if (a > max - b)
    {
      throw std::runtime_error("Sum overflow\n");
    }
    return a + b;
  }

  long long mult(long long a, long long b)
  {
    if (a == 0 || b == 0)
    {
      return 0;
    }

    bool sign = ((a < 0) ^ (b < 0));
    a = std::abs(a);
    b = std::abs(b);

    if (a > (std::numeric_limits< long long >::max() / b))
    {
      throw std::runtime_error("Mult overflow\n");
    }

    long long result = a * b;
    return sign ? -result : result;
  }

  long long div(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::runtime_error("Division by zero\n");
    }

    bool sign = ((a < 0) ^ (b < 0));
    a = std::abs(a);
    b = std::abs(b);

    if (a > std::numeric_limits< long long >::max() - std::abs(b) + 1)
    {
      throw std::runtime_error("Division overflow\n");
    }

    long long result = a / b;
    return sign ? -result : result;
  }

  long long sub(long long a, long long b)
  {
    if ((b > 0 && a < LLONG_MIN + b) || (b < 0 && a > LLONG_MAX + b))
    {
      throw std::runtime_error("Sub overflow\n");
    }
    return a - b;
  }

  long long mod(long long a, long long b)
  {
    auto res = a % b;
    if (res < 0)
    {
      res += abs(b);
    }
    return res;
  }

  std::unordered_map< char, Func > operation = { {'+', sum}, {'-', sub}, {'%', mod}, {'*', mult}, {'/', div} };

  std::string ExpressionCalc::InfixToPostfix(const std::string& expr)
  {
    std::string res;
    res.reserve(expr.size());
    std::stack< char > operators;
    size_t pos = 0;

    while (pos < expr.size())
    {
      unsigned char symbol = static_cast<unsigned char>(expr[pos]);
      if (std::isdigit(symbol))
      {
        res.push_back(symbol);
        ++pos;
      }
      else if (symbol == '(')
      {
        operators.push(symbol);
        ++pos;
      }
      else if (symbol == ')')
      {
        char oper = operators.top();
        while (oper != '(')
        {
          res.push_back(' ');
          res.push_back(oper);
          operators.pop();
          oper = operators.top();
        }
        operators.pop();
        ++pos;
      }
      else if (std::isspace(symbol))
      {
        ++pos;
      }
      else
      {
        while (!operators.empty())
        {
          char oper = operators.top();
          auto isOperand = priority.find(oper);
          if (isOperand == priority.end())
          {
            throw std::runtime_error("Error!Invalid operand\n");
          }
          if (priority[oper] >= priority[symbol])
          {
            res.push_back(' ');
            res.push_back(oper);
            operators.pop();
            continue;
          }
          break;
        }
        res.push_back(' ');
        operators.push(symbol);
        ++pos;
      }
    }
    while (!operators.empty())
    {
      res.push_back(' ');
      res.push_back(operators.top());
      operators.pop();
    }
    return res;
  }

  void ExpressionCalc::readExpression(std::istream& stream)
  {
    std::string line;
    while (!stream.eof())
    {
      std::getline(stream, line);
      if (!line.empty())
      {
        expressions.push(line);
      }
    }
  }

  void ExpressionCalc::CalculateExpressions(Stack< long long >& results)
  {
    while (!expressions.empty())
    {
      auto& expr = expressions.front();
      expr = InfixToPostfix(expr);
      if (expr.empty())
      {
        expressions.pop();
        continue;
      }
      results.push(Calculate(expr));
      expressions.pop();
    }
  }

  long long ExpressionCalc::ParseNum(const std::string& str, size_t& pos)
  {
    auto symbol = static_cast<unsigned char>(str[pos]);
    long long result = 0;
    while (std::isdigit(symbol))
    {
      result = result * 10 + (symbol - '0');
      if (pos == str.size() - 1)
      {
        pos++;
        break;
      }
      symbol = static_cast<unsigned char>(str[++pos]);
    }
    return result;
  }

  long long ExpressionCalc::Calculate(const std::string& postfix)
  {
    std::stack< long long > result;
    size_t pos = 0;
    while (pos < postfix.size())
    {
      auto symbol = static_cast<unsigned char>(postfix[pos]);
      if (std::isspace(symbol))
      {
        ++pos;
      }
      else if (std::isdigit(symbol))
      {
        result.push(ParseNum(postfix, pos));
      }
      else
      {
        auto oprt = operation.find(symbol);
        if (oprt == operation.end())
        {
          throw std::runtime_error("Error!Invalid operand\n");
        }
        auto roperand = result.top();
        result.pop();
        auto loperand = result.top();
        result.pop();
        long long res = oprt->second(loperand, roperand);
        result.push(res);
        ++pos;
      }
    }
    return result.top();
  }
};
