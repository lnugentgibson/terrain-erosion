#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"
#include "graphics/image/binary/util.h"
#include "graphics/noise/random.h"
#include "graphics/noise/noise.h"

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
    PerlinValueGenerator generator(cell_size);
    graphics::image::binary::GenerateStateful(rows, cols, 1, sizeof(float), ofs, &generator, &init);
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
    graphics::image::binary::ScalingTransformer transformer(init.min_val, init.max_val, -1.0, 1.0);
    graphics::image::binary::Map(sizeof(float), ifsc, sizeof(float), 1, ofsc, &transformer);
    ifsc.close();
    ofsc.close();
  }
  return 0;
}