//
// Created by Dustin Cobas Batista on 07-Nov-17.
//

#include <fstream>
#include <cstring>
#include <queue>
#include <map>
#include <unordered_map>
#include <memory>
#include <chrono>

#include <benchmark/benchmark.h>

#include "string-dictionaries/BlindTree.h"
#include "string-dictionaries/TernaryTree.h"
#include "string-dictionaries/HashTableWithLinearProbing.h"
#include "string-dictionaries/hash_function.h"

enum class DictionaryType {
  BlindTree,
  TernaryTree,
  HashTable,
  HashTableDjb2,
  HashTableSdbm,
  HashTableHorner,
  StdMap,
  StdUnorderedMap
};

std::string to_string(DictionaryType _type) {
  switch (_type) {
    case DictionaryType::BlindTree:return "BlindTree";
    case DictionaryType::TernaryTree:return "TernaryTree";
    case DictionaryType::HashTable:return "HashTable";
    case DictionaryType::HashTableDjb2:return "HashTableDjb2";
    case DictionaryType::HashTableSdbm:return "HashTableSdbm";
    case DictionaryType::HashTableHorner:return "HashTableHorner";
    case DictionaryType::StdMap:return "StdMap";
    case DictionaryType::StdUnorderedMap:return "StdUnorderedMap";
  }
}

struct Counter {
  void operator()(const std::pair<std::string, std::pair<int, int>> &_data) {
    total += _data.second.first + _data.second.second;

    if (_data.second.first > 0 && _data.second.second > 0)
      overlap += std::abs(_data.second.first - _data.second.second);

//    std::cout << "XXX " << _data.first << " --- " << _data.second.first << " --- " << _data.second.second << std::endl;
  };

  void operator()(const std::pair<std::string, std::vector<int>> &_data) {
    total += _data.second.size();
  }

  int total = 0;
  int overlap = 0;
} counter;

template<typename _V>
class Dictionary {
 public:
  virtual _V &operator[](const std::string &_key) {}// = 0;
  virtual bool search(const std::string &_key) {}// = 0;
  virtual void traverse(Counter &_counter) {}// = 0;

  virtual std::size_t size() { return 0; } // = 0;

  virtual void clear() {}// = 0;

  virtual std::string asString() { return ""; }// = 0;
};

template<typename _V, typename _D>
class DictionarySimple : public Dictionary<_V> {
 public:
  DictionarySimple() {}
  DictionarySimple(std::size_t _size) : dictionary{_size} {}

  virtual _V &operator[](const std::string &_key) { return dictionary[_key]; }
  virtual bool search(const std::string &_key) { return dictionary.search(_key); }
  virtual void traverse(Counter &_counter) { dictionary.traverse(_counter); }// = 0;

  virtual std::size_t size() { return dictionary.nodesNumber(); }

  virtual void clear() {}// = 0;

  virtual std::string asString() { return ""; }// = 0;

 protected:
  _D dictionary;
};

template<typename _V, typename _D>
class DictionaryHash : public Dictionary<_V> {
 public:
  DictionaryHash() {}
  DictionaryHash(std::size_t _size) : dictionary{_size} {}

  virtual _V &operator[](const std::string &_key) { return dictionary[_key]; }
  virtual bool search(const std::string &_key) { return dictionary.search(_key); }
  virtual void traverse(Counter &_counter) {
    dictionary.traverse(_counter);
  }// = 0;

  virtual std::size_t size() { return dictionary.size(); }

  virtual void clear() {}// = 0;

  virtual std::string asString() { return ""; }// = 0;

 protected:
  _D dictionary;
};

template<typename _V, typename _D>
class DictionaryStd : public Dictionary<_V> {
 public:
  virtual _V &operator[](const std::string &_key) { return dictionary[_key]; }
  virtual bool search(const std::string &_key) { return dictionary.find(_key) != dictionary.end(); }// = 0;
  virtual void traverse(Counter &_counter) {
    for (auto &&item :dictionary) {
      _counter(item);
    }
  }// = 0;

  virtual std::size_t size() { return dictionary.size(); }

  virtual void clear() {}// = 0;

  virtual std::string asString() { return ""; }// = 0;

 protected:
  _D dictionary;
};

