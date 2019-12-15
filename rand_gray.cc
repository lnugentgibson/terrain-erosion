#include <cstdlib>
#include <cstring>
#include <iostream>

#include "grid.h"
#include "random.h"

int main(int argc, char *argv[]) {
  if(argc < 4) {
    std::cout << "Must provide width and height." << std::endl;
    return 1;
  }
  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);
  bool bin = false;
  if(argc > 4) {
    bin = !!atoi(argv[4]);
  }
  char *filename = new char[strlen(argv[3]) + 5];
  strcpy(filename, argv[3]);
  strcat(filename, bin ? (char *) ".bin" : (char *) ".ppm");
  Grid<float, GrayscaleComponent> grid(rows, cols, 1);
  grid.allocate();
  randGrid(grid);
  if(bin) {
    grid.saveBin(filename);
  } else {
    grid.savePpm(filename);
  }
  return 0;
}