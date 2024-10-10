#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"

namespace ankh
{
  template< typename T >
  class Stack
  {
  public:
    Stack() : _list(List< T >()) {}
    Stack(const Stack< T >& other);
    Stack(Stack< T >&& other);
    explicit Stack(const List< T >& cont);
    ~Stack() = default;

    Stack& operator=(const Stack& other);
    Stack& operator=(Stack&& other);

    T& top();
    const T& top() const;

    bool empty() const noexcept;
    size_t size() const;

    void push(const T& value);
    void push(T&& value);
    void pop();
    void swap(Stack& other) noexcept;
  private:
    List< T > _list;
  };

  template< typename T >
  Stack< T >::Stack(const List< T >& cont) :
    _list(cont)
  {}

  template< typename T >
  Stack< T >::Stack(const Stack& other) :
    _list(other._list)
  {}

  template< typename T >
  Stack< T >::Stack(Stack&& other) :
    _list(std::move(other._list))
  {}

  template< typename T >
  Stack< T >& Stack< T >::operator=(const Stack& other)
  {
    _list = other._list;
  }

  template< typename T >
  Stack< T >& Stack< T >::operator=(Stack&& other)
  {
    _list = std::move(other._list);
  }

  template< typename T >
  T& Stack< T >::top()
  {
    return *_list.begin();
  }

  template< typename T >
  const T& Stack< T >::top() const
  {
    return *_list.begin();
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return _list.size() > 0 ? false : true;
  }

  template< typename T >
  size_t Stack< T >::size() const
  {
    size_t size = 0;
    typename List< T >::Iterator curr = _list.begin();
    while (curr != _list.end())
    {
      ++size;
      ++curr;
    }
    return size;
  }

  template< typename T >
  void Stack< T >::push(const T& value)
  {
    _list.push_front(value);
  }

  template< typename T >
  void Stack< T >::push(T&& value)
  {
    _list.push_front(value);
  }

  template< typename T >
  void Stack< T >::pop()
  {
    _list.pop_front();
  }

  template< typename T >
  void Stack< T >::swap(Stack& other) noexcept
  {
    _list.swap(other._list);
  }
}
#endif
