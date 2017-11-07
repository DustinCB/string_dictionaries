//
// Created by Dustin Cobas Batista on 07-Nov-17.
//

#include <fstream>

#include "gtest/gtest.h"

#include "string-dictionaries/HashTableWithLinearProbing.h"

TEST(Perf, insert) {
  std::ifstream ifs("/home/dcobas/Workspace/Doctorado/Cursos/Algoritmos/Tasks/Task 2/string-dictionaries/test/data/11615.txt");

  HashTableWithLinearProbing<int, 1100000> map;
  int eC = 0;
  std::string word;
  while (ifs >> word) {
    ++map[word];
    eC++;
  }

  EXPECT_GE(eC, map.size());

  int rC = 0;
  for (auto &&item : map) {
    rC += item.second;
  }
  EXPECT_EQ(eC, rC);
}