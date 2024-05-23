#include "yall.hpp"
#include <gtest/gtest.h>
#include <numeric>

TEST(YallTest, FrontPushPop) {
  constexpr size_t sz = 101;
  int test_arr[sz];

  std::iota(test_arr, test_arr + sz, 0);

  yall::Yall<int&> dlist;
  EXPECT_FALSE(dlist.front_val().has_value());
  EXPECT_TRUE(dlist.empty());

  for (auto& val: test_arr) {
    dlist.push_front(val);
  }
  EXPECT_FALSE(dlist.empty());

  for (int i = sz - 1; i >= 0; --i) {
    auto val      = test_arr[i];
    auto test_val = dlist.front_val();
    EXPECT_TRUE(test_val.has_value());
    EXPECT_EQ(test_val.value(), val);

    dlist.pop_front();
  }
  EXPECT_TRUE(dlist.empty());
}

TEST(YallTest, BackPushPop) {
  constexpr size_t sz = 101;
  int test_arr[sz];

  std::iota(test_arr, test_arr + sz, 0);

  yall::Yall<int&> dlist;
  EXPECT_FALSE(dlist.back_val().has_value());
  EXPECT_TRUE(dlist.empty());

  for (auto& val: test_arr) {
    dlist.push_back(val);
  }
  EXPECT_FALSE(dlist.empty());

  for (int i = sz - 1; i >= 0; --i) {
    auto val      = test_arr[i];
    auto test_val = dlist.back_val();
    EXPECT_TRUE(test_val.has_value());
    EXPECT_EQ(test_val.value(), val);

    dlist.pop_back();
  }
  EXPECT_TRUE(dlist.empty());
}

TEST(YallTest, RemoveFirst) {
  constexpr size_t sz = 101;
  double test_arr[sz];

  std::iota(test_arr, test_arr + sz, -999.0);

  yall::Yall<double&> dlist;

  // index of to be removed
  constexpr size_t rmv_indx = sz / 3;
  // test with empty list
  EXPECT_FALSE(dlist.remove_first(test_arr[rmv_indx]));

  for (auto& val: test_arr) {
    dlist.push_front(val);
  }

  EXPECT_TRUE(dlist.remove_first(test_arr[rmv_indx]));

  for (int i = sz - 1; i >= 0; --i) {
    auto val      = test_arr[i];
    auto test_val = dlist.front_val();
    EXPECT_TRUE(test_val.has_value());
    if (i != rmv_indx) {
      EXPECT_EQ(test_val.value(), val);
      dlist.pop_front();
    } else {
      EXPECT_NE(test_val.value(), val);
    }
  }
}

TEST(YallTest, RemoveLast) {
  constexpr size_t sz = 101;
  double test_arr[sz];

  std::iota(test_arr, test_arr + sz, -999.0);

  yall::Yall<double&> dlist;

  // index of to be removed
  constexpr size_t rmv_indx = sz / 3;
  // test with empty list
  EXPECT_FALSE(dlist.remove_last(test_arr[rmv_indx]));

  for (auto& val: test_arr) {
    dlist.push_front(val);
  }

  EXPECT_TRUE(dlist.remove_last(test_arr[rmv_indx]));

  for (int i = sz - 1; i >= 0; --i) {
    auto val      = test_arr[i];
    auto test_val = dlist.front_val();
    EXPECT_TRUE(test_val.has_value());
    if (i != rmv_indx) {
      EXPECT_EQ(test_val.value(), val);
      dlist.pop_front();
    } else {
      EXPECT_NE(test_val.value(), val);
    }
  }

  dlist.reset();
  double test_val = 3.14;

  for (auto i = 0UL; i < sz; ++i) {
    dlist.push_front(test_val);
  }

  for (auto i = 0UL; i < sz; ++i) {
    EXPECT_TRUE(dlist.remove_last(test_val));
  }
  // list should be empty
  EXPECT_FALSE(dlist.remove_last(test_val));
  EXPECT_TRUE(dlist.empty());
}

