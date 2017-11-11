//
// Created by Dustin Cobas Batista on 06-11-17.
//

#include "string-dictionaries/hash_function.h"


std::size_t HashDjb2::operator()(const std::string &__s) const noexcept {
  std::size_t hash = 5381;

  for (auto &&c : __s) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash;
}


std::size_t HashSdbm::operator()(const std::string &__s) const noexcept {
  std::size_t hash = 0;

  for (auto &&c : __s) {
    hash = c + (hash << 6) + (hash << 16) - hash;
  }

  return hash;
}


std::size_t HashHorner::operator()(const std::string &__s) const noexcept {
  std::size_t hash = 0;

  for (auto &&c : __s) {
    hash = (hash << 4) + c;

    long g = hash & 0xF0000000L;
    if (g != 0)
      hash ^= g >> 24;

    hash &= ~g;
  }

  return hash;
}
