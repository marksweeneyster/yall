#include "apps.hpp"
#include <algorithm>

int main() {
  using namespace yall;

  Yall<Foo> ll_foo;

  ll_foo.push_front(Foo("zero",0));
  ll_foo.push_back(Foo("one",1));
  ll_foo.push_front(Foo("two",2));
  ll_foo.push_back(Foo("three",3));
  ll_foo.push_front(Foo("four",4));
  ll_foo.push_back(Foo("five",5));
  ll_foo.push_front(Foo("six",6));
  ll_foo.push_back(Foo("seven",7));
  ll_foo.push_front(Foo("eight",8));
  ll_foo.push_back(Foo("nine",9));

  for (auto& foo : ll_foo) {
    std::cout << foo << '\n';
  }

  auto max_foo = std::max_element(ll_foo.begin(), ll_foo.end());
  auto min_foo = std::min_element(ll_foo.begin(), ll_foo.end());

  std::cout << "\nmax element: " << *max_foo << '\n';
  std::cout << "min element: " << *min_foo << '\n';
}