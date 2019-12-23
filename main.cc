#include <array>
#include <vector>
#include <ctime>
#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>

template<size_t rows, size_t cols, typename components>
void diff(Grid<float, rows, cols, components>& z,
  Grid<float, rows, cols, GrayscaleComponent>& xdiff,
  Grid<float, rows, cols, GrayscaleComponent>& ydiff,
  Grid<std::array<float, 3>, rows, cols, NormalComponent>& normal
) {
  int smoothing = 16;
  for(size_t i = 0; i < rows; i++)
    for(size_t j = 0; j < cols; j++) {
      float dx, dy;
      int nj = ((int) j) - smoothing;
      if(nj < 0) nj = 0;
      int xj = ((int) j) + smoothing;
      if(xj > cols - 1) xj = cols - 1;
      dx = (z.get(i, xj) - z.get(i, nj)) * cols / (xj - nj);
      int ni = ((int) i) - smoothing;
      if(ni < 0) ni = 0;
      int xi = ((int) i) + smoothing;
      if(xi > rows - 1) xi = rows - 1;
      dy = (z.get(xi, j) - z.get(ni, j)) * rows / (xi - ni);
      float g = dx * dx + dy * dy;
      float f = 2.0;
      float _g = sqrt(g * g / (f * f) + 1);
      float nx = -dx / (f * _g);
      float ny = -dy / (f * _g);
      float nz = 1.0 / _g;
      std::array<float, 3> n;
      n[0] = nx;
      n[1] = ny;
      n[2] = nz;
      xdiff.set(i, j, dx);
      ydiff.set(i, j, dy);
      normal.set(i, j, n);
    }
}

struct GridPath {
  size_t i;
  size_t j;
  float t;
  float x_start;
  float y_start;
  float x;
  float y;
  float vx_start;
  float vy_start;
  float vx;
  float vy;
  float ax;
  float ay;
  /*
  void print(const std::ostream& os) const {
    os << "i, j: " << i << ", " << j << std::endl;
  }
  //*/
  void print() const {
    std::cout << "i, j: " << i << ", " << j << std::endl;
  }
};

void calcNormal(Grid<float, 512, 512, GrayscaleComponent>& fpg, Grid<std::array<float, 3>, 512, 512, NormalComponent>& normal) {
  Grid<float, 512, 512, GrayscaleComponent> dx;
  Grid<float, 512, 512, GrayscaleComponent> dy;
  diff(fpg, dx, dy, normal);
  /*
  auto r = range(dx);
  float R = fmax(fabs(r.first), r.second);
  map(dx, -R, R, 0.0, 1.0);
  r = range(dy);
  R = fmax(fabs(r.first), r.second);
  map(dy, -R, R, 0.0, 1.0);
  dx.save((char *) "fractal_perlin_dx.ppm");
  dy.save((char *) "fractal_perlin_dy.ppm");
  normal.save((char *) "fractal_perlin_normal.ppm");
  //*/
}

