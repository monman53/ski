#include "ski.hpp"

// Boolean operators
auto TRUE = make_k();
auto FALSE = make_app(make_s(), make_k());

auto NOT = make_app(make_app(make_s(), make_app(make_app(make_s(), make_i()),
                                                make_app(make_k(), FALSE))),
                    make_app(make_k(), TRUE));
auto OR = make_app(make_app(make_s(), make_i()), make_app(make_k(), TRUE));
auto AND = make_app(make_app(make_s(), make_s()),
                    make_app(make_k(), make_app(make_k(), FALSE)));