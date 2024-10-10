#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "list.hpp"

namespace ankh
{
  template< typename T >
  class Queue
  {
  public:
    Queue() : _list(List< T >()) {}
    Queue(const Queue& other);
    Queue(Queue&& other);
    explicit Queue(const List< T >& cont);
    ~Queue() = default;

    Queue< T >& operator=(const Queue& other);
    Queue< T >& operator=(Queue&& other);

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    bool empty() const noexcept;
    size_t size() const;

    void push(const T& value);
    void push(T&& value);
    void pop();
    void swap(Queue& other) noexcept;
  private:
    List< T > _list;
  };

  template< typename T >
  Queue< T >::Queue(const List< T >& cont) :
    _list(cont)
  {}

  template< typename T >
  Queue< T >::Queue(const Queue& other) :
    _list(other.container_)
  {}

  template< typename T >
  Queue< T >::Queue(Queue&& other) :
    _list(std::move(other.container_))
  {}

  template< typename T >
  Queue< T >& Queue< T >::operator=(const Queue& other)
  {
    _list = other.container_;
  }

  template< typename T >
  Queue< T >& Queue< T >::operator=(Queue&& other)
  {
    _list = std::move(other.container_);
  }

  template< typename T >
  T& Queue< T >::front()
  {
    return *_list.begin();
  }

  template< typename T >
  const T& Queue< T >::front() const
  {
    return _list.front();
  }

  template< typename T >
  T& Queue< T >::back()
  {
    if (_list.isEmpty())
    {
      throw std::logic_error("Error: empty container");
    }
    typename List< T >::iterator curr = _list.begin();
    typename List< T >::iterator next = ++curr;
    while (next != _list.end())
    {
      ++curr;
      ++next;
    }
    return (*curr);
  }

  template< typename T >
  const T& Queue< T >::back() const
  {
    if (_list.isEmpty())
    {
      throw std::logic_error("Error: empty container");
    }
    typename List< T >::iterator curr = _list.begin();
    typename List< T >::iterator next = ++curr;
    while (next != _list.end())
    {
      ++curr;
      ++next;
    }
    return (*curr);
  }

  template< typename T >
  bool  Queue< T >::empty() const noexcept
  {
    return _list.size() > 0 ? false : true;
  }

  template< typename T >
  size_t  Queue< T >::size() const
  {
    size_t size = 0;
    typename List< T >::iterator curr = _list.begin();
    while (curr != _list.end())
    {
      ++size;
      ++curr;
    }
    return size;
  }

  template< typename T >
  void  Queue< T >::push(const T& value)
  {
    _list.push_back(value);
  }

  template< typename T >
  void  Queue< T >::push(T&& value)
  {
    _list.pushBack(std::move(value));
  }

  template< typename T >
  void  Queue< T >::pop()
  {
    _list.pop_front();
  }

  template< typename T >
  void  Queue< T >::swap(Queue& other) noexcept
  {
    _list.swap(other.container_);
  }
}
#endif
