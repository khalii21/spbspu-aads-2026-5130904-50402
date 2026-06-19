#ifndef RBCIT_HPP
#define RBCIT_HPP

#include "TreeNode.hpp"
#include "RBIt.hpp"

namespace khalikov {
  template< class T, class Cmp >
  struct RBCIt
  {
    RBCIt();
    ~RBCIt();
    explicit RBCIt(TreeNode< T, Cmp > *node);
    explicit RBCIt(const RBIt< T, Cmp > &other);

    const T &operator*() const;
    const T *operator->() const;

    bool operator==(const RBCIt &rhs) const noexcept;
    bool operator!=(const RBCIt &rhs) const noexcept;

    const RBCIt &operator++();
    const RBCIt operator++(int);

    private:
      TreeNode< T, Cmp > *curr;
  };
}

#endif
