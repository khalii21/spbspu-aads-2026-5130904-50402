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
    void clear();
    bool empty();

    private:
      TreeNode< T, Cmp > *root;
      Cmp cmp;
  };
}

#endif
