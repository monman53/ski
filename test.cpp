#include <cassert>
#include <iostream>

#include "ski.hpp"

int main() {
  // Term class
  {
    TermPtr term0 = make_term(TermType::S);
    TermPtr term1 = make_term(TermType::K);
    TermPtr term2 = make_term(TermType::I);

    TermPtr term3 = make_term(term0, term1);
    TermPtr term4 = make_term(term3, term3);

    assert(term_to_str(term3) == "SK");
    assert(term_to_str(term4) == "SK(SK)");
    assert(term4->lhs != term3);
    assert(term4->lhs != term4->rhs);
  }

  // Parent
  {
    auto t0 = make_s();
    auto t1 = make_k();
    auto t2 = make_app(t0, t1);

    assert(t2->lhs->parent.lock() == t2);
    assert(t2->rhs->parent.lock() == t2);
  }

  return 0;
}