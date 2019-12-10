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
  Color(float r, float g, float b): r_(r), g_(g), b_(b) {}
  float r() {
    return r_;
  }
  float g() {
    return g_;
  }
  float b() {
    return b_;
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
        ofs << (unsigned char)(std::min(float(1), getter.r(data[i][j])) * 255) <<
               (unsigned char)(std::min(float(1), getter.g(data[i][j])) * 255) <<
               (unsigned char)(std::min(float(1), getter.b(data[i][j])) * 255);
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
std::unique_ptr<Grid<float, rows, cols, components>> weightedSum(
  std::vector<std::pair<Grid<float, rows, cols, components>, float>>& grids
) {
  std::unique_ptr<Grid<float, rows, cols, components>> g(new Grid<float, rows, cols, components>());
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      g->set(i, j, 0.0);
    }
  for(auto gw : grids)
    for(int i = 0; i < rows; i++)
      for(int j = 0; j < cols; j++) {
        g->set(i, j, g->get(i, j) + gw.first.get(i, j) * gw.second);
      }
  return g;
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

void fractal_perlin() {
  std::vector<std::pair<Grid<float, 256, 256, GrayscaleComponent>, float>> grids;
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
  grids.emplace_back(Grid<float, 256, 256, GrayscaleComponent>(), 1.0);
  perlinGradientGrid<1, 1, 256>(grids[0].first);
  strech(grids[0].first);
  map(grids[0].first, 0., 1., -1.0, 1.0);
  //srand(time(NULL));
  grids.emplace_back(Grid<float, 256, 256, GrayscaleComponent>(), 0.5);
  perlinGradientGrid<2, 2, 128>(grids[1].first);
  strech(grids[1].first);
  map(grids[1].first, 0., 1., -1.0, 1.0);
  //srand(time(NULL));
  grids.emplace_back(Grid<float, 256, 256, GrayscaleComponent>(), 0.25);
  perlinGradientGrid<4, 4, 64>(grids[2].first);
  strech(grids[2].first);
  map(grids[2].first, 0., 1., -1.0, 1.0);
  //srand(time(NULL));
  grids.emplace_back(Grid<float, 256, 256, GrayscaleComponent>(), 0.125);
  perlinGradientGrid<8, 8, 32>(grids[3].first);
  strech(grids[3].first);
  map(grids[3].first, 0., 1., -1.0, 1.0);
  //srand(time(NULL));
  grids.emplace_back(Grid<float, 256, 256, GrayscaleComponent>(), 0.0625);
  perlinGradientGrid<16, 16, 16>(grids[4].first);
  strech(grids[4].first);
  map(grids[4].first, 0., 1., -1.0, 1.0);
  //srand(time(NULL));
  grids.emplace_back(Grid<float, 256, 256, GrayscaleComponent>(), 0.03125);
  perlinGradientGrid<32, 32, 8>(grids[5].first);
  strech(grids[5].first);
  map(grids[5].first, 0., 1., -1.0, 1.0);
  //*/
  auto fpg = weightedSum(grids);
  strech(*fpg);
  Grid<float, 256, 256, GrayscaleComponent> dx;
  Grid<float, 256, 256, GrayscaleComponent> dy;
  Grid<std::array<float, 3>, 256, 256, NormalComponent> normal;
  diff(*fpg, dx, dy, normal);
  auto r = range(dx);
  float R = fmax(fabs(r.first), r.second);
  map(dx, -R, R, 0.0, 1.0);
  r = range(dy);
  R = fmax(fabs(r.first), r.second);
  map(dy, -R, R, 0.0, 1.0);
  fpg->save((char *) "fractal_perlin.ppm");
  dx.save((char *) "fractal_perlin_dx.ppm");
  dy.save((char *) "fractal_perlin_dy.ppm");
  normal.save((char *) "fractal_perlin_normal.ppm");
  float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  float vx = 0, vy = 0;
}

int main() {
  fractal_perlin();
  return 0;
}