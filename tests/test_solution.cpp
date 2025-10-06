#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <set>
#include <vector>
#include <string>

#include "trees.hpp"
#include "timer.hpp"

template <typename Container, typename DistanceFunc, typename KeyType>
long RangeQuery(const Container& set, DistanceFunc distance_func, KeyType lhs, KeyType rhs) {
  auto begin = set.lower_bound(lhs);
  auto end = set.upper_bound(rhs);
  return distance_func(begin, end);
}

template<typename Container, typename RangeQueryFunc>
std::pair<std::vector<long>, double> RunTest(std::istream& istream, Container& container, RangeQueryFunc range_query_func) {
  std::vector<long> answer;

  timer::Timer solution_timer;
  solution_timer.start();

  char query;
  while (istream >> query) {
    if (query == 'k') {
      int number = 0;
      istream >> number;
      container.insert(number);
    } else if (query == 'q') {
      int begin = 0;
      int end = 0;
      istream >> begin >> end;
      answer.push_back(range_query_func(container, begin, end));
    } else {
      std::cout << "Could not read test\n";
      return {};
    }
  }

  return {answer, solution_timer.elapsed_milliseconds()};
}

template<typename Iterator>
long StdDistance(Iterator begin, Iterator end) {
  return std::distance(begin, end);
}

template<typename KeyType>
long RangeQueryStdSet(const std::set<KeyType>& set, KeyType lhs, KeyType rhs) {
  return RangeQuery(set, StdDistance<typename std::set<KeyType>::iterator>, lhs, rhs);
}

template<typename KeyType>
long RangeQueryAvlTree(const trees::AvlTree<KeyType>& tree, KeyType lhs, KeyType rhs) {
  return RangeQuery(tree, [&tree](auto begin, auto end) { return tree.distance(begin, end); }, lhs, rhs);
}

class TreeRangeQueryTest : public ::testing::TestWithParam<int> {
 protected:
  const std::filesystem::path kPathToFile = "tests/dats/";

  void SetUp() override {
    test_number_ = GetParam();
  }

  int test_number_;


  std::string get_test_file_name() const {
    return kPathToFile.string() + "test_" + std::to_string(test_number_) + ".dat";
  }
};

TEST_P(TreeRangeQueryTest, CompareAvlTreeWithStdSet) {
  std::string test_file = get_test_file_name();
  std::ifstream input_file(test_file);

  if (!input_file.is_open()) {
    GTEST_SKIP() << "Test file " << test_file << " not found";
  }

  trees::AvlTree<int> avl_tree;
  auto [avl_result, avl_time] = RunTest(input_file, avl_tree, RangeQueryAvlTree<int>);

  input_file.clear();
  input_file.seekg(0);

  std::set<int> std_set;
  auto [std_result, std_time] = RunTest(input_file, std_set, RangeQueryStdSet<int>);

  std::cout << "Test " << test_number_ << " timing results:\n";
  std::cout << "  Difference: " << (avl_time - std_time) << " ms\n";
  std::cout << "  AVL is " << (avl_time / std_time) << "x slower than std::set\n";

  EXPECT_EQ(avl_result.size(), std_result.size()) 
    << "Different result sizes for test " << test_number_;

  for (size_t i = 0; i < std::min(avl_result.size(), std_result.size()); ++i) {
    EXPECT_EQ(avl_result[i], std_result[i]) 
      << "Different results at position " << i << " for test " << test_number_;
  }
}

INSTANTIATE_TEST_SUITE_P(
  TreeTests,
  TreeRangeQueryTest,
  ::testing::Range(1, 4), 
  [](const ::testing::TestParamInfo<TreeRangeQueryTest::ParamType>& info) {
    return "Test" + std::to_string(info.param);
  }
);

