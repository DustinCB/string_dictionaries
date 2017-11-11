//
// Created by Dustin Cobas Batista on 02-Nov-17.
//

#include "gtest/gtest.h"

#include "string-dictionaries/HashTableWithLinearProbing.h"
#include "string-dictionaries/BlindTree.h"
#include "string-dictionaries/TernaryTree.h"

#include "string-dictionaries/hash_function.h"

template<typename T>
class DictionaryTC : public ::testing::Test {
};

typedef ::testing::Types<HashTableWithLinearProbing<int>, BlindTree<int>, TernaryTree<int>> DictionaryTypes;
TYPED_TEST_CASE(DictionaryTC, DictionaryTypes);

TYPED_TEST(DictionaryTC, add) {
  TypeParam map;
  EXPECT_EQ(0, map.size());

  EXPECT_TRUE(map.insert("a", 17));
  EXPECT_TRUE(map.search("a"));
  EXPECT_EQ(17, map["a"]);
  EXPECT_EQ(1, map.size());

  EXPECT_TRUE(map.insert("b", 13));
  EXPECT_TRUE(map.search("b"));
  EXPECT_EQ(13, map["b"]);
  EXPECT_EQ(2, map.size());

  EXPECT_FALSE(map.insert("a", 7));
  EXPECT_EQ(17, map["a"]);
  EXPECT_EQ(2, map.size());

  EXPECT_TRUE(map.insert("xxx", 7));
  EXPECT_TRUE(map.search("xxx"));
  EXPECT_EQ(7, map["xxx"]);
  EXPECT_EQ(3, map.size());
}

TYPED_TEST(DictionaryTC, iterator) {
  TypeParam map;

  std::set<std::pair<std::string, int>> eSet = {{"a", 2}, {"b", 3}, {"c", 5}, {"d", 7}, {"xxx", 11}};
  for (auto &&item : eSet) {
    map[item.first] = item.second;
  }

  std::set<std::pair<std::string, int>> rSet;
  for (auto &&item : map) {
    rSet.insert(item);
  }

  EXPECT_EQ(eSet, rSet);
}

TYPED_TEST(DictionaryTC, search) {
  TypeParam map;

  std::vector<std::string> eText = {"this", "is", "a", "simple", "text", "for", "a", "simple", "test"};
  for (auto &&item : eText) {
    ++map[item];
  }
  EXPECT_GT(eText.size(), map.size());
  EXPECT_EQ(eText.size() - 2, map.size());

  int rC = 0;
  for (auto &&item : map) {
    rC += item.second;
  }
  EXPECT_EQ(eText.size(), rC);
}



/*
 * Hash Function Test
 */
template<typename T>
class HashFunctionTC : public ::testing::Test {
};

typedef ::testing::Types<std::hash<std::string>, HashDjb2, HashSdbm, HashHorner> HashTypes;
TYPED_TEST_CASE(HashFunctionTC, HashTypes);

TYPED_TEST(HashFunctionTC, string) {
  TypeParam hash;

  std::cout << "\thash(\"x\") = " << hash("x") << std::endl;
  std::cout << "\thash(\"foo\") = " << hash("foo") << std::endl;

  EXPECT_EQ(hash("foo"), hash("foo"));
  EXPECT_EQ(hash("bar"), hash("bar"));

  EXPECT_NE(hash("foo"), hash("foofoo"));
  EXPECT_NE(hash("foo"), hash("bar"));
  EXPECT_NE(hash("foobar"), hash("barfoo"));
  EXPECT_NE(hash("bar"), hash("x"));
  EXPECT_NE(hash(""), hash(" "));
}


/*
 * Hash Table Test
 */
TEST(HashTableWithLinearProbing, constructor) {
  HashTableWithLinearProbing<int> map1{2};
  EXPECT_EQ(2, map1.capacity());

  HashTableWithLinearProbing<int> map2{100};
  EXPECT_EQ(100, map2.capacity());

  HashTableWithLinearProbing<int> map3{20};
  EXPECT_EQ(20, map3.capacity());
}

TEST(HashTableWithLinearProbing, asMap) {
  HashTableWithLinearProbing<int> map{2};
  EXPECT_EQ(0, map["x"]);
  EXPECT_EQ(0, map["y"]);

  map["x"] = 10;
  EXPECT_EQ(10, map["x"]);

  EXPECT_ANY_THROW(map["z"]);
}
