#include "commands.hpp"
#include <iostream>

void ankh::makePrint(std::istream & in, std::ostream & out, tree_t & data)
{
  std::string datasetName = "";
  in >> datasetName;
  data_t dataset(data.at(datasetName));

  if (dataset.getSize() == 0)
  {
    printEmptyDataset(out);
    return;
  }

  out << datasetName << " ";
  for (data_t::ConstIterator iter(dataset.cbegin()); iter != dataset.cend();)
  {
    out << iter->first << " " << iter->second;
    iter++;
    out << (iter != dataset.cend() ? " " : "\n");
  }
}

void ankh::makeComplement(std::istream & in, std::ostream &, tree_t & data)
{
  std::string newDataName = "";
  std::string dataset1 = "";
  std::string dataset2 = "";
  in >> newDataName >> dataset1 >> dataset2;
  data_t newData;
  if (data.at(dataset1).getSize() != 0 && data.at(dataset2).getSize() != 0)
  {
    for (data_t::ConstIterator iter(data.at(dataset1).cbegin()); iter != data.at(dataset1).cend(); iter++)
    {
      if (data.at(dataset2).find(iter->first) == data.at(dataset2).end())
      {
        newData.insert(std::make_pair(iter->first, iter->second));
      }
    }
  }

  if (data.find(newDataName) != data.end())
  {
    data.at(newDataName).swap(newData);
    newData.clear();
  }
  else
  {
    data.insert(std::make_pair(newDataName, newData));
  }
}

void ankh::makeIntersect(std::istream & in, std::ostream &, tree_t & data)
{
  std::string newDataName = "";
  std::string dataset1 = "";
  std::string dataset2 = "";
  in >> newDataName >> dataset1 >> dataset2;
  data_t newData;
  if (data.at(dataset1).getSize() != 0 && data.at(dataset2).getSize() != 0)
  {
    for (data_t::ConstIterator iter(data.at(dataset1).cbegin()); iter != data.at(dataset1).cend(); iter++)
    {
      if (data.at(dataset2).find(iter->first) != data.at(dataset2).end())
      {
        newData.insert(std::make_pair(iter->first, iter->second));
      }
    }
  }

  if (data.find(newDataName) != data.end())
  {
    data.at(newDataName).swap(newData);
    newData.clear();
  }
  else
  {
    data.insert(std::make_pair(newDataName, newData));
  }
}

void ankh::makeUnion(std::istream & in, std::ostream &, tree_t & data)
{
  std::string newDataName = "";
  std::string dataset1 = "";
  std::string dataset2 = "";
  in >> newDataName >> dataset1 >> dataset2;
  data_t newData;
  if (data.at(dataset1).getSize() != 0 && data.at(dataset2).getSize() == 0)
  {
    for (data_t::ConstIterator iter(data.at(dataset1).cbegin()); iter != data.at(dataset1).cend(); iter++)
    {
      newData.insert(std::make_pair(iter->first, iter->second));
    }
  }
  else if (data.at(dataset1).getSize() == 0 && data.at(dataset2).getSize() != 0)
  {
    for (data_t::ConstIterator iter(data.at(dataset2).cbegin()); iter != data.at(dataset2).cend(); iter++)
    {
      newData.insert(std::make_pair(iter->first, iter->second));
    }
  }
  else if (data.at(dataset1).getSize() != 0 || data.at(dataset2).getSize() != 0)
  {
    for (data_t::ConstIterator iter(data.at(dataset1).cbegin()); iter != data.at(dataset1).cend(); iter++)
    {
      newData.insert(std::make_pair(iter->first, iter->second));
    }

    for (data_t::ConstIterator iter(data.at(dataset2).cbegin()); iter != data.at(dataset2).cend(); iter++)
    {
      if (data.at(dataset1).find(iter->first) == data.at(dataset1).end())
      {
        newData.insert(std::make_pair(iter->first, iter->second));
      }
    }
  }

  if (data.find(newDataName) != data.end())
  {
    data.at(newDataName).swap(newData);
    newData.clear();
  }
  else
  {
    data.insert(std::make_pair(newDataName, newData));
  }
}
