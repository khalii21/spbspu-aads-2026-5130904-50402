#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

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
    private:
      struct Slot {
        Key key;
        Value value;
				SlotState state = SlotState::EMPTY;
      };
      size_t size_;
      size_t cap_;
      Slot* slots_;
  };
}
