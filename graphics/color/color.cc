#include "color.h"

#include <iostream>

Color::Color() : Color(0, 0, 0) {}

Color::Color(float r, float g, float b) : r_(r), g_(g), b_(b) {}

float Color::r() const {
    return r_;
}

void Color::r(float _r) {
    r_ = _r;
}

float Color::g() const {
    return g_;
}

void Color::g(float _g) {
    g_ = _g;
}

float Color::b() const {
    return b_;
}

void Color::b(float _b) {
    b_ = _b;
}

void Color::print() const {
    std::cout << "Color(" << r_ << ", " << g_ << ", " << b_ << ")" << std::endl;
}