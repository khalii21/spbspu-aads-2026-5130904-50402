#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <utility>
#include "TreeNode.hpp"

namespace khalikov {
  template< class T, class Cmp >
  struct RBTree
  {
    RBTree();
    ~RBTree();
    RBTree(const RBTree &rhs);
    RBTree(RBTree &&rhs) noexcept;
    RBTree &operator=(const RBTree &rhs);
    RBTree &operator=(RBTree &&rhs) noexcept;

    void swap(RBTree &rhs) noexcept;
    void clear(TreeNode< T, Cmp > *node);
    bool empty() const noexcept;
    TreeNode< T, Cmp > *copy(TreeNode< T, Cmp > *node, TreeNode< T, Cmp > *parent);

    private:
      TreeNode< T, Cmp > *root;
      Cmp cmp;
  };
}

template< class T, class Cmp >
khalikov::RBTree< T, Cmp >::RBTree() :
  root(nullptr),
  cmp()
{}

template< class T, class Cmp >
khalikov::RBTree< T, Cmp >::~RBTree()
{
  clear(root);
  root = nullptr;
}

template< class T, class Cmp >
khalikov::RBTree< T, Cmp >::RBTree(const RBTree &rhs):
  root(nullptr),
  cmp(rhs.cmp)
{
  root = copy(rhs.root, nullptr);
}

template< class T, class Cmp >
khalikov::RBTree< T, Cmp >::RBTree(RBTree &&rhs) noexcept :
  root(nullptr),
  cmp(std::move(rhs.cmp))
{
  swap(rhs);
}

template< class T, class Cmp >
khalikov::RBTree< T, Cmp >& khalikov::RBTree< T, Cmp >::operator=(const RBTree &rhs)
{
  if (this != std::addressof(rhs)) {
    RBTree temp(rhs);
    swap(temp);
  }
  return *this;
}

template< class T, class Cmp >
khalikov::RBTree< T, Cmp >& khalikov::RBTree< T, Cmp >::operator=(RBTree &&rhs) noexcept
{
  if (this != std::addressof(rhs)) {
    clear(root);
    root = nullptr;
    swap(rhs);
  }
  return *this;
}

template< class T, class Cmp >
void khalikov::RBTree< T, Cmp >::clear(TreeNode< T, Cmp > *node)
{
  if (!node) {
    return;
  }
  clear(node->left);
  clear(node->right);
  delete node;
}

template< class T, class Cmp >
bool khalikov::RBTree< T, Cmp >::empty() const noexcept
{
  return root == nullptr;
}

template< class T, class Cmp >
khalikov::TreeNode< T, Cmp >* khalikov::RBTree< T, Cmp >::copy(TreeNode< T, Cmp > *node, TreeNode< T, Cmp > *parent) {
  if (!node) {
    return nullptr;
  }
  TreeNode< T, Cmp > *newNode = new TreeNode< T, Cmp >(node->data);
  newNode->parent = parent;
  newNode->color = node->color;
  newNode->left = copy(node->left, newNode);
  newNode->right = copy(node->right, newNode);
  return newNode;
}

template< class T, class Cmp >
void khalikov::RBTree< T, Cmp >::swap(RBTree &rhs) noexcept
{
  std::swap(root, rhs.root);
  std::swap(cmp, rhs.cmp);
}


#endif
