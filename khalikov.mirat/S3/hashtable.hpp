#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <htiter.hpp>
#include <htciter.hpp>

namespace khalikov
{
  enum class SlotState {
    EMPTY,
    OCCUPIED,
    TOMBSTONE
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
    public:
      HashTable();
      HashTable(const HashTable& rhs);
      HashTable(HashTable&& rhs) noexcept;
      ~HashTable();

      HashTable& operator=(const HashTable& rhs);
      HashTable& operator=(HashTable&& rhs) noexcept;
      Value& operator[](const Key& key);

      void swap(HashTable& rhs) noexcept;

    private:
      void rehash(const size_t new_cap);
      struct Slot {
        Key key;
        Value value;
				SlotState state = SlotState::EMPTY;
      };
      size_t size_;
      size_t cap_;
      Slot* slots_;
      Hash hasher;
  };
}

template< class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal >::HashTable() :
  size_(0),
  cap_(8),
  slots_(new Slot[8])
{}

template< class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal >::~HashTable() {
  delete[] slots_;
}

template< class Key, class Value, class Hash, class Equal >
void khalikov::HashTable< Key, Value, Hash, Equal >::swap(HashTable& rhs) noexcept {
  std::swap(size_, rhs.size_);
  std::swap(cap_, rhs.cap_);
  std::swap(slots_, rhs.slots_);
  std::swap(hasher, rhs.hasher);
}

template< class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& rhs) :
  size_(0),
  cap_(rhs.cap_),
  slots_(new Slot[rhs.cap_])
{
  for (size_t i = 0; i < rhs.cap_; ++i) {
    slots_[i].state = rhs.slots_[i].state;
    if (rhs.slots_[i].state == SlotState::OCCUPIED) {
      slots_[i].key = rhs.slots_[i].key;
      slots_[i].value = rhs.slots_[i].value;
      size_++;
    }
  }
}

template< class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& rhs) noexcept :
  size_(0),
  cap_(0),
  slots_(nullptr)
{
  this->swap(rhs);
}

template< class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal >&
khalikov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& rhs) {
  if (this != std::addressof(rhs)) {
    HashTable temp(rhs);
    this->swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal >&
khalikov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& rhs) noexcept {
   this->swap(rhs);
   return *this;
}
