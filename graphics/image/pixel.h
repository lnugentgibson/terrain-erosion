#ifndef GRAPHICS_IMAGE_PIXEL_H
#define GRAPHICS_IMAGE_PIXEL_H

#include <iostream>

namespace graphics {
namespace image {

struct DataSpecifier {
  size_t element_size;
  int dim;

  DataSpecifier() {}

  DataSpecifier(size_t _element_size, int _dim = 1)
      : element_size(_element_size), dim(_dim) {}

  size_t PixelSize() const { return dim * element_size; }

  void read(std::istream *is) {
    is->read((char *)&element_size, sizeof(size_t));
    is->read((char *)&dim, sizeof(int));
  }

  void write(std::ostream *os) {
    os->write((char *)&element_size, sizeof(size_t));
    os->write((char *)&dim, sizeof(int));
  }
};

struct ImageDataSpecifier {
  DataSpecifier data;
  int rows, cols;

  ImageDataSpecifier() {}

  ImageDataSpecifier(size_t element_size, int dim = 1)
      : data(element_size, dim) {}

  ImageDataSpecifier(DataSpecifier spec) : data(spec) {}

  ImageDataSpecifier(int r, int c, size_t element_size, int dim = 1)
      : data(element_size, dim), rows(r), cols(c) {}

  ImageDataSpecifier(int r, int c, DataSpecifier spec)
      : data(spec), rows(r), cols(c) {}

  size_t PixelSize() const { return data.PixelSize(); }

  size_t PixelsSize() const { return data.PixelSize() * rows * cols; }

  void read(std::istream *is) {
    is->read((char *)&rows, sizeof(int));
    is->read((char *)&cols, sizeof(int));
    data.read(is);
  }

  void write(std::ostream *os) {
    os->write((char *)&rows, sizeof(int));
    os->write((char *)&cols, sizeof(int));
    data.write(os);
  }
};

struct InputSpecifier {
  ImageDataSpecifier data;
  std::istream *is;

  InputSpecifier(std::istream *_is) : is(_is) {}

  InputSpecifier(std::istream *_is, size_t element_size, int dim = 1)
      : data(element_size, dim), is(_is) {}

  InputSpecifier(std::istream *_is, DataSpecifier spec) : data(spec), is(_is) {}

  void read() { data.read(is); }
};

struct OutputSpecifier {
  ImageDataSpecifier data;
  std::ostream *os;

  OutputSpecifier(std::ostream *_os) : os(_os) {}

  OutputSpecifier(std::ostream *_os, int rows, int cols, size_t element_size,
                  int dim = 1)
      : data(rows, cols, element_size, dim), os(_os) {}

  OutputSpecifier(std::ostream *_os, ImageDataSpecifier spec)
      : data(spec), os(_os) {}

  OutputSpecifier(std::ostream *_os, size_t element_size, int dim = 1)
      : data(element_size, dim), os(_os) {}

  OutputSpecifier(std::ostream *_os, DataSpecifier spec)
      : data(spec), os(_os) {}

  void write() { data.write(os); }
};

struct PixelSpecifier {
  DataSpecifier data;
  char *pixel;

  PixelSpecifier() {}

  PixelSpecifier(DataSpecifier spec) : data(spec) {}

  PixelSpecifier(ImageDataSpecifier spec) : data(spec.data) {}

  PixelSpecifier(InputSpecifier in_spec) : data(in_spec.data.data) {}

  PixelSpecifier(OutputSpecifier out_spec) : data(out_spec.data.data) {}

  size_t PixelSize() const { return data.PixelSize(); }

  void allocate() { pixel = new char[data.PixelSize()]; }

  void deallocate() { delete[] pixel; }

  void read(std::istream *is) { is->read(pixel, data.PixelSize()); }

  void read(InputSpecifier in_spec) {
    in_spec.is->read(pixel, data.PixelSize());
  }

  void write(std::ostream *os) { os->write(pixel, data.PixelSize()); }

  void write(OutputSpecifier out_spec) {
    out_spec.os->write(pixel, data.PixelSize());
  }
};

struct PixelArraySpecifier {
  DataSpecifier data;
  int size;
  char *pixels;

  PixelArraySpecifier() {}

  PixelArraySpecifier(DataSpecifier spec, int n) : data(spec), size(n) {}

  PixelArraySpecifier(InputSpecifier in_spec, int n)
      : data(in_spec.data.data), size(n) {}

  PixelArraySpecifier(OutputSpecifier out_spec, int n)
      : data(out_spec.data.data), size(n) {}

  size_t PixelSize() const { return data.PixelSize(); }

  size_t PixelsSize() const { return data.PixelSize() * size; }

  void allocate() { pixels = new char[PixelsSize() * size]; }

  void deallocate() { delete[] pixels; }

  void read(std::istream *is) { is->read(pixels, PixelsSize()); }

  void read(InputSpecifier in_spec) { in_spec.is->read(pixels, PixelsSize()); }

  void write(std::ostream *os) { os->write(pixels, PixelsSize()); }

  void write(OutputSpecifier out_spec) {
    out_spec.os->write(pixels, PixelsSize());
  }
};

/*
struct PixelSetSpecifier {
  DataSpecifier data;
  int size;
  char **pixels;
  PixelSpecifier() {}
  PixelSpecifier(DataSpecifier spec, int n) : data(spec), size(n) {}
  PixelSpecifier(InputSpecifier in_spec, int n) : data(in_spec.data), size(n) {}
  PixelSpecifier(OutputSpecifier out_spec, int n)
      : data(out_spec.data), size(n) {}
  size_t PixelSize() const { return data.PixelSize(); }
  size_t PixelsSize() const { return data.PixelSize() * size; }
  void allocate() { pixel = new char *[size]; }
  void deallocate() { delete[] pixel; }
  void read(std::istream *is) { is->read(pixel, data.PixelSize()); }
  void read(InputSpecifier in_spec) {
    in_spec.is->read(pixel, data.PixelSize());
  }
  void write(std::ostream *os) { os->write(pixel, data.PixelSize()); }
  void write(OutputSpecifier out_spec) {
    out_spec.os->write(pixel, data.PixelSize());
  }
};
//*/

struct ImagePixelSpecifier {
  ImageDataSpecifier data;
  char *pixels;

  ImagePixelSpecifier() {}

  ImagePixelSpecifier(DataSpecifier spec) : data(spec) {}

  ImagePixelSpecifier(InputSpecifier in_spec) : data(in_spec.data) {}

  ImagePixelSpecifier(OutputSpecifier out_spec) : data(out_spec.data) {}

  size_t PixelSize() const { return data.PixelSize(); }

  size_t PixelsSize() const { return data.PixelsSize(); }

  void allocate() { pixels = new char[data.PixelsSize()]; }

  void deallocate() { delete[] pixels; }

  void read(std::istream *is) { is->read(pixels, PixelsSize()); }

  void read(InputSpecifier in_spec) { in_spec.is->read(pixels, PixelsSize()); }

  void write(std::ostream *os) { os->write(pixels, PixelsSize()); }

  void write(OutputSpecifier out_spec) {
    out_spec.os->write(pixels, PixelsSize());
  }
};

} // namespace image
} // namespace graphics

#endif // GRAPHICS_IMAGE_PIXEL_H