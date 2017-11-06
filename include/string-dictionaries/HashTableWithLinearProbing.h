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

  void insert(const _Key &_key, const _Value &_value) {
    //TODO Add case for full table
    if (size == _Size)
      throw std::exception{};

    auto i = hash(_key) % _Size;
    while (table[i].first && table[i].second.first != _key) {
      i = (i + 1) % _Size;
    }

    table[i].second.second = _value;
    if (!table[i].first) {
      table[i].second.first = _key;
      table[i].first = true;
      size++;
    }
  }

  _Value &search(const _Key &_key) {
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
      size++;
    }

    return table[i].second.second;
  }

 private:
  using InnerContainer = std::array<std::pair<bool, std::pair<_Key, _Value>>, _Size>;

 public:
  class iterator {
   public:
    iterator(const typename InnerContainer::iterator &_it, const typename InnerContainer::iterator &_eit) : it{_it},
                                                                                                            eit{_eit} {}
    iterator(const iterator &_it) : it{_it.it}, eit{_it.eit} {}

    std::pair<_Key, _Value> &operator*() {
      return it->second;
    }

    iterator &operator++() {
      while (it != eit && !(++it)->first) {}
      return *this;
    }

    iterator operator++(int) {
      iterator tmp(*this);
      operator++();
      return tmp;
    }

    bool operator==(const iterator &rhs) const { return it == rhs.it; }

    bool operator!=(const iterator &rhs) const { return it != rhs.it; }

   private:
    typename InnerContainer::iterator it;
    typename InnerContainer::iterator eit;
  };

  iterator begin() {
    if (table.empty())
      return iterator{table.end(), table.end()};

    auto it = table.begin();
    while (!it->first) {
      ++it;
    }

    return iterator{it, table.end()};
  }

  iterator end() {
    return iterator{table.end(), table.end()};
  }

 private:
  InnerContainer table;
  std::hash<_Key> hash;
  std::size_t size = 0;
};

#endif //STRING_DICTIONARIES_HASHTABLELINEARPROBING_H
