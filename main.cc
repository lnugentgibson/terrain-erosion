#include <array>
#include <vector>
#include <ctime>
#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>

class GrayscaleComponent {
 public:
  float r(float *c) {
    return *c;
  }

  float g(float *c) {
    return *c;
  }

  float b(float *c) {
    return *c;
  }
};

template<size_t d>
class ArrayComponent {
 public:
  float r(std::array<float, d> *c) {
    return (*c)[0];
  }

  float g(std::array<float, d> *c) {
    return d > 1 ? (*c)[1] : 0.0;
  }

  float b(std::array<float, d> *c) {
    return d > 2 ? (*c)[2] : 0.0;
  }
};

class Color {
  float r_, g_, b_;
 public:
  Color() : Color(0, 0, 0) {}
  Color(float r, float g, float b): r_(r), g_(g), b_(b) {}
  float r() {
    return r_;
  }
  void r(float _r) {
    r_ = _r;
  }
  float g() {
    return g_;
  }
  void g(float _g) {
    g_ = _g;
  }
  float b() {
    return b_;
  }
  void b(float _b) {
    b_ = _b;
  }
  void print() const {
    std::cout << "Color(" << r_ << ", " << g_ << ", " << b_ << ")" << std::endl;
  }
};

class ColorComponent {
 public:
  float r(Color *c) {
    return c->r();
  }

  float g(Color *c) {
    return c->g();
  }

  float b(Color *c) {
    return c->b();
  }
};

template<typename T, size_t rows, size_t cols, typename components>
class Grid {
  std::array<std::array<T *, cols>, rows> data;
  components getter;
  T defaultValue;
 public:
  Grid() {
    for(int i = 0; i < rows; i++)
      for(int j = 0; j < cols; j++)
        data[i][j] = 0;
  }
  /*
  std::array<T *, cols>& operator [] (int i) {
    return data[i];
  }
  */
  T& get(int i, int j) {
    if(i < 0) {
      std::cout << "get(" << i << ", " << j << "), i < 0 (" << i << " < 0)" << std::endl;
      return defaultValue;
    }
    if(i >= rows) {
      std::cout << "get(" << i << ", " << j << "), i >= rows (" << i << " >= " << rows << ")" << std::endl;
      return defaultValue;
    }
    if(j < 0) {
      std::cout << "get(" << i << ", " << j << "), j < 0 (" << j << " < 0)" << std::endl;
      return defaultValue;
    }
    if(j >= cols) {
      std::cout << "get(" << i << ", " << j << "), j >= cols (" << j << " >= " << cols << ")" << std::endl;
      return defaultValue;
    }
    /*
    if(data[i][j] == 0) {
      std::cout << "get(" << i << ", " << j << ") = 0";
      return defaultValue;
    }
    //*/
    return *data[i][j];
  }
  T& set(int i, int j, T v) {
    if(data[i][j] == 0)
      data[i][j] = new T(v);
    else
      *data[i][j] = v;
    return *data[i][j];
  }
  void save(char *fn) {
    std::ofstream ofs(fn, std::ios::out | std::ios::binary);
    ofs << "P6\n" << cols << " " << rows << "\n255\n";
    for (unsigned i = 0; i < rows; ++i)
      for (unsigned j = 0; j < cols; ++j) {
        ofs << (unsigned char)(std::max(float(0), std::min(float(1), getter.r(data[i][j]))) * 255) <<
               (unsigned char)(std::max(float(0), std::min(float(1), getter.g(data[i][j]))) * 255) <<
               (unsigned char)(std::max(float(0), std::min(float(1), getter.b(data[i][j]))) * 255);
      }
    ofs.close();
  }
};

/*
template<size_t rows, size_t cols>
std::unique_ptr<Grid<float, rows, cols>> randGrid() {
  std::unique_ptr<Grid<float, rows, cols>> rg(new Grid<float, rows, cols>());
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      rg->set(i, j, g);
    }
  return std::move(rg);
}
//*/

template<size_t rows, size_t cols, size_t d, typename components = ArrayComponent<d>>
std::unique_ptr<Grid<std::array<float, d>, rows, cols, components>> randGridVector() {
  std::unique_ptr<Grid<std::array<float, d>, rows, cols, components>>
    rg(new Grid<std::array<float, d>, rows, cols, components>());
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      std::array<float, d> v;
      for(int k = 0; k < d; k++) v[k] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      rg->set(i, j, v);
    }
  return std::move(rg);
}

