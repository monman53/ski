#ifndef __SKI_HPP_
#define __SKI_HPP_

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
  // Only for `type == TermType::App`
  std::shared_ptr<Term> lhs, rhs;

  Term(TermType type) : type(type) {
    if (type == TermType::App) {
      std::cerr << "Term construction with TermType::App is not supported."
                << std::endl;
      exit(1);
    }
  }

  Term(TermPtr lhs, TermPtr rhs) : type(TermType::App), lhs(lhs), rhs(rhs) {}

  std::string to_string() {
    switch (type) {
    case TermType::S:
      return "S";
    case TermType::K:
      return "K";
    case TermType::I:
      return "I";
    }

    const auto lstr = lhs->to_string();
    const auto rstr = rhs->to_string();

    if (rhs->type == TermType::App) {
      return lstr + "(" + rstr + ")";
    } else {
      return lstr + rstr;
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
  return term;
}

// TODO: Use stack rather than function call for optimization.
bool eval_(TermPtr term) {
  if (term->type == TermType::App) {
    // Ix -> x
    if (term->lhs->type == TermType::I) {
      auto x = term->rhs;
      term->type = x->type;
      term->lhs = x->lhs;
      term->rhs = x->rhs;
      return true;
    }

    // Kxy -> x
    if (term->lhs->type == TermType::App) {
      if (term->lhs->lhs->type == TermType::K) {
        auto x = term->lhs->rhs;
        term->type = x->type;
        term->lhs = x->lhs;
        term->rhs = x->rhs;
        return true;
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
          auto new_term = make_term(make_term(x, z), make_term(y, z));
          term->type = new_term->type;
          term->lhs = new_term->lhs;
          term->rhs = new_term->rhs;
          return true;
        }
      }
    }

    // Other
    return eval_(term->lhs) || eval_(term->rhs);
  }

  // Atoms
  return false;
}

TermPtr eval(TermPtr term) {
  while (eval_(term)) {
  };
  return term;
}

bool same(TermPtr a, TermPtr b) {
  if (a->type == TermType::App) {
    if (b->type == TermType::App) {
      return same(a->lhs, b->lhs) && same(a->rhs, b->rhs);
    } else {
      return false;
    }
  } else {
    return a->type == b->type;
  }
}

#endif