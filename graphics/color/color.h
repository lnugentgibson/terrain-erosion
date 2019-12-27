#ifndef COLOR_H
#define COLOR_H

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

#endif // COLOR_H