template<typename _Value>
std::shared_ptr<Dictionary<_Value>> createDictionary(DictionaryType _type, std::size_t _size = 50000) {
  switch (_type) {
    case DictionaryType::BlindTree:
      return std::shared_ptr<Dictionary<_Value>>(new DictionarySimple<_Value,
                                                                      BlindTree<_Value>>);
    case DictionaryType::TernaryTree:
      return std::shared_ptr<Dictionary<_Value>>(new DictionarySimple<_Value,
                                                                      TernaryTree<_Value>>);
    case DictionaryType::HashTable:
      return std::shared_ptr<Dictionary<_Value>>(new DictionaryHash<_Value,
                                                                    HashTableWithLinearProbing<_Value>>(_size));
    case DictionaryType::HashTableDjb2:
      return std::shared_ptr<Dictionary<_Value>>(new DictionaryHash<_Value,
                                                                    HashTableWithLinearProbing<_Value,
                                                                                               HashDjb2>>(_size));
    case DictionaryType::HashTableSdbm:
      return std::shared_ptr<Dictionary<_Value>>(new DictionaryHash<_Value,
                                                                    HashTableWithLinearProbing<_Value,
                                                                                               HashSdbm>>(_size));
    case DictionaryType::HashTableHorner:
      return std::shared_ptr<Dictionary<_Value>>(new DictionaryHash<_Value,
                                                                    HashTableWithLinearProbing<_Value,
                                                                                               HashHorner>>(_size));
    case DictionaryType::StdMap:
      return std::shared_ptr<Dictionary<_Value>>(new DictionaryStd<_Value,
                                                                   std::map<std::string,
                                                                            _Value>>);
    case DictionaryType::StdUnorderedMap:
      return std::shared_ptr<Dictionary<_Value>>(new DictionaryStd<_Value,
                                                                   std::unordered_map<std::string,
                                                                                      _Value>>);
  }
}

//auto dictionaries = std::make_tuple(std::make_shared<BlindTree<std::vector<int>>>(), std::make_shared<TernaryTree<std::vector<int>>>());

std::shared_ptr<Dictionary<std::vector<int>>> dict;

auto BM_create = [](benchmark::State &_state, auto _type) {
  for (auto _ : _state) {
    dict = createDictionary<std::vector<int>>(_type);
  }

  _state.counters["size"] = 0;
};

auto BM_insert = [](benchmark::State &_state, auto _type, std::size_t _size, const std::string &_file) {
  for (auto _ : _state) {
    dict = createDictionary<std::vector<int>>(_type, _size);
    std::ifstream ifs(_file.c_str());

    std::string word;
    int eC = 0;
    while (ifs >> word) {
      (*dict)[word].push_back(eC++);
    }
  }

  _state.counters["size"] = dict->size();

//  std::cout << to_string(_type) << " ---> " << dict->size() << std::endl;
};

std::ofstream ofsSearchResult("searchResult.csv");
auto initialTime = std::chrono::high_resolution_clock::now();

const std::size_t MAX_WORD_SIZE = 16;
std::array<std::chrono::nanoseconds, MAX_WORD_SIZE> foundTimes = {initialTime - initialTime};
std::array<int, MAX_WORD_SIZE> foundCounts = {0};

std::array<std::chrono::nanoseconds, MAX_WORD_SIZE> missTimes = {initialTime - initialTime};
std::array<int, MAX_WORD_SIZE> missCounts = {0};

auto BM_search = [](benchmark::State &_state, const std::string &_file) {
  for (auto _ : _state) {
    std::ifstream ifs(_file.c_str());

    std::string word;
    int eC = 0;
    while (ifs >> word) {
      if (word.size() >= MAX_WORD_SIZE)
        continue;

      auto start = std::chrono::high_resolution_clock::now();
      if (dict->search(word)) {
        auto end = std::chrono::high_resolution_clock::now();
        foundTimes[word.size()] += end - start;
        ++foundCounts[word.size()];
      } else {
        auto end = std::chrono::high_resolution_clock::now();
        missTimes[word.size()] += end - start;
        ++missCounts[word.size()];
      }
    }
  }

  _state.counters["size"] = 0;
};

auto BM_search_report = [](benchmark::State &_state, const std::string &_name) {
  for (auto _ : _state) {
    ofsSearchResult << _name;
    for (int i = 1; i < MAX_WORD_SIZE; ++i) {
      ofsSearchResult << "," << (foundCounts[i] > 0 ? foundTimes[i].count() / foundCounts[i] : 0) << ","
                      << (missCounts[i] > 0 ? missTimes[i].count() / missCounts[i] : 0);

      foundTimes[i] = missTimes[i] = initialTime - initialTime;
      foundCounts[i] = missCounts[i] = 0;
    }
    ofsSearchResult << std::endl;
  }

  _state.counters["size"] = 0;
};

auto BM_traverse = [](benchmark::State &_state) {
  for (auto _ : _state) {
    counter.total = counter.overlap = 0;
    dict->traverse(counter);
  }

  _state.counters["size"] = 0;
};

//Similarity Benchmarks
std::shared_ptr<Dictionary<std::pair<int, int>>> dictSimilarity;

auto BM_insert_similarity =
    [](benchmark::State &_state, auto _type, std::size_t _size, const std::string &_file1, const std::string &_file2) {
      for (auto _ : _state) {
        dictSimilarity = createDictionary<std::pair<int, int>>(_type, _size);
        int c = 0;
        {
          std::ifstream ifs(_file1.c_str());

          std::string word;
          while (ifs >> word) {
            ++c;
            ++((*dictSimilarity)[word].first);
          }
        }

        {
          std::ifstream ifs(_file2.c_str());

          std::string word;
          while (ifs >> word) {
            ++c;
            ++((*dictSimilarity)[word].second);
          }
        }
      }

      _state.counters["size"] = dictSimilarity->size();
    };

