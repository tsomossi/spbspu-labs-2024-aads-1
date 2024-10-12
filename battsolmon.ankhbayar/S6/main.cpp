
#include <stdexcept>
#include <iostream>
#include "sort.hpp"

int main(int argc, char* argv[])
{
  using namespace ankh;
  if (argc != 4)
  {
    std::cerr << "Wrong input\n";
    return 1;
  }
  std::string cmp = argv[1];
  std::string type = argv[2];
  std::string stringSize = argv[3];

  if ((cmp != "ascending" && cmp != "descending") || (type != "ints" && type != "floats"))
  {
    std::cerr << "Wrong input\n";
    return 1;
  }

  size_t size;
  try
  {
    size = std::stoll(stringSize);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  if (size == 0)
  {
    std::cerr << "Wrong input\n";
    return 1;
  }

  if (cmp == "ascending")
  {
    if (type == "ints")
    {
      sortData< int, std::less< int > >(std::cout, type, size, std::less< int >{});
    }
    else
    {
      sortData< float, std::less< float > >(std::cout, type, size, std::less< float >{});
    }
  }
  else
  {
    if (type == "ints")
    {
      sortData< int, std::greater< int > >(std::cout, type, size, std::greater< int >{});
    }
    else
    {
      sortData< float, std::greater< float > >(std::cout, type, size, std::greater< float >{});
    }
  }

  return 0;
}
