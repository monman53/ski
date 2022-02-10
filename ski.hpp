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
using TermPtr = std::unique_ptr<Term>;

TermPtr copy_term(const TermPtr &term);

class Term : public std::enable_shared_from_this<Term> {
public:
  TermType type;
  // Only for `type == TermType::App`
  TermPtr lhs, rhs;

  Term(TermType type) : type(type) {
    if (type == TermType::App) {
      std::cerr << "Term construction with TermType::App is not supported."
                << std::endl;
      exit(1);
    }
  }

  Term(TermPtr &lhs, TermPtr &rhs)
      : type(TermType::App), lhs(copy_term(lhs)), rhs(copy_term(rhs)) {}

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
TermPtr copy_term(const TermPtr &term) {
  if (term->type == TermType::App) {
    auto lhs = copy_term(term->lhs);
    auto rhs = copy_term(term->rhs);
    return std::make_unique<Term>(lhs, rhs);
  }

  return std::make_unique<Term>(term->type);
}

// Create a new atom term.
TermPtr make_term(TermType type) { return std::make_unique<Term>(type); }

// Create a new application term from copies of lhs and rhs terms.
TermPtr make_term(const TermPtr &lhs, const TermPtr &rhs) {
  auto lhs_ = copy_term(lhs);
  auto rhs_ = copy_term(rhs);
  auto term = std::make_unique<Term>(lhs_, rhs_);
  return term;
}

// TODO: Use stack rather than function call for optimization.
bool eval_(const TermPtr &term) {
  if (term->type == TermType::App) {
    // Ix -> x
    if (term->lhs->type == TermType::I) {
      auto x = std::move(term->rhs);
      term->type = x->type;
      term->lhs = std::move(x->lhs);
      term->rhs = std::move(x->rhs);
      return true;
    }

    // Kxy -> x
    if (term->lhs->type == TermType::App) {
      if (term->lhs->lhs->type == TermType::K) {
        auto x = std::move(term->lhs->rhs);
        term->type = x->type;
        term->lhs = std::move(x->lhs);
        term->rhs = std::move(x->rhs);
        return true;
      }
    }

    // Sxyz -> xz(yz)
    if (term->lhs->type == TermType::App) {
      if (term->lhs->lhs->type == TermType::App) {
        if (term->lhs->lhs->lhs->type == TermType::S) {
          auto x = std::move(term->lhs->lhs->rhs);
          auto y = std::move(term->lhs->rhs);
          auto z = std::move(term->rhs);
          // TODO: Optimize here. Only one copy of z is needed.
          auto xz = make_term(x, z);
          auto yz = make_term(y, z);
          auto new_term = make_term(xz, yz);
          term->type = new_term->type;
          term->lhs = std::move(new_term->lhs);
          term->rhs = std::move(new_term->rhs);
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

TermPtr &eval(TermPtr &term) {
  while (eval_(term)) {
  };
  return term;
}

bool same(const TermPtr &a, const TermPtr &b) {
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