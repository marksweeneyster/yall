#include "yall.hpp"
#include <iostream>

struct Fubar {
  std::string name{"fubar"};
  int id = 42;
};

std::ostream& operator<<(std::ostream& os, const Fubar& p) {
  return os << "name: \"" << p.name << "\", id: " << p.id;
}

int main() {
  double d  = 3.14;
  double d2 = 2.7;
  double d3 = 42.0;
  double d4 = 101.111;
  double d5 = 987.654;

  auto dbl_printer = [](const double& d){
    std::cout << d << " <--> ";
  };
  {
    yall::Yall<double&> d_yall;

    d_yall.pop_front();

    auto op = d_yall.front_val();
    if (op) {
      std::cout << " head value: " << op.value() << '\n';
    } else {
      std::cout << "empty head\n";
    }

    d_yall.push_front(d);
    op = d_yall.front_val();
    if (op) {
      std::cout << " head value: " << op.value() << '\n';
    } else {
      std::cout << "empty head\n";
    }
    op = d_yall.back_val();
    if (op) {
      std::cout << " tail value: " << op.value() << '\n';
    } else {
      std::cout << "empty tail\n";
    }
    d_yall.print(dbl_printer);
    d_yall.push_back(d2);
    d_yall.print(dbl_printer);

    d_yall.push_back(d3);
    d_yall.print(dbl_printer);
    d_yall.push_front(d4);
    d_yall.push_front(d5);

    d_yall.print(dbl_printer);
    bool b = d_yall.remove_first(d);
    double x = 0.0;
    b = d_yall.remove_first(x);
    d2 = 999.999;

    d_yall.print(dbl_printer);

    op = d_yall.front_val();
    if (op) {
      std::cout << " head value: " << op.value() << '\n';
    } else {
      std::cout << "empty head\n";
    }
    op = d_yall.back_val();
    if (op) {
      std::cout << " tail value: " << op.value() << '\n';
    } else {
      std::cout << "empty tail\n";
    }
    d_yall.pop_front();
    d_yall.print(dbl_printer);

    std::cout << "======================\n";
    d_yall.pop_back();
    d_yall.print(dbl_printer);

    {
      double z = 13.;
      d_yall.push_back(z);
      d_yall.print(dbl_printer);
    }
    d_yall.print(dbl_printer);

    std::cout << "======================\n";
  }

  {
    Fubar fubar;
    Fubar fubar2("gubar", 7);
    yall::Yall<Fubar> ll;

    ll.push_front(fubar);
    ll.push_front(fubar2);

    auto fubar_printer = [](const Fubar& fub){std::cout << fub <<  " <--> ";};
    {
      Fubar yubar("hmmmm",777);
      ll.push_front(yubar);
      ll.print(fubar_printer);
    }
    ll.print(fubar_printer);
    fubar.id = 96;
  }
  {
    yall::Yall<uint32_t&> ref_list;
    uint32_t n = 1;
    ref_list.push_back(n);
    //ref_list.push_back(7u);
    {
      //uint32_t m = 42;
      //ref_list.push_back(m);
    }
    ref_list.print([](const uint32_t& n){std::cout << n << " <--> ";} );
  }
  return 0;
}
