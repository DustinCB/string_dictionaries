//
// Created by Dustin Cobas Batista on 02-Nov-17.
//

#include "gtest/gtest.h"

#include "string-dictionaries/HashTableWithLinearProbing.h"

TEST(HashTableWithLinearProbing, add) {
  HashTableWithLinearProbing<int, 10> map;
  map.insert("a", 1);
  EXPECT_EQ(1, map.search("a"));
  map.insert("b", 2);
  EXPECT_EQ(2, map.search("b"));
  map.insert("a", 3);
  EXPECT_EQ(3, map.search("a"));
}


TEST(HashTableWithLinearProbing, asMap) {
  HashTableWithLinearProbing<int, 1> map;
  EXPECT_EQ(0, map["x"]);

  map["x"] = 10;
  EXPECT_EQ(10, map["x"]);

  EXPECT_ANY_THROW(map["y"]);
}


TEST(HashTableWithLinearProbing, iterator) {
  std::set<std::pair<std::string, int>> s = {{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"xxx", 13}};

  HashTableWithLinearProbing<int, 10> map;
  for (auto &&item : s) {
    map[item.first] = item.second;
  }

  std::set<std::pair<std::string, int>> s1;
  for (auto &&item : map) {
    s1.insert(item);
  }

  EXPECT_EQ(s, s1);
}
