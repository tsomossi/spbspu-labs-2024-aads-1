#include <fstream>
#include <iostream>
#include <limits>
#include <functional>
#include "commands.hpp"

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "error: wrong parameters\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "error: can't open the file\n";
    return 1;
  }

  using data_t = ankh::BSTree< int, std::string, int >;
  using tree_t = ankh::BSTree< std::string, data_t, int >;
  tree_t data;
  ankh::inputFromFile(input, data);
  BSTree< std::string, std::function< void (std::istream &, std::ostream &, tree_t &) >, int > commands;
  {
    commands["print"] = ankh::makePrint;
    commands["complement"] = ankh::makeComplement;
    commands["intersect"] = ankh::makeIntersect;
    commands["union"] = ankh::makeUnion;
  }

  std::string command = "";
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)(std::cin, std::cout, data);
    }
    catch (const std::out_of_range &)
    {
      ankh::printInvalidCommandMessage(std::cout);
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  for (tree_t::Iterator iter = data.begin(); iter != data.end(); iter++)
  {
    iter->second.clear();
  }
  data.clear();
  commands.clear();

  return 0;
}
