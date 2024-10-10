#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "binarySearchTreeAVL.hpp"
#include "IOFunctions.hpp"

using data_t = ankh::BSTree< int, std::string, int >;
using tree_t = ankh::BSTree< std::string, data_t, int >;

namespace ankh
{
  void makePrint(std::istream & in, std::ostream & out, tree_t & data);
  void makeComplement(std::istream & in, std::ostream &, tree_t & data);
  void makeIntersect(std::istream & in, std::ostream &, tree_t & data);
  void makeUnion(std::istream & in, std::ostream &, tree_t & data);
}

#endif
