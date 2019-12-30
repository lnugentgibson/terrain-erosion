#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"
#include "graphics/noise/random.h"
#include "graphics/noise/noise.h"

struct PerlinState {
  float *row_a;
  float *row_b;
  int cur;
  int row;
  float min_val;
  float max_val;
};

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a color image from binary to ppm");
  options.add_options()
    ("w,width", "Width of image", cxxopts::value<int>())
    ("h,height", "Height of image", cxxopts::value<int>())
    ("e,scale", "Height of image", cxxopts::value<float>())
    ("s,seed", "random seed", cxxopts::value<int>())
    ("c,count", "number of image", cxxopts::value<int>())
    //("i,input", "first binary image file path", cxxopts::value<std::string>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  int cols = result["w"].as<int>();
  int rows = result["h"].as<int>();
  float cell_size = result["e"].as<float>();
  int seed = result["s"].as<int>();
  if(seed < 0) {
    seed = time(NULL);
  }
  srand(seed);
  int count = result["c"].as<int>();
  for(int i = 0; i < count; i++) {
    std::stringstream fs;
    fs << result["o"].as<std::string>();
    fs << "_" << std::setfill('0') << std::setw(4) << rows;
    fs << "_" << std::setfill('0') << std::setw(4) << cols;
    fs << "_" << std::setprecision(5) << std::fixed << std::scientific << cell_size;
    fs << "_" << std::setfill('0') << std::setw(4) << seed;
    fs << "_" << std::setfill('0') << std::setw(3) << i;
    fs << ".bin";
    std::string filename = fs.str();
    std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    PerlinState init;
    int row_size = static_cast<int>(ceil(cols / cell_size + 1));
    init.row_a = new float[2 * row_size];
    init.row_b = new float[2 * row_size];
    init.cur = 1;
    for(int i = 0; i < row_size; i++) {
      float x = randGray() * 2 - 1;
      float y = randGray() * 2 - 1;
      float m = sqrt(x * x + y * y);
      init.row_a[2 * i] = x / m;
      init.row_a[2 * i + 1] = y / m;
    }
    init.row = -1;
    generateStatefulBin<PerlinState>(rows, cols, 1, sizeof(float), ofs, [cell_size, row_size](int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size, PerlinState *state) -> void {
      int row = static_cast<int>(floor(i / cell_size));
      int col = static_cast<int>(floor(j / cell_size));
      if(j == 0 && row != state->row) {
        float *row = state->cur == 0 ? state->row_a : state->row_b;
        for(int i = 0; i < row_size; i++) {
          float x = randGray() * 2 - 1;
          float y = randGray() * 2 - 1;
          float m = sqrt(x * x + y * y);
          row[2 * i] = x / m;
          row[2 * i + 1] = y / m;
        }
        state->cur = 1 - state->cur;
      }
      state->row = row;
      float *base_row = state->cur == 0 ? state->row_a : state->row_b;
      float *next_row = state->cur == 1 ? state->row_a : state->row_b;
      float *nn = base_row + 2 * col;
      float *xn = base_row + 2 * col + 2;
      float *nx = next_row + 2 * col;
      float *xx = next_row + 2 * col + 2;
      float y = curve3((i - row * cell_size) / cell_size);
      float _y = 1 - y;
      float Y = curve3(y);
      float _Y = 1 - Y;
      float x = curve3((j - col * cell_size) / cell_size);
      float _x = 1 - x;
      float X = curve3(x);
      float _X = 1 - X;
      float nnv = nn[0] * x + nn[1] * y;
      float xnv = -xn[0] * _x + xn[1] * y;
      float nxv = nx[0] * x - nx[1] * _y;
      float xxv = -xx[0] * _x - xx[1] * _y;
      float v = _X * _Y * nnv + X * _Y * xnv + _X * Y * nxv + X * Y * xxv;
      if(i == 0 && j == 0) {
        state->min_val = v;
        state->max_val = v;
      } else {
        if(v < state->min_val) state->min_val = v;
        if(v > state->max_val) state->max_val = v;
      }
      *static_cast<float *>(pixel) = v;
      //*static_cast<float *>(pixel) = nn[0] * 0.5 + 0.5;
    }, &init);
    ofs.close();
    std::stringstream fsc;
    fsc << result["o"].as<std::string>();
    fsc << "_" << std::setfill('0') << std::setw(4) << rows;
    fsc << "_" << std::setfill('0') << std::setw(4) << cols;
    fsc << "_" << std::setprecision(5) << std::fixed << std::scientific << cell_size;
    fsc << "_" << std::setfill('0') << std::setw(4) << seed;
    fsc << "_" << std::setfill('0') << std::setw(3) << i;
    fsc << "_c.bin";
    std::string filenamec = fsc.str();
    std::ifstream ifsc(filename, std::ios::in | std::ios::binary);
    std::ofstream ofsc(filenamec, std::ios::out | std::ios::binary);
    float range = init.max_val - init.min_val;
    mapBin(sizeof(float), ifsc, sizeof(float), 1, ofsc, [init, range](int i, int j, int rows, int cols, void *pixel1, int dim1, size_t element_size1, void *pixel2, int dim2, size_t element_size2) -> void {
      *static_cast<float *>(pixel2) = (static_cast<float *>(pixel1)[0] - init.min_val) / range * 2.0 - 1.0;
    });
    ifsc.close();
    ofsc.close();
  }
  return 0;
}