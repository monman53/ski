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

class Term : public std::enable_shared_from_this<Term> {
public:
  TermType type;
  // Only for type == TermType::App
  std::shared_ptr<Term> lhs, rhs;
  std::weak_ptr<Term> parent;

  Term(TermType type) : type(type) {
    if (type == TermType::App) {
      std::cerr << "Term construction with TermType::App is not supported."
                << std::endl;
      exit(1);
    }
  }

  Term(TermPtr lhs, TermPtr rhs) : type(TermType::App), lhs(lhs), rhs(rhs) {}

  // TODO: Rename this method.
  void set_parent() {
    lhs->parent = weak_from_this();
    rhs->parent = weak_from_this();

    // TODO: Optimize here. This is only for reduction of S.
    if (lhs->type == TermType::App) {
      lhs->set_parent();
    }
    if (rhs->type == TermType::App) {
      rhs->set_parent();
    }
  }
};

// Create a new Term object from same structure as `term`.
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
  auto term = std::make_shared<Term>(lhs, rhs);
  term->set_parent();
  return term;
}

// Wrapper functions for `make_term`.
TermPtr make_s() { return make_term(TermType::S); }
TermPtr make_k() { return make_term(TermType::K); }
TermPtr make_i() { return make_term(TermType::I); }
TermPtr make_app(TermPtr lhs, TermPtr rhs) { return make_term(lhs, rhs); }

TermPtr eval(TermPtr term) {
  if (term->type == TermType::App) {
    // Ix -> x
    if (term->lhs->type == TermType::I) {
      auto x = term->rhs;
      return eval(x);
    }

    // Kxy -> x
    if (term->lhs->type == TermType::App) {
      if (term->lhs->lhs->type == TermType::K) {
        auto x = term->lhs->rhs;
        return eval(x);
      }
    }

    // Sxyz -> xz(yz)
    if (term->lhs->type == TermType::App) {
      if (term->lhs->lhs->type == TermType::App) {
        if (term->lhs->lhs->lhs->type == TermType::S) {
          auto x = term->lhs->lhs->rhs;
          auto y = term->lhs->rhs;
          auto z = term->rhs;
          // TODO: Optimize here. Only one copy of z is needed.
          return eval(make_app(make_app(x, z), make_app(y, z)));
        }
      }
    }

    // Other
    term->lhs = eval(term->lhs);
    term->rhs = eval(term->rhs);
    term->set_parent();
  }

  return term;
}

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