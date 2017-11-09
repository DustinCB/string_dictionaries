//
// Created by Dustin Cobas Batista on 02-Nov-17.
//

#ifndef STRING_DICTIONARIES_HASHTABLELINEARPROBING_H
#define STRING_DICTIONARIES_HASHTABLELINEARPROBING_H

#include <utility>
#include <vector>

template<typename _Value, std::size_t _Size = 50000>
class HashTableWithLinearProbing {
 public:

  HashTableWithLinearProbing(){}
  HashTableWithLinearProbing(std::size_t _size): SIZE{_size} {}

  using _Key = std::string;

  bool insert(const _Key &_key, const _Value &_value) {
    if (currentSize == SIZE)
      return false;

    auto i = hash(_key) % SIZE;
    while (table[i].first && table[i].second.first != _key) {
      i = (i + 1) % SIZE;
    }

    if (table[i].first)
      return false;

    table[i].second.second = _value;
    if (!table[i].first) {
      table[i].second.first = _key;
      table[i].first = true;
      currentSize++;
    }

    return true;
  }

  //const _Value &search(const _Key &_key) const {
  bool search(const _Key &_key) const {
    auto i = hash(_key) % SIZE;
    while (table[i].first && table[i].second.first != _key) {
      i = (i + 1) % SIZE;
    }

    if (!table[i].first || table[i].second.first != _key)
      return false;//throw std::exception{};

    return true;//return table[i].second.second;
  }

  _Value &operator[](const _Key &_key) {
    auto s = SIZE + 1;
    auto i = hash(_key) % SIZE;
    while (--s && table[i].first && table[i].second.first != _key) {
      i = (i + 1) % SIZE;
    }

    if (!s)
      throw std::exception{};

    if (!table[i].first) {
      table[i].second.first = _key;
      table[i].first = true;
      currentSize++;
    }

    return table[i].second.second;
  }

  template<typename _O>
  void traverse(_O &_o) {
    for (auto &&item : table) {
      if (item.first) {
        _o(item.second);
      }
    }
  }

  std::size_t size() const {
    return currentSize;
  }

  std::size_t capacity() const {
    return table.size();
  }

 private:
  using InnerContainer = std::vector<std::pair<bool, std::pair<_Key, _Value>>>;

 public:
  class Iterator {
   public:
    Iterator(const typename InnerContainer::iterator &_it,
             const typename InnerContainer::iterator &_eit) : it{_it},
                                                              eit{_eit} {}

    Iterator(const Iterator &_it) : it{_it.it}, eit{_it.eit} {}

    std::pair<_Key, _Value> &operator*() { return it->second; }

    Iterator &operator++() {
      while (it != eit && !(++it)->first) {}
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp(*this);
      operator++();
      return tmp;
    }

    bool operator==(const Iterator &rhs) const { return it == rhs.it; }

    bool operator!=(const Iterator &rhs) const { return it != rhs.it; }

   private:
    typename InnerContainer::iterator it;
    typename InnerContainer::iterator eit;
  };

  Iterator begin() {
    if (table.empty())
      return Iterator{table.end(), table.end()};

    auto it = table.begin();
    while (!it->first) {
      ++it;
    }

    return Iterator{it, table.end()};
  }

  Iterator end() {
    return Iterator{table.end(), table.end()};
  }

  size_t size_of() {
    size_t size = 0;

    for (int i = 0; i < table.size(); ++i) {
      size += sizeof(table[i]);
    }

    return size + sizeof(hash) + sizeof(currentSize);
  }

 private:
  const std::size_t SIZE = _Size;
  InnerContainer table{SIZE};
  std::hash<_Key> hash;
  std::size_t currentSize = 0;
};

#endif //STRING_DICTIONARIES_HASHTABLELINEARPROBING_H
