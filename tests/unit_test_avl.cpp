#include <gtest/gtest.h>
#include "trees.hpp"

using namespace trees;

TEST(AvlTreeTest, DefaultConstructor) {
  AvlTree<int> tree;
  EXPECT_EQ(tree.size(), 0);
}

TEST(AvlTreeTest, InsertRootAndSize) {
  AvlTree<int> tree;

  tree.insert(5);
  EXPECT_EQ(tree.size(), 1);

  tree.insert(3);
  EXPECT_EQ(tree.size(), 2);

  tree.insert(7);
  EXPECT_EQ(tree.size(), 3);

  tree.insert(5);
  EXPECT_EQ(tree.size(), 3); 
}


TEST(AvlTreeTest, CustomComparator) {
  AvlTree<int, std::greater<int>> tree;

  tree.insert(5);
  EXPECT_EQ(tree.size(), 1);

  tree.insert(3);
  EXPECT_EQ(tree.size(), 2);

  tree.insert(7);
  EXPECT_EQ(tree.size(), 3);

  tree.insert(5);
  EXPECT_EQ(tree.size(), 3); 
}

TEST(AvlTreeTest, TestLowerBound) {
  AvlTree<int> tree;

  tree.insert(5);
  EXPECT_EQ(tree.size(), 1);

  tree.insert(3);
  EXPECT_EQ(tree.size(), 2);

  tree.insert(7);
  EXPECT_EQ(tree.size(), 3);

  tree.insert(5);
  EXPECT_EQ(tree.size(), 3); 

  auto it = tree.lower_bound(4);
  EXPECT_EQ(tree.get_key(it), 5);
}

TEST(AvlTreeTest, TestUpperBoundEnd) {
  AvlTree<int> tree;

  tree.insert(5);
  EXPECT_EQ(tree.size(), 1);

  tree.insert(3);
  EXPECT_EQ(tree.size(), 2);

  tree.insert(7);
  EXPECT_EQ(tree.size(), 3);

  tree.insert(5);
  EXPECT_EQ(tree.size(), 3); 

  auto it = tree.upper_bound(10);
  EXPECT_EQ(it, tree.end());
}

TEST(AvlTreeTest, TestUpperBoundNotEnd) {
  AvlTree<int> tree;

  tree.insert(5);
  tree.insert(3);
  tree.insert(7);

  EXPECT_EQ(tree.size(), 3);
  auto it = tree.upper_bound(6);
  EXPECT_EQ(tree.get_key(it), 7);
}

TEST(AvlTreeTest, TestDistance) {
  AvlTree<int> tree;

  tree.insert(5);
  tree.insert(3);
  tree.insert(7);

  auto it_lower = tree.lower_bound(2);
  EXPECT_NE(it_lower, tree.end());
  EXPECT_EQ(tree.get_key(it_lower), 3);

  auto it_upper = tree.upper_bound(6);
  EXPECT_NE(it_upper, tree.end());
  EXPECT_EQ(tree.get_key(it_upper), 7);

  long dist = tree.distance(it_lower, it_upper);
  EXPECT_EQ(dist, 2);
}

TEST(AvlTreeTest, TestDistanceAfterEnd) {
  AvlTree<int> tree;

  tree.insert(5);
  tree.insert(3);
  tree.insert(7);

  auto it_lower = tree.lower_bound(3);
  EXPECT_NE(it_lower, tree.end());
  EXPECT_EQ(tree.get_key(it_lower), 3);

  auto it_upper = tree.upper_bound(7);
  EXPECT_EQ(it_upper, tree.end());

  long dist = tree.distance(it_lower, it_upper);
  EXPECT_EQ(dist, 3);
}
