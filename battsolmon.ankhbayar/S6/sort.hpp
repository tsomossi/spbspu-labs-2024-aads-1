#ifndef SORT_HPP
#define SORT_HPP
#include "list.hpp"
#include <string>
#include <list>
#include <forward_list>
#include <deque>
#include <random>
#include <iomanip>

namespace ankh
{
  using str = const std::string;
  template< typename T >
  void randCont(str type, size_t elCnt, List< T >& fList, std::list< T >& bList, std::deque< T >& queue);

  template< typename T >
  void printCont(std::ostream& out, const T& container);

  template< typename Iterator, typename Comparator >
  void oddEvenSort(Iterator first, Iterator last, Comparator cmp);

  template< typename Iterator, typename Comparator >
  void mergeSort(Iterator first, Iterator last, Comparator cmp);

  template< typename Iterator, typename OtherIter, typename Comparator >
  void merge(Iterator fbeg, Iterator fend, Iterator sbeg, Iterator send, OtherIter dest, Comparator cmp);

  template< typename T, typename Compare >
  void sortData(std::ostream& out, std::string type, size_t size, Compare cmp);
}

template< typename T >
void ankh::randCont(str type, size_t elCnt, List< T >&fList, std::list< T >&bList, std::deque< T >&queue)
{
  std::random_device dev;
  std::mt19937 reng(dev());
  for (size_t i = 0; i < elCnt; i++)
  {
    if (type == "ints")
    {
      std::uniform_int_distribution< int > dis(-1000, 1000);
      int value = dis(dev);
      queue.push_back(value);
      fList.pushBack(value);
      bList.push_back(value);
    }
    else if (type == "floats")
    {
      std::uniform_real_distribution< float > dis(0.0f, 99.9f);
      float value = dis(dev);
      queue.push_back(value);
      fList.pushBack(value);
      bList.push_back(value);
    }
  }
}

template< typename T >
void ankh::printCont(std::ostream& out, const T& container)
{
  auto it = container.begin();
  out << *it;
  it++;
  while (it != container.end())
  {
    out << ' ' << *it;
    it++;
  }
  out << "\n";
}

template< typename Iterator, typename Comparator >
void ankh::oddEvenSort(Iterator first, Iterator last, Comparator cmp)
{
  if (std::distance(first, last) < 1)
  {
    return;
  }
  bool isSorted = false;
  while (!isSorted)
  {
    isSorted = true;
    Iterator current = first;
    Iterator next = first;
    next++;
    size_t number = 0;
    while (next != last)
    {
      if (number % 2 != 0)
      {
        if (cmp(*next, *current))
        {
          std::swap(*current, *next);
          isSorted = false;
        }
        current++;
        next++;
        number++;
      }
      else
      {
        current++;
        next++;
        number++;
      }
    }
    current = first;
    next = first;
    next++;
    number = 0;
    while (next != last)
    {
      if (number % 2 == 0)
      {
        if (cmp(*next, *current))
        {
          std::swap(*current, *next);
          isSorted = false;
        }
        current++;
        next++;
        number++;
      }
      else
      {
        current++;
        next++;
        number++;
      }
    }
  }
}

template< typename Iterator, typename Comparator >
void ankh::mergeSort(Iterator first, Iterator last, Comparator cmp)
{
  if (std::distance(first, last) < 2)
  {
    return;
  }
  size_t midIndex = std::distance(first, last) / 2;
  Iterator mid = first;
  size_t curIndex = 0;
  while (curIndex < midIndex)
  {
    mid++;
    curIndex++;
  }
  mergeSort(first, mid, cmp);
  mergeSort(mid, last, cmp);
  std::list< typename std::iterator_traits< Iterator >::value_type > temp;
  merge(first, mid, mid, last, std::back_inserter(temp), cmp);
  std::copy(temp.begin(), temp.end(), first);
}

template< typename Iterator, typename OtherIter, typename Comparator >
void ankh::merge(Iterator fbeg, Iterator fend, Iterator sbeg, Iterator send, OtherIter dest, Comparator cmp)
{
  while (fbeg != fend && sbeg != send)
  {
    if (cmp(*fbeg, *sbeg))
    {
      *dest = *fbeg;
      ++fbeg;
    }
    else
    {
      *dest = *sbeg;
      ++sbeg;
    }
    ++dest;
  }
  while (fbeg != fend)
  {
    *dest = *fbeg;
    ++fbeg;
    ++dest;
  }
  while (sbeg != send)
  {
    *dest = *sbeg;
    ++sbeg;
    ++dest;
  }
}

template< typename T, typename Compare >
void ankh::sortData(std::ostream& out, std::string type, size_t size, Compare cmp)
{
  List< T > forwardList;
  std::list< T > list;
  std::deque< T > deque;

  List< T > tempFwdList;
  std::list< T > tempList;
  std::deque< T > tempDeque;

  randCont(type, size, forwardList, list, deque);

  typename List< T >::Iterator first = forwardList.begin();
  typename List< T >::Iterator last = forwardList.end();
  while (first != last)
  {
    tempFwdList.pushBack(*first);
    first++;
  }

  std::copy(list.begin(), list.end(), std::back_inserter(tempList));
  std::copy(deque.begin(), deque.end(), std::back_inserter(tempDeque));

  printCont(out, forwardList);

  oddEvenSort(forwardList.begin(), forwardList.end(), cmp);
  printCont(out, forwardList);
  mergeSort(tempFwdList.begin(), tempFwdList.end(), cmp);
  printCont(out, tempFwdList);

  oddEvenSort(list.begin(), list.end(), cmp);
  printCont(out, list);
  mergeSort(tempList.begin(), tempList.end(), cmp);
  printCont(out, tempList);

  oddEvenSort(deque.begin(), deque.end(), cmp);
  printCont(out, deque);
  mergeSort(tempDeque.begin(), tempDeque.end(), cmp);
  printCont(out, tempDeque);
}

#endif
