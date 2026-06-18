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
    bool insert(const T &val);

    private:
      TreeNode< T, Cmp > *root;
      Cmp cmp;
      TreeNode< T, Cmp > *copy(TreeNode< T, Cmp > *node, TreeNode< T, Cmp > *parent);
      void rotateLeft(TreeNode< T, Cmp > *x);
      void rotateRight(TreeNode< T, Cmp > *x);
      void fixInsert(TreeNode< T, Cmp > *x);
  };
}

template< class T, class Cmp >
bool khalikov::RBTree< T, Cmp >::insert(const T &val)
{
  TreeNode< T, Cmp > *z = new TreeNode< T, Cmp >(val);
  TreeNode< T, Cmp > *y = nullptr;
  TreeNode< T, Cmp > *x = root;
  while (x) {
    y = x;
    if (!cmp(z->data, x->data) && !cmp(x->data, z->data)) {
      delete z;
      return false;
    }
    if (cmp(z->data, x->data)) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  z->parent = y;
  if (!y) {
    root = z;
  } else if (cmp(z->data, y->data)) {
    y->left = z;
  } else {
    y->right = z;
  }
  fixInsert(z);
  return true;
}

template< class T, class Cmp >
void khalikov::RBTree< T, Cmp >::fixInsert(TreeNode< T, Cmp > *x)
{
  while (x != root && x->parent->color == 'R') {
    TreeNode< T, Cmp > *p = x->parent;
    TreeNode< T, Cmp > *g = p->parent;
    if (p == g->left) {
      TreeNode< T, Cmp > *u = g->right;
      if (u && u->color == 'R') {
        p->color = 'B';
        u->color = 'B';
        g->color = 'R';
        x = g;
      }
      else {
        if (x == p->right) {
          x = p;
          rotateLeft(x);
          p = x->parent;
        }
        p->color = 'B';
        g->color = 'R';
        rotateRight(g);
      }
    }
    else {
      TreeNode< T, Cmp > *u = g->left;
      if (u && u->color == 'R') {
        p->color = 'B';
        u->color = 'B';
        g->color = 'R';
        x = g;
      }
      else {
        if (x == p->left) {
          x = p;
          rotateRight(x);
          p = x->parent;
        }
        p->color = 'B';
        g->color = 'R';
        rotateLeft(g);
      }
    }
  }
  root->color = 'B';
}

template< class T, class Cmp >
void khalikov::RBTree< T, Cmp >::rotateRight(TreeNode< T, Cmp > *x)
{
  TreeNode< T, Cmp > *y = x->left;
  x->left = y->right;
  if (y->right) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (!x->parent) {
    root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->right = x;
  x->parent = y;
}

template< class T, class Cmp >
void khalikov::RBTree< T, Cmp >::rotateLeft(TreeNode< T, Cmp > *x)
{
  TreeNode< T, Cmp > *y = x->right;
  x->right = y->left;
  if (y->left) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (!x->parent) {
    root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
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
