#include <iostream>
#include <memory>
#include <string>

enum TermType {
  S,   // Atom S
  K,   // Atom K
  I,   // Atom I
  App, // Application
};

class Term;
using TermPtr = std::shared_ptr<Term>;

class Term {
public:
  TermType type;
  // Only for type == TermType::App
  const std::shared_ptr<Term> lhs, rhs;

  Term(TermType type) : type(type) {
    if (type == TermType::App) {
      std::cerr << "Term construction with TermType::App is not supported."
                << std::endl;
      exit(1);
    }
  }

  Term(TermPtr lhs, TermPtr rhs) : type(TermType::App), lhs(lhs), rhs(rhs) {}
};

TermPtr copy_term(TermPtr term) {
  if (term->type == TermType::App) {
    auto lhs = copy_term(term->lhs);
    auto rhs = copy_term(term->rhs);
    return std::make_shared<Term>(lhs, rhs);
  }

  return std::make_shared<Term>(term->type);
}

// Create a new atom term.
TermPtr make_term(TermType type) { return std::make_shared<Term>(type); }

// Create a new application term from copies of lhs and rhs terms.
TermPtr make_term(TermPtr lhs, TermPtr rhs) {
  lhs = copy_term(lhs);
  rhs = copy_term(rhs);
  return std::make_shared<Term>(lhs, rhs);
}

// Wrapper functions for `make_term`.
TermPtr make_s() { return make_term(TermType::S); }
TermPtr make_k() { return make_term(TermType::K); }
TermPtr make_i() { return make_term(TermType::I); }
TermPtr make_app(TermPtr lhs, TermPtr rhs) { return make_term(lhs, rhs); }

std::string term_to_str(TermPtr term) {
  switch (term->type) {
  case TermType::S:
    return "S";
  case TermType::K:
    return "K";
  case TermType::I:
    return "I";
  }

  const auto lstr = term_to_str(term->lhs);
  const auto rstr = term_to_str(term->rhs);

  if (term->rhs->type == TermType::App) {
    return lstr + "(" + rstr + ")";
  } else {
    return lstr + rstr;
  }
}