template<size_t rows, size_t cols, size_t d, typename components = ArrayComponent<d>>
std::unique_ptr<Grid<std::array<float, d>, rows, cols, components>> randGridDirection() {
  std::unique_ptr<Grid<std::array<float, d>, rows, cols, components>>
    rg(new Grid<std::array<float, d>, rows, cols, components>());
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      std::array<float, d> v;
      float l = 0;
      for(int k = 0; k < d; k++) {
        v[k] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5;
        l += v[k] * v[k];
      }
      l = sqrt(l);
      for(int k = 0; k < d; k++) {
        v[k] = v[k] / l;
      }
      rg->set(i, j, v);
    }
  return std::move(rg);
}

/*
template<size_t rows, size_t cols>
std::unique_ptr<Grid<Color, rows, cols>> randGridColor() {
  std::unique_ptr<Grid<Color, rows, cols>> rg(new Grid<Color, rows, cols>());
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      rg->set(i, j, Color(r, g, b));
    }
  return std::move(rg);
}
//*/

float curve3(float x) {
  return x * x * (-x * 2 + 3);
}

/*
template<size_t base_rows, size_t base_cols, size_t cell_size>
std::unique_ptr<Grid<float, base_rows * cell_size, base_cols * cell_size>> perlinGrid() {
  auto rg = randGrid<base_rows + 1, base_cols + 1>();
  std::unique_ptr<Grid<float, base_rows * cell_size, base_cols * cell_size>>
    pg(new Grid<float, base_rows * cell_size, base_cols * cell_size>());
  for(size_t i = 0; i < base_rows; i++)
    for(size_t j = 0; j < base_cols; j++) {
      float nn = rg->get(i, j);
      float xn = rg->get(i, j + 1);
      float nx = rg->get(i + 1, j);
      float xx = rg->get(i + 1, j + 1);
      for(size_t _i = 0; _i < cell_size; _i++) {
        float y = curve3((_i + 0.5) / cell_size);
        float _y = 1 - y;
        for(size_t _j = 0; _j < cell_size; _j++) {
          float x = curve3((_j + 0.5) / cell_size);
          float _x = 1 - x;
          pg->set(i * cell_size + _i, j * cell_size + _j, _x * _y * nn + x * _y * xn + _x * y * nx + x * y * xx);
        }
      }
    }
  return pg;
}
//*/

template<size_t base_rows, size_t base_cols, size_t cell_size, typename components>
void perlinGradientGrid(Grid<float, base_rows * cell_size, base_cols * cell_size, components>& pg) {
  auto rg = randGridDirection<base_rows + 1, base_cols + 1, 2>();
  for(size_t i = 0; i < base_rows; i++)
    for(size_t j = 0; j < base_cols; j++) {
      auto& nn = rg->get(i, j);
      auto& xn = rg->get(i, j + 1);
      auto& nx = rg->get(i + 1, j);
      auto& xx = rg->get(i + 1, j + 1);
      for(size_t _i = 0; _i < cell_size; _i++) {
        float y = (_i + 0.5) / cell_size;
        float _y = 1 - y;
        float Y = curve3(y);
        float _Y = 1 - Y;
        for(size_t _j = 0; _j < cell_size; _j++) {
          float x = (_j + 0.5) / cell_size;
          float _x = 1 - x;
          float X = curve3(x);
          float _X = 1 - X;
          float nnv = nn[0] * x + nn[1] * y;
          float xnv = -xn[0] * _x + xn[1] * y;
          float nxv = nx[0] * x - nx[1] * _y;
          float xxv = -xx[0] * _x - xx[1] * _y;
          pg.set(
            i * cell_size + _i,
            j * cell_size + _j,
            (
              _X * _Y * nnv + X * _Y * xnv + _X * Y * nxv + X * Y * xxv
              //x > 0.5 ? (y > 0.5 ? xxv : xnv) : (y > 0.5 ? nxv : nnv)
            ) * 0.5 + 0.5
          );
        }
      }
    }
}

template<size_t rows, size_t cols, typename components>
std::pair<float, float> range(Grid<float, rows, cols, components>& grid) {
  float n, x;
  n = x = grid.get(0, 0);
  for(size_t i = 0; i < rows; i++)
    for(size_t j = 0; j < cols; j++) {
      float v = grid.get(i, j);
      if(v < n) n = v;
      if(v > x) x = v;
    }
  return std::pair<float, float>(n, x);
}

template<size_t rows, size_t cols, typename components>
void map(Grid<float, rows, cols, components>& grid, float fn, float fx, float tn, float tx) {
  float fr = fx - fn;
  float tr = tx - tn;
  for(size_t i = 0; i < rows; i++)
    for(size_t j = 0; j < cols; j++) {
      grid.set(i, j, (grid.get(i, j) - fn) / fr * tr + tn);
    }
}

