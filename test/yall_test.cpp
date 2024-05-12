#include "yall.hpp"
#include <gtest/gtest.h>
#include <numeric>

TEST(YallTest, FrontPushPop) {
  constexpr size_t sz = 101;
  int test_arr[sz];

  std::iota(test_arr, test_arr+sz, 0);

  yall::Yall<int&> dlist;
  EXPECT_FALSE(dlist.front_val().has_value());
  EXPECT_TRUE(dlist.empty());

  for (auto i=0; i<sz; ++i) {
    dlist.push_front(test_arr[i]);
  }
  EXPECT_FALSE(dlist.empty());

  for (int i=sz-1; i>=0; --i) {
    auto val = test_arr[i];
    auto test_val = dlist.front_val();
    EXPECT_TRUE(test_val.has_value());
    EXPECT_EQ(test_val.value(),val);

    dlist.pop_front();
  }
  EXPECT_TRUE(dlist.empty());
}

TEST(YallTest, RemoveFirst) {
  constexpr size_t sz = 101;
  double test_arr[sz];

  std::iota(test_arr, test_arr+sz, -999.0);

  yall::Yall<double&> dlist;

  // index of to be removed
  constexpr size_t rmv_indx = sz/3;
  // test with empty list
  EXPECT_FALSE(dlist.remove_first(test_arr[rmv_indx]));

  for (auto i=0; i<sz; ++i) {
    dlist.push_front(test_arr[i]);
  }

  EXPECT_TRUE(dlist.remove_first(test_arr[rmv_indx]));

  for (int i=sz-1; i>=0; --i) {
    auto val = test_arr[i];
    auto test_val = dlist.front_val();
    EXPECT_TRUE(test_val.has_value());
    if (i != rmv_indx) {
      EXPECT_EQ(test_val.value(),val);
      dlist.pop_front();
    } else {
      EXPECT_NE(test_val.value(),val);
    }
  }
}

TEST(YallTest, RemoveLast) {
  constexpr size_t sz = 101;
  double test_arr[sz];

  std::iota(test_arr, test_arr+sz, -999.0);

  yall::Yall<double&> dlist;

  // index of to be removed
  constexpr size_t rmv_indx = sz/3;
  // test with empty list
  EXPECT_FALSE(dlist.remove_last(test_arr[rmv_indx]));

  for (auto i=0; i<sz; ++i) {
    dlist.push_front(test_arr[i]);
  }

  EXPECT_TRUE(dlist.remove_last(test_arr[rmv_indx]));

  for (int i=sz-1; i>=0; --i) {
    auto val = test_arr[i];
    auto test_val = dlist.front_val();
    EXPECT_TRUE(test_val.has_value());
    if ( i != rmv_indx) {
      EXPECT_EQ(test_val.value(),val);
      dlist.pop_front();
    } else {
      EXPECT_NE(test_val.value(),val);
    }
  }

  dlist.reset();
  double test_val = 3.14;

  for (auto i=0; i<sz; ++i) {
    dlist.push_front(test_val);
  }

  for (auto i=0; i<sz; ++i) {
    EXPECT_TRUE(dlist.remove_last(test_val));
  }
  // list should be empty
  EXPECT_FALSE(dlist.remove_last(test_val));
  EXPECT_TRUE(dlist.empty());
}

TEST(YallTest, Reset) {
  constexpr size_t sz = 11;
  unsigned long test_arr[sz];

  std::iota(test_arr, test_arr+sz, 0);

  yall::Yall<unsigned long&> dlist;
  EXPECT_FALSE(dlist.front_val().has_value());
  EXPECT_FALSE(dlist.back_val().has_value());

  for (auto i=0; i<sz; ++i) {
    dlist.push_back(test_arr[i]);
  }
  EXPECT_TRUE(dlist.front_val().has_value());
  EXPECT_TRUE(dlist.back_val().has_value());

  dlist.reset();
  EXPECT_FALSE(dlist.front_val().has_value());
  EXPECT_FALSE(dlist.back_val().has_value());
}
