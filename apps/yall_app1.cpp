#include "apps.hpp"

int main() {
  using namespace yall;

  Yall<double> ll_dbl;
  fn_a<double>(ll_dbl);
  ll_dbl.print(dbl_printer);

  Yall<Foo> ll_foo;
  fn_b<Foo>(ll_foo);
  ll_foo.print(foo_printer);

  for (const auto& f : ll_foo) {
    std::cout << "\t" << f << '\n';
  }
  return 0;
}
