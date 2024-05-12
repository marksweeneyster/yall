#include "apps.hpp"

int main() {
  using namespace yall;

  Yall<double> ll_dbl;
  fn_a<double>(ll_dbl);
  ll_dbl.print(dbl_printer);

  Yall<Fubar> ll_fub;
  fn_b<Fubar>(ll_fub);
  ll_fub.print(fubar_printer);

  return 0;
}
