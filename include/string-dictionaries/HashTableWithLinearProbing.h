//
// Created by Dustin Cobas Batista on 02-Nov-17.
//

#ifndef STRING_DICTIONARIES_HASHTABLELINEARPROBING_H
#define STRING_DICTIONARIES_HASHTABLELINEARPROBING_H

#include <utility>
#include <array>
#include <iostream>

template<typename _Value, std::size_t _Size>
class HashTableWithLinearProbing {
 public:
  using _Key = std::string;

  bool insert(const _Key &_key, const _Value &_value) {
    if (currentSize == _Size)
      return false;

    auto i = hash(_key) % _Size;
    while (table[i].first && table[i].second.first != _key) {
      i = (i + 1) % _Size;
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

  const _Value &search(const _Key &_key) const {
    auto i = hash(_key) % _Size;
    while (table[i].first && table[i].second.first != _key) {
      i = (i + 1) % _Size;
    }

    if (!table[i].first || table[i].second.first != _key)
      throw std::exception{};

    return table[i].second.second;
  }

  _Value &operator[](const _Key &_key) {
    auto s = _Size + 1;
    auto i = hash(_key) % _Size;
    while (--s && table[i].first && table[i].second.first != _key) {
      i = (i + 1) % _Size;
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

  std::size_t size() const {
    return currentSize;
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

 private:
  InnerContainer table{_Size};
  std::hash<_Key> hash;
  std::size_t currentSize = 0;
};

#endif //STRING_DICTIONARIES_HASHTABLELINEARPROBING_H
