//
// Created by Dustin Cobas Batista on 02-Nov-17.
//

#include "gtest/gtest.h"

#include "string-dictionaries/HashTableWithLinearProbing.h"
#include "string-dictionaries/BlindTree.h"
#include "string-dictionaries/TernaryTree.h"


template<typename T>
class DictionaryTC : public ::testing::Test {
};

typedef ::testing::Types<HashTableWithLinearProbing<int, 10>, BlindTree<int>, TernaryTree<int>> MyTypes;
TYPED_TEST_CASE(DictionaryTC, MyTypes);

TYPED_TEST(DictionaryTC, add) {
  TypeParam map;
  EXPECT_EQ(0, map.size());

  EXPECT_TRUE(map.insert("a", 17));
  EXPECT_EQ(17, map.search("a"));
  EXPECT_EQ(17, map["a"]);
  EXPECT_EQ(1, map.size());

  EXPECT_TRUE(map.insert("b", 13));
  EXPECT_EQ(13, map.search("b"));
  EXPECT_EQ(13, map["b"]);
  EXPECT_EQ(2, map.size());

  EXPECT_FALSE(map.insert("a", 7));
  EXPECT_EQ(17, map["a"]);
  EXPECT_EQ(2, map.size());

  EXPECT_TRUE(map.insert("xxx", 7));
  EXPECT_EQ(7, map.search("xxx"));
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

TEST(HashTableWithLinearProbing, asMap) {
  HashTableWithLinearProbing<int, 2> map;
  EXPECT_EQ(0, map["x"]);
  EXPECT_EQ(0, map["y"]);

  map["x"] = 10;
  EXPECT_EQ(10, map["x"]);

  EXPECT_ANY_THROW(map["z"]);
}
