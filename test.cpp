#include <cassert>
#include <iostream>

#include "ski.hpp"
#include "skilib.hpp"

// Test if evaluated `actual` term is equal to `expected` term.
void assert_eval(TermPtr actual, TermPtr expected) {
  auto actual_str = actual->to_string();
  auto evaluated_actual = eval(actual);
  if (!same(evaluated_actual, expected)) {
    std::cerr << "Term " << actual_str << " is not reduced to "
              << expected->to_string() << "." << std::endl;
    std::cerr << "actual  : " << evaluated_actual->to_string() << std::endl;
    std::cerr << "expected: " << expected->to_string() << std::endl;
    exit(1);
  }
}

void assert_eval(TermPtr actual, std::string expected) {
  auto actual_str = actual->to_string();
  auto evaluated_actual = eval(actual);
  auto evaluated_actual_str = evaluated_actual->to_string();
  if (evaluated_actual_str != expected) {
    std::cerr << "Term " << actual_str << " is not reduced to " << expected
              << "." << std::endl;
    std::cerr << "actual  : " << evaluated_actual_str << std::endl;
    std::cerr << "expected: " << expected << std::endl;
    exit(1);
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

    assert(term3->to_string() == "SK");
    assert(term4->to_string() == "SK(SK)");
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
    assert_eval(ik, "K");

    // K
    auto kis = make_app(make_app(k, i), s);
    assert_eval(kis, "I");

    // S
    auto sski = make_app(make_app(make_app(s, s), k), i);
    assert_eval(sski, "SI(KI)");

    // SKI
    auto sksk = make_app(make_app(make_app(s, k), s), k);
    assert_eval(sksk, "K");

    auto i_ik_ = make_app(i, make_app(i, k));
    assert_eval(i_ik_, "K");

    auto ik_ik_ = make_app(ik, ik);
    assert_eval(ik_ik_, "KK");

    // Wikipedia examples
    auto ski_kis_ =
        make_app(make_app(make_app(s, k), i), make_app(make_app(k, i), s));
    assert_eval(ski_kis_, "I");

    auto ks_i_sksi__ = make_app(
        make_app(k, s), make_app(i, make_app(make_app(make_app(s, k), s), i)));
    assert_eval(ks_i_sksi__, "S");

    auto skik = make_app(make_app(make_app(s, k), i), k);
    assert_eval(skik, "K");
  }

  // SKI lib
  {
    // NOT
    assert_eval(make_app(NOT, TRUE), FALSE);
    assert_eval(make_app(NOT, FALSE), TRUE);
    assert_eval(make_not(TRUE), FALSE);
    assert_eval(make_not(FALSE), TRUE);

    // OR
    assert_eval(make_app(make_app(OR, FALSE), FALSE), FALSE);
    assert_eval(make_app(make_app(OR, FALSE), TRUE), TRUE);
    assert_eval(make_app(make_app(OR, TRUE), FALSE), TRUE);
    assert_eval(make_app(make_app(OR, TRUE), TRUE), TRUE);
    assert_eval(make_or(FALSE, FALSE), FALSE);
    assert_eval(make_or(FALSE, TRUE), TRUE);
    assert_eval(make_or(TRUE, FALSE), TRUE);
    assert_eval(make_or(TRUE, TRUE), TRUE);

    // AND
    assert_eval(make_app(make_app(AND, FALSE), FALSE), FALSE);
    assert_eval(make_app(make_app(AND, FALSE), TRUE), FALSE);
    assert_eval(make_app(make_app(AND, TRUE), FALSE), FALSE);
    assert_eval(make_app(make_app(AND, TRUE), TRUE), TRUE);
    assert_eval(make_and(FALSE, FALSE), FALSE);
    assert_eval(make_and(FALSE, TRUE), FALSE);
    assert_eval(make_and(TRUE, FALSE), FALSE);
    assert_eval(make_and(TRUE, TRUE), TRUE);
  }

  return 0;
}