TEST(YallTest, Reset) {
  constexpr size_t sz = 11;
  unsigned long test_arr[sz];

  std::iota(test_arr, test_arr + sz, 0);

  yall::Yall<unsigned long&> ul_list;
  EXPECT_FALSE(ul_list.front_val().has_value());
  EXPECT_FALSE(ul_list.back_val().has_value());
  EXPECT_TRUE(ul_list.empty());

  for (auto& val: test_arr) {
    ul_list.push_back(val);
  }
  EXPECT_TRUE(ul_list.front_val().has_value());
  EXPECT_TRUE(ul_list.back_val().has_value());
  EXPECT_FALSE(ul_list.empty());

  ul_list.reset();
  EXPECT_FALSE(ul_list.front_val().has_value());
  EXPECT_FALSE(ul_list.back_val().has_value());
  EXPECT_TRUE(ul_list.empty());
}

TEST(YallTest, SingleVal) {
  float test_val = 42.0f;

  yall::Yall<float&> ref_list;

  ref_list.push_front(test_val);
  ASSERT_EQ(ref_list.front_val(), test_val);
  ASSERT_EQ(ref_list.back_val(), test_val);

  yall::Yall<float> val_list;

  val_list.push_back(test_val);
  ASSERT_EQ(val_list.front_val(), test_val);
  ASSERT_EQ(val_list.back_val(), test_val);

  test_val = 13.0f;
  ASSERT_EQ(ref_list.front_val(), test_val);
  ASSERT_EQ(ref_list.back_val(), test_val);
  // the value list stored a copy of the test value
  ASSERT_NE(val_list.front_val(), test_val);
  ASSERT_NE(val_list.back_val(), test_val);
}

TEST(YallTest, ConstSingleVal) {
  const float test_val = 42.0f;

  yall::Yall<const float&> flist1;

  flist1.push_front(test_val);
  ASSERT_EQ(flist1.front_val(), test_val);
  ASSERT_EQ(flist1.back_val(), test_val);

  yall::Yall<const float> flist2;

  flist2.push_back(test_val);
  ASSERT_EQ(flist2.front_val(), test_val);
  ASSERT_EQ(flist2.back_val(), test_val);
}

TEST(YallTest, FrontBack) {
  class Clazz {
    int data;

  public:
    explicit Clazz(int n) : data(n) {}
    int get() const { return data; }
  };

  Clazz obj1(1);
  Clazz obj2(7);
  Clazz obj3(9);
  Clazz obj4(2);

  yall::Yall<Clazz&> ll_nc;
  ll_nc.push_back(obj1);
  ll_nc.push_back(obj2);
  ll_nc.push_back(obj3);

  EXPECT_TRUE(ll_nc.front(obj4));
  EXPECT_EQ(obj4.get(), obj1.get());
  ll_nc.pop_front();

  EXPECT_TRUE(ll_nc.front(obj4));
  EXPECT_EQ(obj4.get(), obj2.get());
  ll_nc.pop_front();

  EXPECT_TRUE(ll_nc.front(obj4));
  EXPECT_EQ(obj4.get(), obj3.get());
  ll_nc.pop_front();

  EXPECT_TRUE(ll_nc.empty());

  ll_nc.push_back(obj1);
  ll_nc.push_back(obj2);
  ll_nc.push_back(obj3);

  EXPECT_TRUE(ll_nc.back(obj4));
  EXPECT_EQ(obj4.get(), obj3.get());
  ll_nc.pop_back();

  EXPECT_TRUE(ll_nc.back(obj4));
  EXPECT_EQ(obj4.get(), obj2.get());
  ll_nc.pop_back();

  EXPECT_TRUE(ll_nc.back(obj4));
  EXPECT_EQ(obj4.get(), obj1.get());
  ll_nc.pop_back();
}