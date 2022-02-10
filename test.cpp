#include <cassert>
#include <iostream>

#include "ski.hpp"
#include "skilib.hpp"

// Test if evaluated `actual` term is equal to `expected` term.
void assert_eval(TermPtr actual, TermPtr expected) {
  auto actual_str = term_to_str(actual);
  auto evaluated_actual = eval(actual);
  if (!same(evaluated_actual, expected)) {
    std::cerr << "Term " << actual_str << " is not reduced to "
              << term_to_str(expected) << "." << std::endl;
    std::cerr << "actual  : " << term_to_str(evaluated_actual) << std::endl;
    std::cerr << "expected: " << term_to_str(expected) << std::endl;
  }
}

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

  // Eval
  {
    auto i = make_i();
    auto k = make_k();
    auto s = make_s();

    // I
    auto ik = make_app(i, k);
    assert(term_to_str(eval(ik)) == "K");

    // K
    auto kis = make_app(make_app(k, i), s);
    assert(term_to_str(eval(kis)) == "I");

    // S
    auto sski = make_app(make_app(make_app(s, s), k), i);
    assert(term_to_str(eval(sski)) == "SI(KI)");

    // SKI
    auto sksk = make_app(make_app(make_app(s, k), s), k);
    assert(term_to_str(eval(sksk)) == "K");

    auto i_ik_ = make_app(i, make_app(i, k));
    assert(term_to_str(eval(i_ik_)) == "K");

    auto ik_ik_ = make_app(ik, ik);
    assert(term_to_str(eval(ik_ik_)) == "KK");

    // Wikipedia examples
    auto ski_kis_ =
        make_app(make_app(make_app(s, k), i), make_app(make_app(k, i), s));
    assert(term_to_str(eval(ski_kis_)) == "I");

    auto ks_i_sksi__ = make_app(
        make_app(k, s), make_app(i, make_app(make_app(make_app(s, k), s), i)));
    assert(term_to_str(eval(ks_i_sksi__)) == "S");

    auto skik = make_app(make_app(make_app(s, k), i), k);
    assert(term_to_str(eval(skik)) == "K");
  }

  // SKI lib
  {
    // NOT
    assert_eval(make_app(NOT, TRUE), FALSE);
    assert_eval(make_app(NOT, FALSE), TRUE);

    // OR
    assert_eval(make_app(make_app(OR, FALSE), FALSE), FALSE);
    assert_eval(make_app(make_app(OR, FALSE), TRUE), TRUE);
    assert_eval(make_app(make_app(OR, TRUE), FALSE), TRUE);
    assert_eval(make_app(make_app(OR, TRUE), TRUE), TRUE);

    // AND
    assert_eval(make_app(make_app(AND, FALSE), FALSE), FALSE);
    assert_eval(make_app(make_app(AND, FALSE), TRUE), FALSE);
    assert_eval(make_app(make_app(AND, TRUE), FALSE), FALSE);
    assert_eval(make_app(make_app(AND, TRUE), TRUE), TRUE);
  }

  return 0;
}