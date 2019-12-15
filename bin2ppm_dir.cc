#include <cstdlib>
#include <cstring>
#include <iostream>

#include "grid.h"
#include "random.h"

int main(int argc, char *argv[]) {
  if(argc < 3) {
    std::cout << "Must provide input and output." << std::endl;
    return 1;
  }
  auto grid = readBin<float, NormalComponent>(argv[1]);
  grid.savePpm(argv[2]);
  return 0;
}