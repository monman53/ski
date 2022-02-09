#include <iostream>
#include <string>

#include "ski.hpp"

int main() {
  auto i = make_i();
  auto s = make_s();
  auto ss = make_app(s, s);
  auto is = make_app(i, s);
  auto ii = make_app(i, i);
  auto isis = make_app(is, is);
  auto result = eval(isis);
  std::cout << term_to_str(result) << std::endl;
  return 0;
}