auto BM_calculate_similarity = [](benchmark::State &_state) {
  for (auto _ : _state) {
    counter.total = counter.overlap = 0;
    dictSimilarity->traverse(counter);
  }

  _state.counters["size"] = 0;
};

int main(int argc, char **argv) {

  ofsSearchResult << "name";
  for (int i = 1; i < MAX_WORD_SIZE; ++i) {
    ofsSearchResult << ",found_" + std::to_string(i) << ",missed_" + std::to_string(i);
  }
  ofsSearchResult << std::endl;

  std::string folder1 = "../test/data/text1/";

  //Search Test
  for (int i = 12, size = 1 << (i - 1); i < 21; ++i, size *= 2) {
    for (auto &type : {DictionaryType::BlindTree, DictionaryType::TernaryTree, DictionaryType::HashTable,
                       DictionaryType::HashTableDjb2, DictionaryType::HashTableSdbm, DictionaryType::HashTableHorner,
                       DictionaryType::StdMap, DictionaryType::StdUnorderedMap}) {
//      benchmark::RegisterBenchmark(("Create/" + to_string(type)).c_str(), BM_create, type);

      benchmark::RegisterBenchmark(("Query/Insert/" + to_string(type) + "/" + std::to_string(i)).c_str(),
                                   BM_insert,
                                   type,
                                   size < 100000 ? size : 100000,
                                   folder1 + std::to_string(i))->Unit(benchmark::kMicrosecond);

      benchmark::RegisterBenchmark(("Query/Search/" + to_string(type) + "/" + std::to_string(i)).c_str(),
                                   BM_search,
                                   "../test/data/google-10000-english.txt")->Unit(benchmark::kMicrosecond);

      benchmark::RegisterBenchmark(("Query/SearchReport/" + to_string(type) + "/" + std::to_string(i)).c_str(),
                                   BM_search_report,
                                   "Search/" + to_string(type) + "/" + std::to_string(i))->Iterations(1)
          ->Unit(benchmark::kMicrosecond);

      benchmark::RegisterBenchmark(("Query/Traverse/" + to_string(type) + "/" + std::to_string(i)).c_str(),
                                   BM_traverse)->Unit(benchmark::kMicrosecond);
    }
  }

  for (auto &type : {DictionaryType::BlindTree, DictionaryType::TernaryTree, DictionaryType::HashTable,
                     DictionaryType::HashTableDjb2, DictionaryType::HashTableSdbm, DictionaryType::HashTableHorner,
                     DictionaryType::StdMap, DictionaryType::StdUnorderedMap}) {
//      benchmark::RegisterBenchmark(("Create/" + to_string(type)).c_str(), BM_create, type);

    benchmark::RegisterBenchmark(("Query/Insert/" + to_string(type) + "/words").c_str(),
                                 BM_insert,
                                 type,
                                 1000000,
                                 "../test/data/words_alpha.txt")->Unit(benchmark::kMicrosecond);

    benchmark::RegisterBenchmark(("Query/Search/" + to_string(type) + "/words").c_str(),
                                 BM_search,
                                 "../test/data/google-10000-english.txt")->Unit(benchmark::kMicrosecond);

    benchmark::RegisterBenchmark(("Query/SearchReport/" + to_string(type) + "/words").c_str(),
                                 BM_search_report,
                                 "Search/" + to_string(type) + "/words")->Iterations(1)
        ->Unit(benchmark::kMicrosecond);

    benchmark::RegisterBenchmark(("Query/Traverse/" + to_string(type) + "/words").c_str(),
                                 BM_traverse)->Unit(benchmark::kMicrosecond);
  }

  std::string folder2 = "../test/data/text2/";

  //Search Test
  for (int i = 12, size = 1 << (i - 1); i < 21; ++i, size *= 2) {
    for (auto &type : {DictionaryType::BlindTree, DictionaryType::TernaryTree, DictionaryType::HashTable,
                       DictionaryType::StdMap, DictionaryType::StdUnorderedMap}) {
//      benchmark::RegisterBenchmark(("Create/" + to_string(type)).c_str(), BM_create, type);

      benchmark::RegisterBenchmark(("Similarity/Insert/" + to_string(type) + "/" + std::to_string(i)).c_str(),
                                   BM_insert_similarity,
                                   type,
                                   size < 100000 ? size : 100000,
                                   folder1 + std::to_string(i), folder2 + std::to_string(i))
          ->Unit(benchmark::kMicrosecond);
      benchmark::RegisterBenchmark(("Similarity/Calculate/" + to_string(type) + "/" + std::to_string(i)).c_str(),
                                   BM_calculate_similarity)->Unit(benchmark::kMicrosecond);
    }
  }
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}