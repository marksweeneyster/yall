#include "apps.hpp"

int main() {
  using namespace yall;

  Yall<double&> ll_dref;
  fn_a<double&>(ll_dref);
  ll_dref.print(dbl_printer);

  Yall<Foo&> ll_foo_ref;
  fn_b<Foo&>(ll_foo_ref);
  ll_foo_ref.print(foo_printer);

  return 0;
}
