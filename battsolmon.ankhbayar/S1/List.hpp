#ifndef LIST_HPP
#define LIST_HPP
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include <iterator>
#include "Node.hpp"
#include "Iterator.hpp"
#include "ConstIterator.hpp"

namespace ankh
{
  template< typename T >
  class List
  {
  public:
    List();
    List(const List & other);
    List(List && other);
    List(size_t count, const T & value);
    List(std::initializer_list<T> init);
    template < class InputIterator >
    List(InputIterator first, InputIterator last);
    ~List();

    List< T >& operator=(const List< T >& other);
    List< T >& operator=(List< T >&& other);

    void pushFront(const T & val);
    void pushBack(const T & val);
    void popFront();
    void popBack();
    T & getFront() const noexcept;
    T & getBack() const noexcept;
    bool empty() noexcept;
    void clear() noexcept;
    void swap(List & other);
    details::ConstIterator< T > begin() const noexcept;
    details::ConstIterator< T > end() const noexcept;
    details::Iterator< T > begin() noexcept;
    details::Iterator< T > end() noexcept;

    void remove(const T& value);
    template< class UnaryPredicate >
    void remove_if(UnaryPredicate p);
    void assign(size_t count, const T& value);
    template< class InputIt >
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<T> ilist);
    details::Iterator< T > insert(details::Iterator< T > pos, const T& value);
    details::Iterator< T > erase(details::Iterator< T > pos);
    void reverse();
    void splice(details::Iterator< T > position, List< T >& x);
    size_t getSize() const noexcept;

  private:
    size_t size_;
    details::Node< T > * head_;
    details::Node< T > * tail_;
  };
}

template< typename T >
ankh::List< T >::List():
  size_(0),
  head_(nullptr),
  tail_(nullptr)
{}

template< typename T >
ankh::List<T>::List(const List & other):
  size_(0),
  head_(nullptr),
  tail_(nullptr)
{
  details::Node< T > * temp = other.head_;
  while(temp)
  {
    pushBack(temp->data_);
    temp = temp->next_;
  }
}

template< typename T >
ankh::List< T >::List(List && other):
  size_(other.size_),
  head_(other.head_),
  tail_(other.tail_)
{
  other.size_ = 0;
  other.head_ = nullptr;
  other.tail_ = nullptr;
}

template< typename T >
ankh::List< T >::List(size_t count, const T & value):
List()
{
  for (size_t i; i < count; i++)
  {
    pushBack(value);
  }
}

template< typename T >
ankh::List< T >::List(std::initializer_list<T> init):
List()
{
  const T * ptr = init.begin();
  while (ptr)
  {
    pushBack(*ptr);
    ptr++;
  }
}

template< typename T >
template < class InputIterator >
ankh::List< T >::List(InputIterator first, InputIterator last):
List()
{
  while (first != last)
  {
    pushBack(*first);
    first++;
  }
}

template< typename T >
ankh::List<T>::~List()
{
  clear();
  delete head_;
}

template< typename T >
ankh::List< T >& ankh::List< T >::operator=(const List< T >& other)
{
  List< T > temp(other);
  if (std::addressof(other) != this)
  {
    swap(temp);
  }
  return *this;
}

template< typename T >
ankh::List< T >& ankh::List< T >::operator=(List< T >&& other)
{
  List< T > temp(std::move(other));
  if (std::addressof(other) != this)
  {
    swap(temp);
  }
  return *this;
}

template< typename T >
void ankh::List<T>::pushFront(const T & val)
{
  details::Node< T > * newNode = new details::Node< T >(val, nullptr, head_);
  if (empty())
  {
    tail_ = newNode;
    head_ = newNode;
  }
  else
  {
    head_->prev_ = newNode;
    head_ = newNode;
  }
  size_++;
}

template< typename T >
void ankh::List<T>::pushBack(const T & val)
{
  details::Node< T > * newNode = new details::Node< T >(val, tail_, nullptr);
  if (empty())
  {
    tail_ = newNode;
    head_ = newNode;
  }
  else
  {
    tail_->next_ = newNode;
    tail_ = newNode;
  }
  size_++;
}

template< typename T >
void ankh::List<T>::popFront()
{
  if (empty())
  {
    throw std::logic_error("can't delete element from empty list");
  }
  details::Node< T > * temp = head_;
  head_ = head_->next_;
  if (temp->next_ == nullptr)
  {
    tail_ = nullptr;
  }
  delete temp;
  size_--;
}

template< typename T >
void ankh::List<T>::popBack()
{
  if (empty())
  {
    throw std::logic_error("can't delete element from empty list");
  }
  details::Node< T > * temp = tail_;
  tail_ = tail_->prev_;
  if (size_ == 1)
  {
    head_ = nullptr;
  }
  delete temp;
  size_--;
}

