#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

namespace ankh
{
  template< typename T >
  struct List
  {
  public:
    class Iterator;
    class ConstIterator;

    List();
    ~List();

    void pop_front();
    void push_back(const T& value);
    void clear();
    size_t size() const;

    void push_front(const T& value);
    void insert(const T& value, int index);
    void removeAt(int index);
    void pop_back();

    Iterator begin() const;
    Iterator end() const;

    ConstIterator cBegin() const;
    ConstIterator cEnd() const;

  private:
    template< typename U >
    class Node
    {
    public:
      Node* nextPoint;
      T  data;
      Node(T data = T(), Node* nextPoint = nullptr)
      {
        this->data = data;
        this->nextPoint = nextPoint;
      }
    };
    Node< T >* head;
    int s = 0;
  };
}

template< typename T >
class ankh::List< T >::ConstIterator : public std::iterator<std::forward_iterator_tag, T>
{
public:
  ConstIterator();
  ConstIterator(Node< T >* ptr);
  ~ConstIterator() = default;

  void operator++();

  const T& operator*();
  const T* operator->();

  bool operator!=(const Iterator& other) const;
  bool operator==(const Iterator& other) const;

private:
  Node< T >* ptr;
};

template< typename T >
ankh::List< T >::ConstIterator::ConstIterator() :
  ptr(nullptr)
{}

template< typename T >
ankh::List< T >::ConstIterator::ConstIterator(Node< T >* ptr) :
  ptr(ptr)
{}

template< typename T >
void ankh::List< T >::ConstIterator::operator++()
{
  ptr = ptr->nextPoint;
}

template< typename T >
const T& ankh::List< T >::ConstIterator::operator*()
{
  return ptr->data;
}

template< typename T >
const T* ankh::List< T >::ConstIterator::operator->()
{
  return std::addressof(ptr->data);
}

template< typename T >
bool ankh::List< T >::ConstIterator::operator!=(const Iterator& other) const
{
  return ptr != other.ptr;
}

template< typename T >
bool ankh::List< T >::ConstIterator::operator==(const Iterator& other) const
{
  return ptr == other.ptr;
}

template< typename T >
class ankh::List< T >::Iterator : public std::iterator<std::forward_iterator_tag, T>
{
public:
  Iterator();
  Iterator(Node< T >* ptr);
  ~Iterator() = default;

  void operator++();

  T& operator*();
  T* operator->();

  bool operator!=(const Iterator& other);
  bool operator==(const Iterator& other);

private:
  Node< T >* ptr;
};

template< typename T >
ankh::List< T >::Iterator::Iterator() :
  ptr(nullptr)
{}

template< typename T >
ankh::List< T >::Iterator::Iterator(Node< T >* ptr) :
  ptr(ptr)
{}

template< typename T >
void ankh::List< T >::Iterator::operator++()
{
  ptr = ptr->nextPoint;
}

template< typename T >
T& ankh::List< T >::Iterator::operator*()
{
  return ptr->data;
}

template< typename T >
T* ankh::List< T >::Iterator::operator->()
{
  return std::addressof(ptr->data);
}

template< typename T >
bool ankh::List< T >::Iterator::operator!=(const Iterator& other)
{
  return ptr != other.ptr;
}

template< typename T >
bool ankh::List< T >::Iterator::operator==(const Iterator& other)
{
  return ptr == other.ptr;
}

template< typename T >
ankh::List< T >::List()
{
  s = 0;
  head = nullptr;
}

template< typename T >
ankh::List< T >::~List()
{
  clear();
}

template< typename T >
void ankh::List< T >::pop_front()
{
  Node<T>* temp = head;
  head = head->nextPoint;
  delete temp;
  s--;
}

template< typename T >
void ankh::List< T >::push_back(const T& value)
{
  if (head == nullptr)
  {
    head = new Node< T >(value);
  }
  else
  {
    Node< T >* current = head;
    while (current->nextPoint != nullptr)
    {
      current = current->nextPoint;
    }
    current->nextPoint = new Node< T >(value);
  }
  s++;
}

template< typename T >
void ankh::List< T >::clear()
{
  while (s)
  {
    pop_front();
  }
}

template< typename T >
size_t ankh::List< T >::size() const
{
  return s;
}

template< typename T >
void ankh::List< T >::push_front(const T& value)
{
  head = new Node< T >(value, head);
  s++;
}

template< typename T >
void ankh::List< T >::insert(const T& value, int index)
{
  if (index == 0)
  {
    push_front(value);
  }
  else
  {
    Node< T >* prevPoint = this->head;
    for (int i = 0; i < index - 1; i++)
    {
      prevPoint = prevPoint->nextPoint;
    }
    Node< T >* newNode = new Node< T >(value, prevPoint->nextPoint);
    prevPoint->nextPoint = newNode;
    s++;
  }
}

template< typename T >
void ankh::List< T >::removeAt(int index)
{
  if (index == 0)
  {
    pop_front();
  }
  else
  {
    Node< T >* prevPoint = this->head;
    for (int i = 0; i < index - 1; i++)
    {
      prevPoint = prevPoint->nextPoint;
    }
    Node< T >* forDelete = prevPoint->nextPoint;
    prevPoint->nextPoint = forDelete->nextPoint;
    delete forDelete;
    s--;
  }
}

template< typename T >
void ankh::List< T >::pop_back()
{
  removeAt(s - 1);
}

template< typename T >
typename ankh::List< T >::Iterator ankh::List< T >::begin() const
{
  return Iterator(head);
}

template< typename T >
typename ankh::List< T >::Iterator ankh::List< T >::end() const
{
  return Iterator(nullptr);
}

template< typename T >
typename ankh::List< T >::ConstIterator ankh::List< T >::cBegin() const
{
  return ConstIterator(head);
}

template< typename T >
typename ankh::List< T >::ConstIterator ankh::List< T >::cEnd() const
{
  return ConstIterator(nullptr);
}
#endif
