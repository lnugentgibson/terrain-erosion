#include <cstdlib>
#include <cstring>
#include <iostream>

#include "grid.h"
#include "random.h"

int main(int argc, char *argv[]) {
  if(argc < 5) {
    std::cout << "Must provide width and height." << std::endl;
    return 1;
  }
  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);
  int dim = atoi(argv[3]);
  bool bin = false;
  if(argc > 5) {
    bin = !!atoi(argv[5]);
  }
  char *filename = new char[strlen(argv[4]) + 5];
  strcpy(filename, argv[4]);
  strcat(filename, bin ? (char *) ".bin" : (char *) ".ppm");
  Grid<float, NormalComponent> grid(rows, cols, dim);
  grid.allocate();
  randGridDirection(grid);
  //grid.print();
  if(bin) {
    grid.saveBin(filename);
  } else {
    grid.savePpm(filename);
  }
  return 0;
}