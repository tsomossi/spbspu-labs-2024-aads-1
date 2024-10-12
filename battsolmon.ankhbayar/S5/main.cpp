#include <iostream>
#include <fstream>
#include <functional>
#include "commands.h"

using AVLtree = ankh::AVLTree<long long, std::string, long long>;

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    ankh::printErrorMessage(std::cout);
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input)
  {
    ankh::printErrorMessage(std::cout);
    return 1;
  }
  AVLtree allTree;
  try
  {
    while (!input.eof())
    {
      long long key = 0;
      std::string value = "";
      if (input >> key >> value)
      {
        allTree.insert(std::make_pair(key, value));
      }
    }
  }
  catch (const std::exception &e)
  {
    ankh::printErrorMessage(std::cout);
    allTree.clear();
    return 1;
  }
  if (allTree.empty())
  {
    ankh::printEmptyAVLTree(std::cout);
    return 0;
  }
  ankh::AVLTree<std::string, std::function<void(std::ostream &, AVLtree &)>, long long> commands;
  {
    commands.insert("ascending", ankh::ascending);
    commands.insert("descending", ankh::descending);
    commands.insert("breadth", ankh::breadth);
  }
  std::string direction = argv[1];
  try
  {
    commands.at(direction)(std::cout, allTree);
  }
  catch (const std::out_of_range &)
  {
    ankh::printInvalidCommandMessage(std::cout);
    clearTree(allTree, commands);
    return 1;
  }
  clearTree(allTree, commands);
  return 0;
}
