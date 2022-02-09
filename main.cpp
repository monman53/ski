#include <iostream>
#include <string>

#include "ski.hpp"

int main() {
  TermPtr term = std::make_shared<Term>(TermType::S);
  std::cout << term_to_str(term) << std::endl;
  return 0;
}