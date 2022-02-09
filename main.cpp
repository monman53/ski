#include <iostream>
#include <string>

#include "ski.hpp"

int main() {
  TermPtr term = std::make_shared<Term>(TermType::S);
  std::cout << term_to_str(term) << std::endl;

  term = make_term(term, term);
  std::cout << term_to_str(term) << std::endl;

  if (term == term->lhs->parent.lock()) {
    std::cout << "debug" << std::endl;
  }

  return 0;
}