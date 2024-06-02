#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include <utility>
#include <iterator>
#include <initializer_list>
#include <functional>
#include <iostream>

namespace mihalchenko
{
  template <typename Key, typename Value, typename Compare = std::less<Key>>
  class AVLTree
  {
  public:
    class ConstIterator;
    class Iterator;

    AVLTree();

    template <class InputIt>
    AVLTree(InputIt start, InputIt stop);

    using pair_t = std::pair<const Key, Value>;
    AVLTree(size_t count, pair_t &pairKeyVal);
    AVLTree(std::initializer_list<pair_t> initAVL);
    AVLTree(const AVLTree &copy);
    AVLTree(AVLTree &&move);
    ~AVLTree();

    void insert(Key key, Value value);

    void insert(pair_t pairKeyVal);
    bool empty() const noexcept;
    size_t getSize() const noexcept;
    size_t erase(const Key &key);
    void clear();

    Value &at(const Key &);
    const Value &at(const Key &key) const;

    Value &operator[](const Key &key);
    const Value &operator[](const Key &) const;
    Iterator find(const Key &);
    ConstIterator find(const Key &) const;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;
    Iterator begin() noexcept;
    Iterator end() noexcept;

  private:
    class Node
    {
    public:
      friend class AVLTree;
      Node(Key key, Value data, int height = 0, Node *left = nullptr, Node *right = nullptr, Node *previous = nullptr);

    private:
      pair_t pairOfKeyVal_;
      int height_;
      Node *left_;
      Node *right_;
      Node *previous_;
    };

    Node *insertNode(Key key, Value val, Node *node);
    Iterator findNode(const Key &key, Node *node);
    ConstIterator findNode(const Key &key, Node *node) const;

    void leftSpin(Node *node);
    void leftRightSpin(Node *node);
    void rightSpin(Node *node);
    void rightLeftSpin(Node *node);

    int calcHeight(Node *node);
    // int resiveHeight(Node * node);
    Node *SearchHiHeight(Node *node);

    //----------------------------------
    size_t updateHeight(Node *node);
    int checkBalance(Node *node);
    Node *isBalanced(Node *node);
    void balancingTree(Node *overweight);
    int getHeight(Node *node);
    Node *getRoot();
    const Node *getRoot() const;
    //----------------------------------

    Node *copyTree(Node *node, Node *previous);
    void doStableTree(Node *checkNode);
    void clear(Node *node);
    // void recombinateLinksRight(Node * deletedNode, Node * prevDelNode);
    // void recombinateLinksLeft(Node * deletedNode, Node * prevDelNode);

    Node *root_;
    size_t size_ = 0;
  };
}

template <typename Key, typename Value, typename Compare>
class mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator : public std::iterator<std::bidirectional_iterator_tag, std::pair<const Key, Value>>
{
public:
  friend class AVLTree<Key, Value, Compare>;
  ConstIterator();
  ConstIterator(Node *node, Node *root);
  ConstIterator(const ConstIterator &) = default;
  ~ConstIterator() = default;

  ConstIterator &operator++();
  ConstIterator operator++(int);
  ConstIterator &operator--();
  ConstIterator operator--(int);
  const std::pair<const Key, Value> &operator*() const;
  const std::pair<const Key, Value> *operator->() const;
  bool operator!=(const ConstIterator &rhs) const;
  bool operator==(const ConstIterator &rhs) const;
  ConstIterator &operator=(const ConstIterator &) = default;

private:
  Node *node_;
  Node *root_;
};

template <typename Key, typename Value, typename Compare>
class mihalchenko::AVLTree<Key, Value, Compare>::Iterator : public std::iterator<std::bidirectional_iterator_tag, Key, Value, Compare>
{
public:
  friend class AVLTree<Key, Value, Compare>;
  Iterator();
  Iterator(ConstIterator iter);
  Iterator(const Iterator &) = default;
  ~Iterator() = default;