template<size_t rows, size_t cols, typename components>
void strech(Grid<float, rows, cols, components>& grid) {
  auto r = range(grid);
  map(grid, r.first, r.second, 0.0, 1.0);
}

template<size_t rows, size_t cols, typename components>
void transfer(
  Grid<float, rows, cols, components>& a,
  Grid<float, rows, cols, components>& b
) {
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      b.set(i, j, a.get(i, j));
    }
}

template<size_t rows, size_t cols, typename components>
void weightedSum(
  Grid<float, rows, cols, components>& g,
  std::vector<Grid<float, rows, cols, components>>& grids,
  std::vector<float>& weights
) {
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      g.set(i, j, 0.0);
    }
  for(int k = 0; k < grids.size(); k++)
    for(int i = 0; i < rows; i++)
      for(int j = 0; j < cols; j++) {
        g.set(i, j, g.get(i, j) + grids[k].get(i, j) * weights[k]);
      }
}

template<size_t rows, size_t cols, typename components>
void weightedSum(
  Grid<float, rows, cols, components>& a,
  float wa,
  Grid<float, rows, cols, components>& b,
  float wb,
  Grid<float, rows, cols, components>& c
) {
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      c.set(i, j, a.get(i, j) * wa + b.get(i, j) * wb);
    }
}

template<size_t rows, size_t cols, typename components>
void add(
  Grid<float, rows, cols, components>& a,
  Grid<float, rows, cols, components>& b,
  float w = 1.0
) {
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      a.set(i, j, a.get(i, j) + b.get(i, j) * w);
    }
}

class NormalComponent {
 public:
  float r(std::array<float, 3> *c) {
    return (*c)[0] * 0.5 + 0.5;
  }

  float g(std::array<float, 3> *c) {
    return (*c)[1] * 0.5 + 0.5;
  }

  float b(std::array<float, 3> *c) {
    return (*c)[2] * 0.5 + 0.5;
  }
};

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

template<size_t rows, size_t cols, typename cg, typename cc>
void grayToColor(Grid<float, rows, cols, cg>& gray, Grid<Color, rows, cols, cc>& col) {
  for(size_t i = 0; i < rows; i++)
    for(size_t j = 0; j < cols; j++) {
      float g = gray.get(i, j);
      col.set(i, j, Color(g, g, g));
    }
}

void fractal_perlin(Grid<float, 512, 512, GrayscaleComponent>& fpg) {
  Grid<float, 512, 512, GrayscaleComponent> perlin;
  std::vector<float> weights;
  /*
  for(int s = 256, i = 0; s > 0; s /= 2) {
    srand(time(NULL));
    grids.emplace_back(Grid<float, 256, 256, GrayscaleComponent>(), s / 256.0);
    perlinGradientGrid<256 / s, 256 / s, s>(grids[i].first);
    strech(grids[i].first);
    map(grids[i].first, 0., 1., -1.0, 1.0);
    i++;
  }
  //*/
  //*
  srand(time(NULL));
  perlinGradientGrid<1, 1, 512>(perlin);
  strech(perlin);
  map(perlin, 0., 1., -1.0, 1.0);
  transfer(perlin, fpg);
  //srand(time(NULL));
  perlinGradientGrid<2, 2, 256>(perlin);
  strech(perlin);
  map(perlin, 0., 1., -1.0, 1.0);
  add(fpg, perlin, 0.5);
  //srand(time(NULL));
  perlinGradientGrid<4, 4, 128>(perlin);
  strech(perlin);
  map(perlin, 0., 1., -1.0, 1.0);
  add(fpg, perlin, 0.25);
  //*
  //srand(time(NULL));
  perlinGradientGrid<8, 8, 64>(perlin);
  strech(perlin);
  map(perlin, 0., 1., -1.0, 1.0);
  add(fpg, perlin, 0.125);
  //srand(time(NULL));
  perlinGradientGrid<16, 16, 32>(perlin);
  strech(perlin);
  map(perlin, 0., 1., -1.0, 1.0);
  add(fpg, perlin, 0.0625);
  //srand(time(NULL));
  perlinGradientGrid<32, 32, 16>(perlin);
  strech(perlin);
  map(perlin, 0., 1., -1.0, 1.0);
  add(fpg, perlin, 0.03125);
  //srand(time(NULL));
  perlinGradientGrid<64, 64, 8>(perlin);
  strech(perlin);
  map(perlin, 0., 1., -1.0, 1.0);
  add(fpg, perlin, 0.015625);
  //*/
  strech(fpg);
  fpg.save((char *) "fractal_perlin.ppm");
}

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