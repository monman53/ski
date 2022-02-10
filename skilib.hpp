#include "ski.hpp"

// Basics
TermPtr make_s() { return make_term(TermType::S); }
TermPtr make_k() { return make_term(TermType::K); }
TermPtr make_i() { return make_term(TermType::I); }
TermPtr make_app(const TermPtr &lhs, const TermPtr &rhs) {
  return make_term(lhs, rhs);
}

// Boolean operators
auto TRUE = make_k();
auto FALSE = make_app(make_s(), make_k());

auto NOT = make_app(make_app(make_s(), make_app(make_app(make_s(), make_i()),
                                                make_app(make_k(), FALSE))),
                    make_app(make_k(), TRUE));
auto OR = make_app(make_app(make_s(), make_i()), make_app(make_k(), TRUE));
auto AND = make_app(make_app(make_s(), make_s()),
                    make_app(make_k(), make_app(make_k(), FALSE)));

TermPtr make_not(const TermPtr &term) { return make_app(NOT, term); };
TermPtr make_or(const TermPtr &a, const TermPtr &b) {
  return make_app(make_app(OR, a), b);
};
TermPtr make_and(const TermPtr &a, const TermPtr &b) {
  return make_app(make_app(AND, a), b);
};