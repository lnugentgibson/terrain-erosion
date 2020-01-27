#ifndef GRAPHICS_IMAGE_PIXEL_H
#define GRAPHICS_IMAGE_PIXEL_H

#include <iostream>

namespace graphics {
namespace image {

struct DataSpecifier {
  int dim;
  size_t element_size;
  DataSpecifier() {}
  DataSpecifier(size_t _element_size, int _dim = 1) : dim(_dim), element_size(_element_size) {}
  size_t PixelSize() const {
    return dim * element_size;
  }
  void readDim(std::istream *is) {
    is->read((char *) &dim, sizeof(int));
  }
  void writeDim(std::ostream *os) {
    os->write((char *) &dim, sizeof(int));
  }
};

struct InputSpecifier {
  DataSpecifier data;
  std::istream *is;
  InputSpecifier(std::istream *_is, size_t element_size, int dim = 1) : data(element_size, dim), is(_is) {}
  InputSpecifier(std::istream *_is, DataSpecifier spec) : data(spec), is(_is) {}
  void readDim() {
    data.readDim(is);
  }
  void read(int *rows, int *cols) {
    is->read((char *) rows, sizeof(int));
    is->read((char *) cols, sizeof(int));
    data.readDim(is);
  }
};

struct OutputSpecifier {
  DataSpecifier data;
  std::ostream *os;
  OutputSpecifier(std::ostream *_os, size_t element_size, int dim = 1) : data(element_size, dim), os(_os) {}
  OutputSpecifier(std::ostream *_os, DataSpecifier spec) : data(spec), os(_os) {}
  void writeDim() {
    data.writeDim(os);
  }
  void write(int *rows, int *cols) {
    os->write((char *) rows, sizeof(int));
    os->write((char *) cols, sizeof(int));
    data.writeDim(os);
  }
};

struct PixelSpecifier {
  DataSpecifier data;
  char *pixel;
  PixelSpecifier() {}
  PixelSpecifier(DataSpecifier spec) : data(spec) {}
  PixelSpecifier(InputSpecifier in_spec) : data(in_spec.data) {}
  PixelSpecifier(OutputSpecifier out_spec) : data(out_spec.data) {}
  size_t PixelSize() const {
    return data.PixelSize();
  }
  void allocate() {
    pixel = new char[data.PixelSize()];
  }
  void deallocate() {
    delete[] pixel;
  }
  void read(std::istream *is) {
    is->read(pixel, data.PixelSize());
  }
  void read(InputSpecifier in_spec) {
    in_spec.is->read(pixel, data.PixelSize());
  }
  void write(std::ostream *os) {
    os->write(pixel, data.PixelSize());
  }
  void write(OutputSpecifier out_spec) {
    out_spec.os->write(pixel, data.PixelSize());
  }
};

} // namespace image
} // namespace graphics

#endif // GRAPHICS_IMAGE_PIXEL_H