#ifndef NODE_HPP
#define NODE_HPP

namespace ankh
{
  namespace details
  {
    template< typename T >
    struct Node
    {
      T data_;
      Node * prev_;
      Node * next_;
      Node() : data_(0), prev_(nullptr), next_(nullptr) {}
      Node(const T & data, Node * prev, Node * next) : data_(data), prev_(prev), next_(next) {}
    };
  }
}

#endif
