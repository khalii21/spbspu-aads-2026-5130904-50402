#ifndef RBIT_HPP
#define RBIT_HPP

#include "TreeNode.hpp"

namespace khalikov {
  template< class T, class Cmp >
  struct RBIt
  {
    RBIt();
    ~RBIt();
    explicit RBIt(TreeNode< T, Cmp > *node);

    T &operator*();
    T *operator->();

    bool operator==(const RBIt &rhs) const noexcept;
    bool operator!=(const RBIt &rhs) const noexcept;

    RBIt &operator++();
    RBIt operator++(int);

    private:
      TreeNode< T, Cmp > *curr;
  };
}



#endif
