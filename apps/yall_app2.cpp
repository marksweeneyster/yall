#include "apps.hpp"

int main() {
  using namespace yall;

  Yall<double&> ll_dref;
  fn_a<double&>(ll_dref);
  ll_dref.print(dbl_printer);

  Yall<Fubar&> ll_fubref;
  fn_b<Fubar&>(ll_fubref);
  ll_fubref.print(fubar_printer);

  return 0;
}
