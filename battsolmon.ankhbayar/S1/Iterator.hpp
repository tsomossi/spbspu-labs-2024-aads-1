#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <memory>
#include "Node.hpp"

namespace ankh
{
  namespace details
  {
    template< typename T >
    class Iterator
    {
    public:
      using this_t = Iterator;
      Iterator(): node_(nullptr) {};
      Iterator(details::Node< T > * ptr) : node_(ptr) {};
      Iterator(const this_t &) = default;
      ~Iterator() = default;
      this_t & operator=(const this_t &) = default;
      this_t & operator++();
      this_t operator++(int);
      this_t & operator--();
      this_t operator--(int);
      T & operator*();
      T * operator->();
      const T & operator*() const;
      const T * operator->() const;
      bool operator!=(const Iterator &) const;
      bool operator==(const Iterator &) const;
    private:
      details::Node< T > * node_;
    };
  }
}

template< typename T >
typename ankh::details::Iterator< T > & ankh::details::Iterator< T >::operator++()
{
  assert(node_ != nullptr);
  node_ = node_->next_;
  return *this;
}

template< typename T >
typename ankh::details::Iterator< T > ankh::details::Iterator< T >::operator++(int)
{
  assert(node_ != nullptr);
  Iterator< T > result(*this);
  node_ = node_->next_;
  return result;
}

template< typename T >
typename ankh::details::Iterator< T > & ankh::details::Iterator< T >::operator--()
{
  assert(node_ != nullptr);
  node_ = node_->prev_;
  return *this;
}

template< typename T >
typename ankh::details::Iterator< T > ankh::details::Iterator< T >::operator--(int)
{
  assert(node_ != nullptr);
  Iterator< T > result(*this);
  node_ = node_->prev_;
  return result;
}

template< typename T >
bool ankh::details::Iterator< T >::operator==(const ankh::details::Iterator< T > & rhs) const
{
  return node_ == rhs.node_;
}

template< typename T >
bool ankh::details::Iterator< T >::operator!=(const ankh::details::Iterator< T > & rhs) const
{
  return !(rhs == *this);
}

template< typename T >
T & ankh::details::Iterator< T >::operator*()
{
  assert(node_ != nullptr);
  return node_->data_;
}

template< typename T >
T * ankh::details::Iterator< T >::operator->()
{
  assert(node_ != nullptr);
  return std::addressof(node_->data_);
}

#endif