template< typename T >
T & ankh::List<T>::getFront() const noexcept
{
  return head_->data_;
}

template< typename T >
T & ankh::List<T>::getBack() const noexcept
{
  return tail_->data_;
}

template< typename T >
bool ankh::List<T>::empty() noexcept
{
  return (size_ == 0);
}

template< typename T >
void ankh::List<T>::clear() noexcept
{
  while (!empty())
  {
    popFront();
  }
}

template< typename T >
void ankh::List<T>::swap(List & other)
{
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

template< typename T >
typename ankh::details::ConstIterator< T > ankh::List< T >::begin() const noexcept
{
  return details::ConstIterator< T >(head_);
}

template< typename T >
typename ankh::details::ConstIterator< T > ankh::List< T >::end() const noexcept
{
  return details::ConstIterator< T >(nullptr);
}

template< typename T >
typename ankh::details::Iterator< T > ankh::List< T >::begin() noexcept
{
  return details::Iterator< T >(head_);
}

template< typename T >
typename ankh::details::Iterator< T > ankh::List< T >::end() noexcept
{
  return details::Iterator< T >(nullptr);
}

template< typename T >
void ankh::List< T >::remove(const T& value)
{
  details::Node< T > * temp = head_;
  while (temp)
  {
    if ((temp->data_ == value) && (temp == head_))
    {
      popFront();
    }
    else if ((temp->data_ == value) && (temp == tail_))
    {
      popBack();
    }
    else if ((temp->data_ == value) && (temp != tail_) && (temp != head_))
    {
      temp->next_->prev_ = temp->prev_;
      temp->prev_->next_ = temp->next_;
      delete temp;
      size_--;
    }
    temp = temp->next_;
  }
}

template< typename T >
template< class UnaryPredicate>
void ankh::List< T >::remove_if(UnaryPredicate p)
{
  details::Node< T > * temp = head_;
  while (temp)
  {
    if ((p(temp->data_)) && (temp == head_))
    {
      popFront();
    }
    else if ((p(temp->data_)) && (temp == tail_))
    {
      popBack();
    }
    else if ((p(temp->data_)) && (temp != tail_) && (temp != head_))
    {
      temp->next_->prev_ = temp->prev_;
      temp->prev_->next_ = temp->next_;
      delete temp;
      size_--;
    }
    temp = temp->next_;
  }
}

template< typename T >
void ankh::List< T >::assign(size_t count, const T& value)
{
  clear();
  for (size_t i = 0; i < count; i++)
  {
    pushBack(value);
  }
}

template< typename T >
template< class InputIt >
void ankh::List< T >::assign(InputIt first, InputIt last)
{
  clear();
  while (first != last)
  {
    pushBack(*first);
    first++;
  }
}

template< typename T >
void ankh::List< T >::assign(std::initializer_list<T> ilist)
{
  clear();
  const T * ptr = ilist.begin();
  while (ptr)
  {
    pushBack(*ptr);
    ptr++;
  }
}

template< typename T >
typename ankh::details::Iterator< T > ankh::List< T >::insert(details::Iterator< T > pos, const T& value)
{
  details::Iterator< T > it;
  if (pos == head_)
  {
    pushFront(value);
    it = head_;
  }
  else
  {
    details::Node< T > * node = new details::Node< T >(value, pos->prev_, pos->prev_->next_);
    it = node;
    pos->prev_ = it;
    pos->prev_next_ = it;
  }
  return it;
}

template< typename T >
typename ankh::details::Iterator< T > ankh::List< T >::erase(details::Iterator< T > pos)
{
  details::Iterator< T > it;
  if (pos == head_)
  {
    popFront();
    it = head_;
  }
  else if (pos == tail_)
  {
    popBack();
    it = tail_;
  }
  else
  {
    pos->next_->prev_ = pos->prev_;
    pos->prev_->next_ = pos->next_;
    it = pos->next_;
    delete pos;
  }
  return it;
}

template< typename T >
void ankh::List< T >::reverse()
{
  details::Node< T > * temp = head_;
  while (head_->next_)
  {
    std::swap(head_->next_, head_->prev_);
    head_ = head_->prev_;
  }
  std::swap(tail_, temp);
  delete temp;
}

template< typename T >
void ankh::List< T >::splice(ankh::details::Iterator< T > position, List< T >& x)
{
  while (x.head_)
  {
    this->insert(position);
    position++;
    x.head_ = x.head_->next_;
  }
  delete x;
}

template< typename T >
size_t ankh::List< T >::getSize() const noexcept
{
  return size_;
}

#endif
