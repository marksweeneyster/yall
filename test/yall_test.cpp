#include "yall.hpp"
#include <array>
#include <gtest/gtest.h>
#include <numeric>
#include <ranges>

namespace {
  class Clazz {
    int data;

  public:
    explicit Clazz(int n) : data(n) {}
    int get() const { return data; }
  };

  class NoCopy {
    int data;

  public:
    explicit NoCopy(int n) : data(n) {}
    NoCopy() = delete;

    NoCopy(const NoCopy&)            = delete;
    NoCopy(NoCopy&&)                 = delete;
    NoCopy& operator=(const NoCopy&) = delete;
    NoCopy& operator=(NoCopy&&)      = delete;

    int get() const { return data; }
  };
}// namespace

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
  EXPECT_EQ(ul_list.size(), 0);

  for (auto& val: test_arr) {
    ul_list.push_back(val);
  }
  EXPECT_TRUE(ul_list.front_val().has_value());
  EXPECT_TRUE(ul_list.back_val().has_value());
  EXPECT_FALSE(ul_list.empty());
  EXPECT_EQ(ul_list.size(), sz);

  ul_list.reset();
  EXPECT_FALSE(ul_list.front_val().has_value());
  EXPECT_FALSE(ul_list.back_val().has_value());
  EXPECT_TRUE(ul_list.empty());
  EXPECT_EQ(ul_list.size(), 0);
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
  Clazz obj1(1);
  Clazz obj2(7);
  Clazz obj3(9);
  Clazz obj4(2);

  yall::Yall<Clazz&> ll_clazz;
  ll_clazz.push_back(obj1);
  ll_clazz.push_back(obj2);
  ll_clazz.push_back(obj3);

  EXPECT_TRUE(ll_clazz.front(obj4));
  EXPECT_EQ(obj4.get(), obj1.get());
  ll_clazz.pop_front();

  EXPECT_TRUE(ll_clazz.front(obj4));
  EXPECT_EQ(obj4.get(), obj2.get());
  ll_clazz.pop_front();

  EXPECT_TRUE(ll_clazz.front(obj4));
  EXPECT_EQ(obj4.get(), obj3.get());
  ll_clazz.pop_front();

  EXPECT_TRUE(ll_clazz.empty());

  ll_clazz.push_back(obj1);
  ll_clazz.push_back(obj2);
  ll_clazz.push_back(obj3);

  EXPECT_TRUE(ll_clazz.back(obj4));
  EXPECT_EQ(obj4.get(), obj3.get());
  ll_clazz.pop_back();

  EXPECT_TRUE(ll_clazz.back(obj4));
  EXPECT_EQ(obj4.get(), obj2.get());
  ll_clazz.pop_back();

  EXPECT_TRUE(ll_clazz.back(obj4));
  EXPECT_EQ(obj4.get(), obj1.get());
  ll_clazz.pop_back();
}

TEST(ConstIterTest, Empty) {
  yall::Yall<unsigned int&> u_list;

  EXPECT_EQ(u_list.cbegin(), u_list.cend());
  EXPECT_EQ(u_list.crbegin(), u_list.crend());
}

TEST(ConstIterTest, FwdRev) {
  constexpr size_t sz = 101;
  unsigned int test_arr[sz];

  std::iota(test_arr, test_arr + sz, 0);

  yall::Yall<unsigned int&> u_list;

  for (auto& val: test_arr) {
    u_list.push_front(val);
  }

  size_t indx = 0;
  for (auto it = u_list.crbegin(); it != u_list.crend(); ++it, ++indx) {
    EXPECT_EQ(*it, test_arr[indx]);
  }

  u_list.reset();
  for (auto& val: test_arr) {
    u_list.push_back(val);
  }
  indx = 0;
  for (auto it = u_list.cbegin(); it != u_list.cend(); ++it, ++indx) {
    EXPECT_EQ(*it, test_arr[indx]);
  }
}

TEST(ConstIterTest, PostFwdRev) {
  constexpr size_t sz = 101;
  unsigned int test_arr[sz];

  std::iota(test_arr, test_arr + sz, 0);

  yall::Yall<unsigned int&> u_list;

  for (auto& val: test_arr) {
    u_list.push_front(val);
  }

  size_t indx = 0;
  for (auto it = u_list.crbegin(); it != u_list.crend(); it++, ++indx) {
    EXPECT_EQ(*it, test_arr[indx]);
  }

  u_list.reset();
  for (auto& val: test_arr) {
    u_list.push_back(val);
  }
  indx = 0;
  for (auto it = u_list.cbegin(); it != u_list.cend(); it++, ++indx) {
    EXPECT_EQ(*it, test_arr[indx]);
  }
}

TEST(ConstIterTest, RangeLoop) {
  constexpr size_t sz = 101;
  unsigned int test_arr[sz];

  std::iota(test_arr, test_arr + sz, 0);

  yall::Yall<unsigned int&> u_list;

  for (auto& val: test_arr) {
    u_list.push_back(val);
  }

  size_t indx = 0;
  for (auto n: u_list) {
    EXPECT_EQ(n, test_arr[indx++]);
  }
}

// Yall front/back methods won't compile for a no-copy type but the iterators
// allow us to access references to these objects stored in a Yall container.
TEST(ConstIterTest, NoCopy) {
  std::array<int, 4> arr = {2, 4, 6, 8};
  NoCopy obj0(arr[0]);
  NoCopy obj1(arr[1]);
  NoCopy obj2(arr[2]);
  NoCopy obj3(arr[3]);

  yall::Yall<const NoCopy&> ll_nc;
  ll_nc.push_back(obj0);
  ll_nc.push_back(obj1);
  ll_nc.push_back(obj2);
  ll_nc.push_back(obj3);

  size_t indx = 0;
  for (const auto& nc: ll_nc) {
    EXPECT_EQ(nc.get(), arr[indx++]);
  }

  auto& front0 = *ll_nc.cbegin();
  EXPECT_EQ(front0.get(), obj0.get());

  ll_nc.pop_front();
  auto& front1 = *ll_nc.cbegin();
  EXPECT_EQ(front1.get(), obj1.get());

  ll_nc.pop_front();
  auto& front2 = *ll_nc.cbegin();
  EXPECT_EQ(front2.get(), obj2.get());

  ll_nc.pop_front();// only one item left in list
  auto& back = *ll_nc.crbegin();
  EXPECT_EQ(back.get(), obj3.get());

  ll_nc.pop_back();
  EXPECT_TRUE(ll_nc.empty());
}

TEST(ConstIterTest, NoCopyRanges) {
  std::array<int, 4> arr = {2, 3, 4, 5};
  NoCopy obj0(arr[0]);
  NoCopy obj1(arr[1]);
  NoCopy obj2(arr[2]);
  NoCopy obj3(arr[3]);

  yall::Yall<const NoCopy&> ll_nc;
  ll_nc.push_back(obj0);
  ll_nc.push_back(obj1);
  ll_nc.push_back(obj2);
  ll_nc.push_back(obj3);

  // example from: https://en.cppreference.com/w/cpp/ranges
  auto even   = [](const NoCopy& nc) { return 0 == nc.get() % 2; };
  auto square = [](const NoCopy& nc) {
    auto n = nc.get();
    return n * n;
  };

  int indx = 0;
  for (int sq:
       ll_nc | std::views::filter(even) | std::views::transform(square)) {
    auto expected_sq = arr[indx] * arr[indx];
    EXPECT_EQ(sq, expected_sq);
    indx += 2;
  }
}
