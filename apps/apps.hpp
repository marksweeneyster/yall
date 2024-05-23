#ifndef YALL_APPS_APPS_HPP
#define YALL_APPS_APPS_HPP

#include "yall.hpp"
#include <iostream>
#include <utility>

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

  struct Foo {
    explicit Foo(std::string nm = "foo", int id_ = 42)
        : name(std::move(nm)), id(id_) {}
    std::string name;
    int id;

    bool operator<(Foo const &rhs) const { return id < rhs.id; }
  };


  std::ostream& operator<<(std::ostream& os, const Foo& p) {
    return os << "name: \"" << p.name << "\", id: " << p.id;
  }

  auto foo_printer = [](const yall::Foo& foo) { std::cout << foo << " <--> "; };

  template<typename T>
  void fn_b(Yall<T>& llist) {
    yall::Foo foo1;
    yall::Foo foo2("goo", 7);

    llist.push_front(foo1);
    llist.push_back(foo2);
  }
}// namespace yall
#endif//YALL_APPS_APPS_HPP
