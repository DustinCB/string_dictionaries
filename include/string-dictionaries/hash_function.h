//
// Created by Dustin Cobas Batista on 06-11-17.
//

#ifndef STRING_DICTIONARIES_HASH_FUNCTION_H
#define STRING_DICTIONARIES_HASH_FUNCTION_H

#include <string>


/*
 * This algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c.
 * another version of this algorithm (now favored by bernstein) uses xor: hash(i) = hash(i - 1) * 33 ^ str[i];
 * the magic of number 33 (why it works better than many other constants, prime or not) has never been adequately explained.
 */
struct HashDjb2 : public std::__hash_base<size_t, std::string> {
  std::size_t operator()(const std::string &__s) const noexcept;
};


/*
 * This algorithm was created for sdbm (a public-domain reimplementation of ndbm) database library.
 * It was found to do well in scrambling bits, causing better distribution of the keys and fewer splits.
 * It also happens to be a good general hashing function with good distribution.
 * The actual function is hash(i) = hash(i - 1) * 65599 + str[i]; what is included below is the faster version used in gawk.
 * [there is even a faster, duff-device version] the magic constant 65599 was picked out of thin air while experimenting
 * with different constants, and turns out to be a prime. this is one of the algorithms used in berkeley db
 * (see sleepycat) and elsewhere.
 */
struct HashSdbm : public std::__hash_base<size_t, std::string> {
  std::size_t operator()(const std::string &__s) const noexcept;
};


/*
 * Uses Horner’s rule to compute a polynomial in x =16, using all the characters, plus doing some additional manipulations
 */
struct HashHorner : public std::__hash_base<size_t, std::string> {

  std::size_t operator()(const std::string &__s) const noexcept;
};

#endif //STRING_DICTIONARIES_HASH_FUNCTION_H