int main() {
  Grid<float, 512, 512, GrayscaleComponent> fpg;
  fractal_perlin(fpg);
  //*
  Grid<std::array<float, 3>, 512, 512, NormalComponent> normal;
  //calcNormal(fpg, normal);
  //*/
  /*
  float mg = 1;
  GridPath cur;
  cur.x_start = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  cur.y_start = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  cur.i = (size_t) floor(cur.x_start * 512);
  cur.j = (size_t) floor(cur.y_start * 512);
  cur.x_start = cur.x_start * 512 - cur.i;
  cur.y_start = cur.y_start * 512 - cur.j;
  cur.vx_start = 0;
  cur.vy_start = 0;
  std::vector<GridPath> path;
  int iteration = 0;
  while(cur.i >= 0 && cur.i < 512 && cur.j >= 0 && cur.j < 512 && iteration < 1024) {
    auto n = normal.get(cur.i, cur.j);
    float c = n[2];
    float s = sqrt(1 - c * c);
    float p = sqrt(1 - c);
    float px = n[0] / p;
    float py = n[1] / p;
    float a = mg * s;
    cur.ax = a * px;
    cur.ay = a * py;
    float _dxp = cur.vx_start * cur.vx_start - 2 * cur.ax * (1 - cur.x_start);
    float dxp = sqrt(_dxp);
    float txp1 = -cur.vx_start + dxp, txp2 = -cur.vx_start - dxp;
    float _dyp = cur.vy_start * cur.vy_start - 2 * cur.ay * (1 - cur.y_start);
    float dyp = sqrt(_dyp);
    float typ1 = -cur.vx_start + dyp, typ2 = -cur.vy_start - dyp;
    float _dxn = cur.vx_start * cur.vx_start - 2 * cur.ax * cur.x_start;
    float dxn = sqrt(_dxn);
    float txn1 = -cur.vx_start + dxn, txn2 = -cur.vx_start - dxn;
    float _dyn = cur.vy_start * cur.vy_start - 2 * cur.ay * cur.y_start;
    float dyn = sqrt(_dyn);
    float tyn1 = -cur.vx_start + dyn, tyn2 = -cur.vy_start - dyn;
    int ni, nj;
    float t;
    bool tset = false;
    if(_dxp > 0) {
      if(txp1 > 0) {
        cur.t = txp1;
        ni = cur.i + 1;
        nj = cur.j;
        tset = true;
      }
      if(txp2 > 0 && (!tset || txp2 < t)) {
        cur.t = txp2;
        ni = cur.i + 1;
        nj = cur.j;
        tset = true;
      }
    }
    if(_dyp > 0) {
      if(typ1 > 0 && (!tset || typ1 < t)) {
        cur.t = typ1;
        ni = cur.i;
        nj = cur.j + 1;
        tset = true;
      }
      if(typ2 > 0 && (!tset || typ2 < t)) {
        cur.t = typ2;
        ni = cur.i;
        nj = cur.j + 1;
        tset = true;
      }
    }
    if(_dxn > 0) {
      if(txn1 > 0 && (!tset || txn1 < t)) {
        cur.t = txn1;
        ni = cur.i - 1;
        nj = cur.j;
        tset = true;
      }
      if(txn2 > 0 && (!tset || txn2 < t)) {
        cur.t = txn2;
        ni = cur.i - 1;
        nj = cur.j;
        tset = true;
      }
    }
    if(_dyn > 0) {
      if(tyn1 > 0 && (!tset || tyn1 < t)) {
        cur.t = tyn1;
        ni = cur.i;
        nj = cur.j - 1;
        tset = true;
      }
      if(tyn2 > 0 && (!tset || tyn2 < t)) {
        cur.t = tyn2;
        ni = cur.i;
        nj = cur.j - 1;
        tset = true;
      }
    }
    cur.vx = cur.vx_start + cur.ax * cur.t;
    cur.vy = cur.vy_start + cur.ay * cur.t;
    cur.x = cur.x_start + cur.vx_start * cur.t + cur.ax * cur.t * cur.t / 2.0;
    cur.y = cur.y_start + cur.vy_start * cur.t + cur.ay * cur.t * cur.t / 2.0;
    path.push_back(cur);
    cur.i = ni;
    cur.j = nj;
    cur.x_start = cur.x;
    cur.y_start = cur.y;
    cur.vx_start = cur.vx;
    cur.vy_start = cur.vy;
    iteration++;
  }
  Grid<Color, 512, 512, ColorComponent> fpgc;
  grayToColor(*fpg, fpgc);
  float step = 0.5;
  for(const auto& node : path) {
    //node.print(std::cout);
    //node.print();
    auto& c = fpgc.get(node.i, node.j);
    //c.print();
    c.r(c.r() * 0.5 + step);
    c.g(c.g() * 0.5);
    c.b(c.b() * 0.5);
    //c.print();
    fpgc.set(node.i, node.j, c);
  }
  fpgc.save((char *) "fractal_perlin_path.ppm");
  //*/
  return 0;
}