#ifndef NOISE_H
#define NOISE_H

#include "grid.h"

float curve1(float x) {
  return x;
}

float curve3(float x) {
  return x * x * (-x * 2 + 3);
}

float curve(float x, int d) {
  switch(d) {
    case 3:
      return x * x * (-x * 2 + 3);
    default:
      return x;
  }
}

template<typename C>
void perlinGrid(Grid<float, C>& grid, Grid<float, C>& r_grid, float cell_size) {
  float nn, xn, nx, xx;
  for(int i = 0; i < grid.rows / cell_size; i++) {
    xn = i >= r_grid.getFirst(i, 0);
    xx = i + 1 >= r_grid.getFirst(i + 1, 0);
    for(int j = 0; j < grid.cols / cell_size; j++) {
      nn = xn;
      xn = r_grid.getFirst(i, j + 1);
      nx = xx;
      xx = r_grid.getFirst(i + 1, j + 1);
      for(int _i = 0; _i < cell_size; _i++) {
        // float y = curve3((_i + 0.5) / cell_size);
        float y = curve3(_i / cell_size);
        float _y = 1 - y;
        for(size_t _j = 0; _j < cell_size; _j++) {
          // float x = curve3((_j + 0.5) / cell_size);
          float x = curve3(_j / cell_size);
          float _x = 1 - x;
          grid.setFirst(i * cell_size + _i, j * cell_size + _j, _x * _y * nn + x * _y * xn + _x * y * nx + x * y * xx);
        }
      }
    }
  }
}

template<typename C>
void perlinGradientGrid(Grid<float, C>& grid, Grid<float, C>& r_grid, float cell_size) {
  float *nn, *xn, *nx, *xx;
  for(int i = 0; i < grid.rows / cell_size; i++) {
    xn = r_grid.get(i, 0);
    xx = r_grid.get(i + 1, 0);
    for(int j = 0; j < grid.cols / cell_size; j++) {
      nn = xn;
      xn = r_grid.get(i, j + 1);
      nx = xx;
      xx = r_grid.get(i + 1, j + 1);
      for(int _i = 0; _i < cell_size; _i++) {
        float y = (_i + 0.5) / cell_size;
        float _y = 1 - y;
        float Y = curve3(y);
        float _Y = 1 - Y;
        for(int _j = 0; _j < cell_size; _j++) {
          float x = (_j + 0.5) / cell_size;
          float _x = 1 - x;
          float X = curve3(x);
          float _X = 1 - X;
          float nnv = nn[0] * x + nn[1] * y;
          float xnv = -xn[0] * _x + xn[1] * y;
          float nxv = nx[0] * x - nx[1] * _y;
          float xxv = -xx[0] * _x - xx[1] * _y;
          grid.setFirst(
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
}

#endif // NOISE_H