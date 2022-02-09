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

  return 0;
}