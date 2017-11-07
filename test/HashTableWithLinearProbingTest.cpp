//
// Created by Dustin Cobas Batista on 02-Nov-17.
//

#include "gtest/gtest.h"

#include "string-dictionaries/HashTableWithLinearProbing.h"

TEST(HashTableWithLinearProbing, add) {
  HashTableWithLinearProbing<int, 10> map;
  EXPECT_EQ(0, map.size());

  EXPECT_TRUE(map.insert("a", 17));
  EXPECT_EQ(17, map.search("a"));
  EXPECT_EQ(1, map.size());

  EXPECT_TRUE(map.insert("b", 13));
  EXPECT_EQ(13, map.search("b"));
  EXPECT_EQ(2, map.size());

  EXPECT_FALSE(map.insert("a", 7));
  EXPECT_EQ(2, map.size());

  EXPECT_TRUE(map.insert("xxx", 7));
  EXPECT_EQ(7, map.search("xxx"));
  EXPECT_EQ(3, map.size());
}

TEST(HashTableWithLinearProbing, asMap) {
  HashTableWithLinearProbing<int, 2> map;
  EXPECT_EQ(0, map["x"]);
  EXPECT_EQ(0, map["y"]);

  map["x"] = 10;
  EXPECT_EQ(10, map["x"]);

  EXPECT_ANY_THROW(map["z"]);
}

TEST(HashTableWithLinearProbing, iterator) {
  std::set<std::pair<std::string, int>> eSet = {{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"xxx", 13}};

  HashTableWithLinearProbing<int, 10> map;
  for (auto &&item : eSet) {
    map[item.first] = item.second;
  }

  std::set<std::pair<std::string, int>> rSet;
  for (auto &&item : map) {
    rSet.insert(item);
  }

  EXPECT_EQ(eSet, rSet);
}