  Iterator &operator++();
  Iterator operator++(int);
  Iterator &operator--();
  Iterator operator--(int);
  Iterator &operator=(const Iterator &) = default;
  pair_t &operator*();
  pair_t *operator->();
  const pair_t &operator*() const;
  const pair_t *operator->() const;
  bool operator!=(const Iterator &rhs) const;
  bool operator==(const Iterator &rhs) const;

private:
  ConstIterator constIter_;
};

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::Node::Node(Key key, Value data, int height, Node *left, Node *right, Node *previous) : pairOfKeyVal_(std::make_pair(key, data)), height_(height), left_(left), right_(right), previous_(previous)
{
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node *mihalchenko::AVLTree<Key, Value, Compare>::insertNode(Key key, Value val, Node *node)
{
  // std::cout << "мы в инит size_=" << size_ << "key=" << key << std::endl;
  Node *newNode = new Node(key, val, 0, nullptr, nullptr, nullptr);
  if (!node)
  {
    root_ = newNode;
    size_++;
    // std::cout << "insertNode:  root_ создали! size_=" << size_ << std::endl;
    return root_;
  }
  Compare compare;
  if ((node) && !compare(key, node->pairOfKeyVal_.first) && !compare(node->pairOfKeyVal_.first, key))
  {
    return node;
  }
  bool existNode = true;
  while (existNode)
  {
    if (compare(key, node->pairOfKeyVal_.first))
    {
      if (!node->left_)
      {
        node->left_ = newNode;
        newNode->previous_ = node;
        size_++;
        existNode = false;
        // std::cout << "left_ insertNode:  root_ создали! size_=" << size_ << std::endl;
      }
      node = node->left_;
    }
    else
    {
      if (!node->right_)
      {
        node->right_ = newNode;
        newNode->previous_ = node;
        size_++;
        existNode = false;
        // std::cout << "right_ insertNode:  root_ создали! size_=" << size_ << std::endl;
      }
      node = node->right_;
    }
  }
  // balance(newNode);

  // if (root_ != nullptr)
  balancingTree(node);
  //{
  // int finalWeight = calcHeight(root_->right_) - calcHeight(root_->left_);
  // std::cout << "finalWeight=" << finalWeight << " size_" << size_ << std::endl;
  //}

  // else
  //{
  // throw std::logic_error("can't get balance");
  //}

  return newNode;
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree()
{
  root_ = nullptr;
  size_ = 0;
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree(size_t count, pair_t &pairKeyVal)
{
  root_ = nullptr;
  size_ = 0;
  for (size_t i = 0; i < count; ++i)
  {
    insert(pairKeyVal);
    size_++;
  }
}

template <typename Key, typename Value, typename Compare>
template <class InputIt>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree(InputIt start, InputIt stop)
{
  root_ = nullptr;
  size_ = 0;
  for (InputIt it = start; it != stop; ++it)
  {
    insert(*it);
    size_++;
  }
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree(std::initializer_list<pair_t> initAVL)
{
  root_ = nullptr;
  size_ = 0;
  for (auto it = initAVL.begin(); it != initAVL.end(); ++it)
  {
    insert(*it);
    size_++;
  }
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree(const AVLTree &copy)
{
  // root_ = nullptr;
  // size_ = 0;
  root_ = copyTree(copy.root_, nullptr);
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree(AVLTree &&move)
//  root_(nullptr), size_(std::move(move.size_))
{
  // root_ = nullptr;
  // size_ = 0;
  root_ = copyTree(std::move(move.root_), nullptr);
  move.clear();
  move.root_ = nullptr;
  move.size_ = 0;
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::~AVLTree()
{
  clear();
  root_ = nullptr;
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::insert(mihalchenko::AVLTree<Key, Value, Compare>::pair_t pair)
{
  insert(pair.first, pair.second);
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::insert(Key key, Value value)
{
  // Node * node = insertNode(key, value, root_, nullptr);
  /*  root_ = insertNode(key, value, root_, nullptr);
    updateHeight(root_);
    Node * overweight = isBalanced(find(key).constIter_.node_);
    doBalance(overweight);  //balancingTree
    return;
    */
  Node *node = insertNode(key, value, root_);
  if (node != nullptr)
  {
    size_t value1 = (node->left_) ? node->left_->height_ : 0;
    size_t value2 = (node->right_) ? node->right_->height_ : 0;
    node->height_ = std::max(value1, value2) + 1;
  }
  // Node * checkNode = SearchHiHeight(find(key).constIter_.node_);
  // doStableTree(checkNode);
  return;
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::doStableTree(Node *checkNode)
{
  while (checkNode)
  {
    if ((calcHeight(checkNode) == 2) && (calcHeight(checkNode->right_) >= 0))
    {
      leftSpin(checkNode);
    }
    else if (calcHeight(checkNode) == 2)
    {
      rightLeftSpin(checkNode);
    }
    else if (calcHeight(checkNode->left_) < 0)
    {
      rightSpin(checkNode);
    }
    else
    {
      leftRightSpin(checkNode);
    }
    checkNode = SearchHiHeight(checkNode);
  }
}

template <typename Key, typename Value, typename Compare>
size_t mihalchenko::AVLTree<Key, Value, Compare>::erase(const Key &key)
{
  if (find(key) == end()) // если удаляемый узел не найден
  {
    return 0;
  }
  Node *deletedNode = find(key).constIter_.node_;
  Node *prevDelNode = deletedNode->previous_;
  Node *workerNode = deletedNode;
  Node *firstRightNode = nullptr;
  // Node * firstLeftNode = nullptr;
  // std::cout << "мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << std::endl;
  if ((deletedNode->left_ == nullptr) && (deletedNode->right_ == nullptr)) // если удаляемый узел - лист
  {
    // std::cout << "1 мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << std::endl;
    if (prevDelNode)
    {
      // std::cout << "9 мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << " prevDelNode->right_=" << prevDelNode->right_ << " prevDelNode->left_=" << prevDelNode->left_ << std::endl;
      if (prevDelNode->left_)
      {
        (prevDelNode->left_ == deletedNode) ? prevDelNode->left_ = nullptr : prevDelNode->right_ = nullptr;
      }
      else
      {
        (prevDelNode->right_ == deletedNode) ? prevDelNode->right_ = nullptr : prevDelNode->left_ = nullptr;
      }

      // std::cout << "10 мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << " root_=" << root_ << " prevDelNode->right_=" << prevDelNode->right_ << " prevDelNode->left_=" << prevDelNode->left_ << std::endl;
      // delete deletedNode;
      // std::cout << "11 мы в erase. deletedNode=" << "deletedNode" << "prevDelNode=" << prevDelNode << " root_=" << root_ << " prevDelNode->right_=" << prevDelNode->right_ << " prevDelNode->left_=" << prevDelNode->left_ << std::endl;
    }
    else
    {
      // Node * temp = deletedNode;
      // delete deletedNode;
      root_ = nullptr;
      std::cout << "Дерево растворилось..." << std::endl;
    }
  }
  else if (!(deletedNode->left_ == nullptr) && !(deletedNode->right_ == nullptr)) // если есть оба наследника у узла
  {
    // std::cout << "2 мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << std::endl;
    // std::cout << "2 мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << " root_=" << root_ << " deletedNode->right_=" << deletedNode->right_ << " deletedNode->left_=" << deletedNode->left_ << std::endl;
    workerNode = deletedNode->right_;
    while (workerNode->left_)
    {
      workerNode = workerNode->left_;
    }
    firstRightNode = workerNode; // нашли узел с самым близким справа ключом к ключу удаляемого узла

    workerNode = deletedNode->left_;
    /*    while (workerNode->right_)
        {
           workerNode = workerNode->right_;
        }
        firstLeftNode = workerNode;  // нашли узел с самым близким слева ключом к ключу удаляемого узла
    */
    workerNode->previous_ = firstRightNode;
    firstRightNode->left_ = workerNode;

    if (deletedNode == root_)
    {
      root_ = firstRightNode;
      std::cout << "Дерево поменяло корень..." << std::endl;
    }
    // std::cout << "12 мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << std::endl;
    // delete deletedNode;
  }
  else // если один наследник у узла
  {
    // std::cout << "3 мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << std::endl;
    if (prevDelNode)
    {
      if (prevDelNode->left_ == deletedNode)
      {
        // std::cout << "4 мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << std::endl;
        if (!(deletedNode->left_ = nullptr))
        {
          deletedNode->left_->previous_ = prevDelNode;
          prevDelNode->left_ = deletedNode->left_;
          // delete deletedNode;
        }
        else
        {
          deletedNode->right_->previous_ = prevDelNode;
          prevDelNode->left_ = deletedNode->right_;
          // delete deletedNode;
        }
      }

      else // if (prevDelNode->right_ == deletedNode)`
      {
        // std::cout << "5 мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << std::endl;
        if (!(deletedNode->right_ = nullptr))
        {
          deletedNode->left_->previous_ = prevDelNode;
          prevDelNode->right_ = deletedNode->left_;
          // delete deletedNode;
        }
        else
        {
          deletedNode->right_->previous_ = prevDelNode;
          prevDelNode->right_ = deletedNode->right_;
          // delete deletedNode;
        }
      }
    }
    else
    {
      // std::cout << "6 мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << " root_=" << root_ << " deletedNode->right_=" << deletedNode->right_ << " deletedNode->left_=" << deletedNode->left_ << std::endl;
      // Node * temp = deletedNode;
      (deletedNode->right_) ? root_ = deletedNode->right_ : root_ = deletedNode->left_;
      // delete temp;
      // std::cout << "7 мы в erase. deletedNode=" << deletedNode << "prevDelNode=" << prevDelNode << " root_=" << root_ << " deletedNode->right_=" << deletedNode->right_ << " deletedNode->left_=" << deletedNode->left_ << std::endl;
      // prevDelNode = root_;
      // std::cout << "77 мы в erase. deletedNode=" << deletedNode << " root_=" << root_ << " root_->pairOfKeyVal_=" << prevDelNode->pairOfKeyVal_.first << std::endl;

      std::cout << "Хитрое дерево поменяло корень..." << std::endl;
    }
  }
  /*  if (root_ != nullptr)
   {
     size_t value1 = (root_->left_) ? root_->left_->height_ : 0;
     size_t value2 = (root_->right_) ? root_->right_->height_ : 0;
     root_->height_ = std::max(value1, value2) + 1;
   }*/
  if (prevDelNode)
  {
    // Node * checkNode = SearchHiHeight(prevDelNode);
    // doStableTree(checkNode);
  }
  size_--;
  return 0;
}

/*template< typename Key, typename Value, typename Compare >
void mihalchenko::AVLTree < Key, Value, Compare >::leftSpin(Node * node)
{
  if (node->previous_)
  {
    Node * previos = node->previous_;
    Node * newLeafOfTree;
    if (node == previos->right_)
    {
      previos->right_ = node->right_;
      newLeafOfTree = previos->right_;
    }
    else
    {
      previos->left_ = node->right_;
      newLeafOfTree = previos->left_;
    }
    newLeafOfTree->previous_ = previos;
    newLeafOfTree->left_ = node;
    node->previous_ = newLeafOfTree;
    node->right_ = node->right_->left_;
  }
  else
  {
    Node * wrem = node;
    root_ = node->right_;
    wrem->right_ = root_->right_->left_;
    root_->left_ = wrem;
    wrem->previous_ = root_;
    root_->previous_ = nullptr;
  }

  if (root_ != nullptr)
  {
    size_t value1 = (root_->left_) ? root_->left_->height_ : 0;
    size_t value2 = (root_->right_) ? root_->right_->height_ : 0;
    root_->height_ = std::max(value1, value2) + 1;
  }
  return;
}*/

/*template< typename Key, typename Value, typename Compare >
void mihalchenko::AVLTree < Key, Value, Compare >::rightSpin(Node * node)
{
  if (node->previous_)
  {
    Node * previos = node->previous_;
    Node * newLeafOfTree;
    if (previos->left_ == node)
    {
      previos->left_ = node->left_;
      newLeafOfTree = previos->left_;
    }
    else
    {
      previos->right_ = node->left_;
      newLeafOfTree = previos->right_;
    }
    newLeafOfTree->previous_ = previos;
    node->left_ = node->left_->right_;
    newLeafOfTree->right_ = node;
    node->previous_ = newLeafOfTree;
  }
  else
  {
    Node * wrem = node;
    root_ = node->left_;
    wrem->left_ = root_->left_->right_;
    root_->right_ = wrem;
    wrem->previous_ = root_;
    root_->previous_ = nullptr;
  }
  if (root_ != nullptr)
  {
    size_t value1 = (root_->left_) ? root_->left_->height_ : 0;
    size_t value2 = (root_->right_) ? root_->right_->height_ : 0;
    root_->height_ = std::max(value1, value2) + 1;
  }
}*/

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::rightLeftSpin(Node *node)
{
  rightSpin(node->right_);
  leftSpin(node);
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::leftRightSpin(Node *node)
{
  leftSpin(node->left_);
  rightSpin(node);
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator mihalchenko::AVLTree<Key, Value, Compare>::find(const Key &key)
{
  return findNode(key, root_);
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator mihalchenko::AVLTree<Key, Value, Compare>::find(const Key &key) const
{
  return findNode(key, root_);
}

template <typename Key, typename Value, typename Compare>
Value &mihalchenko::AVLTree<Key, Value, Compare>::at(const Key &key)
{
  return (*find(key)).second;
}

template <typename Key, typename Value, typename Compare>
const Value &mihalchenko::AVLTree<Key, Value, Compare>::at(const Key &key) const
{
  return (*find(key)).second;
}

template <typename Key, typename Value, typename Compare>
Value &mihalchenko::AVLTree<Key, Value, Compare>::operator[](const Key &key)
{
  try
  {
    find(key);
  }
  catch (const std::out_of_range &)
  {
    insert(key, root_);
    return (*find(key)).second;
  }
}

template <typename Key, typename Value, typename Compare>
const Value &mihalchenko::AVLTree<Key, Value, Compare>::operator[](const Key &key) const
{
  try
  {
    find(key);
  }
  catch (const std::out_of_range &)
  {
    return (*find(key)).second;
  }
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::clear()
{
  clear(root_);
}

template <typename Key, typename Value, typename Compare>
bool mihalchenko::AVLTree<Key, Value, Compare>::empty() const noexcept
{
  return root_ == nullptr;
}

template <typename Key, typename Value, typename Compare>
size_t mihalchenko::AVLTree<Key, Value, Compare>::getSize() const noexcept
{
  return size_;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator mihalchenko::AVLTree<Key, Value, Compare>::begin() noexcept
{
  if (empty())
  {
    return cend();
  }
  Node *temp = root_;
  while (temp->left_ != nullptr)
  {
    temp = temp->left_;
  }
  return Iterator(ConstIterator(temp, root_));
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator mihalchenko::AVLTree<Key, Value, Compare>::end() noexcept
{
  return ConstIterator(nullptr, root_);
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator mihalchenko::AVLTree<Key, Value, Compare>::cbegin() const noexcept
{
  if (empty())
  {
    return cend();
  }
  Node *temp = root_;
  while (temp->left != nullptr)
  {
    temp = temp->left;
  }
  return ConstIterator(temp, root_);
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator mihalchenko::AVLTree<Key, Value, Compare>::cend() const noexcept
{
  return ConstIterator(nullptr, root_);
}

/*template< typename Key, typename Value, typename Compare >
int mihalchenko::AVLTree < Key, Value, Compare >::calcHeight(Node * node)
{
  int height = 0;
  if ((node->right_) && (node->left_))
  {
    height = node->right_->height_ - node->left_->height_;
  }
  return height;
}*/

template <typename Key, typename Value, typename Compare>
int mihalchenko::AVLTree<Key, Value, Compare>::calcHeight(Node *node)
{
  if ((root_ == nullptr) || (node == nullptr))
  {
    return 0;
  }
  // std::cout << "ku..." << std::endl;
  int weightLeftNode = (node->left_) ? calcHeight(node->left_) : 0;
  int weightRightNode = (node->right_) ? calcHeight(node->right_) : 0;
  int temp = (weightRightNode > weightLeftNode) ? weightRightNode - weightLeftNode + 1 : weightLeftNode - weightRightNode + 1;
  node->height_ = temp;
  return temp;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node *mihalchenko::AVLTree<Key, Value, Compare>::SearchHiHeight(Node *node)
{
  Node *wrem = node;
  while (wrem)
  {
    if ((calcHeight(wrem) > 1) || (calcHeight(wrem) < -1))
    {
      return wrem;
    }
    wrem = wrem->previous_;
  }
  return nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------
template <typename Key, typename Value, typename Compare>
size_t mihalchenko::AVLTree<Key, Value, Compare>::updateHeight(Node *node)
{
  if (node == nullptr)
  {
    return 0;
  }
  return node->height_ = std::max(updateHeight(node->left_), updateHeight(node->right_)) + 1;
}

template <typename Key, typename Value, typename Compare>
int mihalchenko::AVLTree<Key, Value, Compare>::checkBalance(Node *node)
{
  return getHeight(node->right_) - getHeight(node->left_);
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node *mihalchenko::AVLTree<Key, Value, Compare>::isBalanced(Node *node)
{
  Node *temp = node;
  while (temp)
  {
    if (std::abs(checkBalance(temp)) > 1)
    {
      return temp;
    }
    temp = temp->previous_;
  }
  return nullptr;
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::balancingTree(Node *node)
{
  int finalWeight = (root_) ? calcHeight(root_->right_) - calcHeight(root_->left_) : 0;

  /*std::cout << "finalWeight=" << finalWeight << "nodeHeight=" << node->height_ << " ";

  if (node->left_)
  {
    std::cout << "node->left_->height_=" << node->left_->height_  << " ";
  }
  if (node->right_)
  {
    std::cout << "node->right_->height_=" << node->right_->height_;
  }
  std::cout << std::endl;*/
  return;
  if (finalWeight >= 2) // если вес корня дерева больше или равен 2, то надо балансировать
  {
    if ((node->right_) && (node->left_)) // если у корня две ветки
    {
      if (node->right_->height_ - node->left_->height_ >= 2) // условие правого поворота, т.е. корень вправо смещаем
      {

        leftSpin(node);
      }
      else if ((node->height_ < -1) && (node->left_->height_) <= 0)
      {
        rightSpin(node);
      }
      else if ((node->height_ > 1) && (node->right_->height_) < 0)
      {
        rightSpin(node->right_);
        leftSpin(node);
      }
      else if ((node->height_ < -1) && (node->left_->height_) > 0)
      {
        // node->left_ = rightSpin(node->left_);
        rightSpin(node->left_);
        rightSpin(node);
      }
    }
  }
  return;
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::leftSpin(Node *root)
{
  /* Node* rightSubtree = root->right_;
   Node* rightSubtreeLeftSubtree = rightSubtree->left_;
   rightSubtree->left_ = root;
   root->right_ = rightSubtreeLeftSubtree;
   root->parent_ = rightSubtree;
   if (rightSubtreeLeftSubtree)
   {
     rightSubtreeLeftSubtree->parent_ = root;
   }
   updateHeight(root);
   updateHeight(rightSubtree);*/
  return;
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::rightSpin(Node *root)
{
  /*  Node* leftSubtree = root->left_;
    Node* leftSubtreeRightSubtree = leftSubtree->right_;
    leftSubtree->right_ = root;
    root->left_ = leftSubtreeRightSubtree;
    root->parent_ = leftSubtree;
    if (leftSubtreeRightSubtree)
    {
      leftSubtreeRightSubtree->parent_ = root;
    }
    updateHeight(root);
    updateHeight(leftSubtree);
    return;*/
}

template <typename Key, typename Value, typename Compare>
int mihalchenko::AVLTree<Key, Value, Compare>::getHeight(Node *node)
{
  return (node) ? node->height_ : 0;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node *mihalchenko::AVLTree<Key, Value, Compare>::getRoot()
{
  Node *temp = this;
  while (temp->previous_ != root_)
  {
    temp = temp->previous_;
  }
  return temp->previous_;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node const *mihalchenko::AVLTree<Key, Value, Compare>::getRoot() const
{
  Node *temp = this;
  while (temp->previous_ != root_)
  {
    temp = temp->previous_;
  }
  return temp->previous_;
}
//-------------------------------------------------------------------------------------------------------------------------

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node *mihalchenko::AVLTree<Key, Value, Compare>::copyTree(Node *node, Node *previous)
{
  if (node == nullptr)
  {
    return nullptr;
  }
  Node *newnode = new Node(node->pairOfKeyVal_.first, node->pairOfKeyVal_.second, node->height_, nullptr, nullptr, previous);
  newnode->left_ = copyTree(node->left_, newnode);
  newnode->right_ = copyTree(node->right_, newnode);
  return newnode;
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::ConstIterator()
{
  node_ = nullptr;
  root_ = nullptr;
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::ConstIterator(Node *node, Node *root)
{
  node_ = node;
  root_ = root;
}

template <typename Key, typename Value, typename Compare> //
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator &mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator++()
{
  if (node_->right_)
  {
    node_ = node_->right_;
    while (node_->left_)
    {
      node_ = node_->left_;
    }
  }
  else
  {
    Node *tempNode = node_->previous_;
    while (tempNode && node_ == tempNode->right_)
    {
      node_ = tempNode;
      tempNode = node_->previous_;
    }
    node_ = tempNode;
  }
  return *this;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator++(int)
{
  ConstIterator result(*this);
  ++(*this);
  return result;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator &mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator--()
{
  if (node_ == nullptr)
  {
    node_ = root_;
    while (node_->right_)
    {
      node_ = node_->right_;
    }
  }
  else
  {
    if (node_->left_)
    {
      node_ = node_->left_;
      while (node_->right_)
      {
        node_ = node_->right_;
      }
    }
    else
    {
      Node *tempNode = node_->previous_;
      while (tempNode && node_ == tempNode->left_)
      {
        node_ = tempNode;
        tempNode = node_->previous_;
      }
      node_ = tempNode;
    }
  }
  return *this;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator--(int)
{
  ConstIterator result(*this);
  --(*this);
  return result;
}

template <typename Key, typename Value, typename Compare>
const typename mihalchenko::AVLTree<Key, Value, Compare>::pair_t &mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator*() const
{
  return node_->pairOfKeyVal_;
}

template <typename Key, typename Value, typename Compare>
const typename mihalchenko::AVLTree<Key, Value, Compare>::pair_t *mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator->() const
{
  return std::addressof(node_->pairOfKeyVal_);
}

template <typename Key, typename Value, typename Compare>
bool mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator==(const ConstIterator &rhs) const
{
  return node_ == rhs.node_;
}

template <typename Key, typename Value, typename Compare>
bool mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator!=(const ConstIterator &rhs) const
{
  return !(*this == rhs);
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::Iterator::Iterator() : constIter_(ConstIterator())
// constIter_(value) - было
{
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::Iterator::Iterator(ConstIterator iter) : constIter_(iter)
{
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator &mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator++()
//::Iterator::this_t &
{
  ++constIter_;
  return *this;
}

template <typename Key, typename Value, typename Compare>
// typename mihalchenko::AVLTree < Key, Value, Compare >::Iterator::this_t mihalchenko::Tree< Key, Value, Compare >::Iterator::operator++(int)
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator++(int)
{
  Iterator result = constIter_;
  // this_t result = iter_;
  ++constIter_;
  return result;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator &mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator--()
{
  --constIter_;
  return *this;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator--(int)
{
  Iterator result = constIter_;
  --constIter_;
  return result;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::pair_t &mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator*()
{
  return constIter_.node_->pairOfKeyVal_;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::pair_t *mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator->()
{
  return std::addressof(constIter_.node_->pairOfKeyVal_);
}

template <typename Key, typename Value, typename Compare>
bool mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator!=(const Iterator &rhs) const
{
  return !(rhs == *this);
  // return !(rhs.constIter_ == constIter_);
}

template <typename Key, typename Value, typename Compare>
bool mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator==(const Iterator &rhs) const
{
  return constIter_ == rhs.constIter_;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator mihalchenko::AVLTree<Key, Value, Compare>::findNode(const Key &key, Node *node)
{
  Compare compFunc;
  // if (empty())
  if (node == nullptr)
  {
    return Iterator(ConstIterator(nullptr, root_));
  }
  if (node->pairOfKeyVal_.first == key)
  {
    return Iterator(ConstIterator(node, root_));
  }
  else if (compFunc(key, node->pairOfKeyVal_.first))
  {
    return findNode(key, node->left_);
  }
  else if (!compFunc(key, node->pairOfKeyVal_.first))
  {
    return findNode(key, node->right_);
  }
  else
  {
    throw std::out_of_range("Such element not exist");
  }
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator mihalchenko::AVLTree<Key, Value, Compare>::findNode(const Key &key, Node *node) const
{
  Compare compFunc;
  // if (empty())
  if (node == nullptr)
  {
    return ConstIterator(nullptr, root_);
  }
  if (node->pairOfKeyVal_.first == key)
  {
    return ConstIterator(node, root_);
  }
  else if (compFunc(key, node->pairOfKeyVal_.first))
  {
    return findNode(key, node->left_);
  }
  else if (!compFunc(key, node->pairOfKeyVal_.first))
  {
    return findNode(key, node->right_);
  }
  else
  {
    throw std::out_of_range("Such element not exist");
  }
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::clear(Node *node)
{
  if (node == nullptr)
  {
    return;
  }
  clear(node->left_);
  clear(node->right_);
  delete node;
}

#endif
