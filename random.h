#ifndef RANDOM_H
#define RANDOM_H

#include <array>
#include <cmath>

#include "grid.h"

template<typename C>
void randGrid(Grid<float, C>& grid) {
  for(int i = 0; i < grid.rows; i++)
    for(int j = 0; j < grid.cols; j++) {
      float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      grid.set(i, j, g);
    }
}

template<size_t d, typename C>
void randGridVector(Grid<float, C>& grid) {
  for(int i = 0; i < grid.rows; i++)
    for(int j = 0; j < grid.cols; j++) {
      std::array<float, d> v;
      for(int k = 0; k < d; k++) v[k] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      grid.set(i, j, v);
    }
}

template<size_t d, typename C>
void randGridDirection(Grid<float, C>& grid) {
  for(int i = 0; i < grid.rows; i++)
    for(int j = 0; j < grid.cols; j++) {
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
      grid.set(i, j, v);
    }
}

template<typename C>
void randGrid(Grid<Color, C>& grid) {
  for(int i = 0; i < grid.rows; i++)
    for(int j = 0; j < grid.cols; j++) {
      float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      grid.set(i, j, Color(r, g, b));
    }
}

#endif // RANDOM_H