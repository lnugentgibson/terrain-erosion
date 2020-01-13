#ifndef GRAPHICS_COLORS_COLOR_H
#define GRAPHICS_COLORS_COLOR_H

class Color {
  float r_, g_, b_;
 public:
  Color();
  Color(float r, float g, float b);
  float r() const;
  void r(float _r);
  float g() const;
  void g(float _g);
  float b() const;
  void b(float _b);
  void print() const;
};

#endif // GRAPHICS_COLORS_COLOR_H