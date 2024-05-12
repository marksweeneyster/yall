#ifndef YALL_APPS_APPS_HPP
#define YALL_APPS_APPS_HPP

#include "yall.hpp"
#include <iostream>

namespace yall {

  auto dbl_printer = [](const double& d) { std::cout << d << " <--> "; };

  template<typename T>
  void fn_a(Yall<T>& llist) {
    double d1 = 3.14;
    double d2 = 2.7;
    double d3 = 42.0;

    llist.push_front(d1);
    llist.push_back(d2);
    llist.push_front(d3);
  }

  struct Fubar {
    Fubar(std::string nm="fubar", int id_=42) : name(nm), id(id_) {}
    std::string name;
    int id;
  };

  std::ostream& operator<<(std::ostream& os, const Fubar& p) {
    return os << "name: \"" << p.name << "\", id: " << p.id;
  }

  auto fubar_printer = [](const yall::Fubar& fub) {
    std::cout << fub << " <--> ";
  };

  template<typename T>
  void fn_b(Yall<T>& llist) {
    yall::Fubar fubar1;
    yall::Fubar fubar2("gubar", 7);

    llist.push_front(fubar1);
    llist.push_back(fubar2);
  }
};
#endif//YALL_APPS_APPS_HPP
