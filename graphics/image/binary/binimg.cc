#include "binimg.h"

#include <algorithm>
#include <iterator>

namespace graphics {
namespace image {
namespace binary {

void StatelessTransformer::TransformStateful(int i, int j, int rows, int cols,
                                             const PixelSpecifier in_pixel,
                                             PixelSpecifier out_pixel,
                                             void *state) {
  Transform(i, j, rows, cols, in_pixel, out_pixel);
}

void PixelTransformer::TransformNeighborhood(int i, int j, int rows, int cols,
                                             const Neighborhood &neighborhood,
                                             PixelSpecifier out_pixel) {
  const PixelSpecifier in_pixel = neighborhood.get(0, 0);
  Transform(i, j, rows, cols, in_pixel, out_pixel);
};

void SimpleTransformer::TransformStateful(int i, int j, int rows, int cols,
                                          const PixelSpecifier in_pixel,
                                          PixelSpecifier out_pixel,
                                          void *state) {
  Transform(i, j, rows, cols, in_pixel, out_pixel);
}

void SimpleTransformer::TransformNeighborhood(int i, int j, int rows, int cols,
                                              const Neighborhood &neighborhood,
                                              PixelSpecifier out_pixel) {
  const PixelSpecifier in_pixel = neighborhood.get(0, 0);
  Transform(i, j, rows, cols, in_pixel, out_pixel);
};

Neighborhood::Neighborhood(int _span, int _cols, DataSpecifier _in_spec)
    : span(_span), cols(_cols), pixel(_in_spec) {
  pixel.allocate();
}

Neighborhood::~Neighborhood() { pixel.deallocate(); }

std::array<int, 4> Neighborhood::range() const {
  if (buffer.empty())
    return {0, 0, 0, 0};
  return std::array<int, 4>({-center_i, (int)buffer.size() - center_i,
                             -std::min(center_j, span),
                             std::min(cols - center_j, span + 1)});
}

const PixelSpecifier Neighborhood::get(int i, int j) const {
  i += center_i;
  j += center_j;
  /*
  if(i < 0 || i >= (int) buffer.size()) {
    return nullptr;
  }
  if(j < 0 || j >= cols) {
    return nullptr;
  }
  */
  char *row = buffer[i];
  std::copy(row + j * pixel.PixelSize(), row + (j + 1) * pixel.PixelSize(),
            pixel.pixel);
  return pixel;
}

bool GeneratorFactory::Register(
    const std::string name, GeneratorBuilderInstanceGenerator create_function) {
  auto it = GeneratorFactory::create_functions.find(name);
  if (it == GeneratorFactory::create_functions.end()) {
    GeneratorFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<GeneratorBuilder>
GeneratorFactory::Create(const std::string &name) {
  auto it = GeneratorFactory::create_functions.find(name);
  if (it != GeneratorFactory::create_functions.end())
    return it->second();
  return nullptr;
}

GeneratorFactory &GeneratorFactory::get() {
  static GeneratorFactory instance;
  return instance;
}

void Generate(OutputSpecifier out_spec, Generator *generator) {
  int &rows = out_spec.data.rows;
  int &cols = out_spec.data.cols;
  out_spec.write();
  PixelSpecifier pixel(out_spec);
  pixel.allocate();
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      generator->Generate(i, j, rows, cols, pixel);
      pixel.write(out_spec);
    }
  pixel.deallocate();
}

void *GenerateStateful(OutputSpecifier out_spec, Generator *generator,
                       void *initial) {
  int &rows = out_spec.data.rows;
  int &cols = out_spec.data.cols;
  out_spec.write();
  PixelSpecifier pixel(out_spec);
  pixel.allocate();
  void *state = initial;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      generator->GenerateStateful(i, j, rows, cols, pixel, state);
      pixel.write(out_spec);
    }
  pixel.deallocate();
  return state;
}

bool FunctorFactory::Register(const std::string name,
                              FunctorBuilderInstanceGenerator create_function) {
  auto it = FunctorFactory::create_functions.find(name);
  if (it == FunctorFactory::create_functions.end()) {
    FunctorFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<FunctorBuilder>
FunctorFactory::Create(const std::string &name) {
  auto it = FunctorFactory::create_functions.find(name);
  if (it != FunctorFactory::create_functions.end())
    return it->second();
  return nullptr;
}

FunctorFactory &FunctorFactory::get() {
  static FunctorFactory instance;
  return instance;
}

void ForEach(InputSpecifier in_spec, Functor *functor) {
  in_spec.read();
  int &rows = in_spec.data.rows;
  int &cols = in_spec.data.cols;
  PixelSpecifier pixel(in_spec);
  pixel.allocate();
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      pixel.read(in_spec);
      functor->Do(i, j, rows, cols, pixel);
    }
  pixel.deallocate();
}

void *ForEachStateful(InputSpecifier in_spec, Functor *functor, void *initial) {
  in_spec.read();
  int &rows = in_spec.data.rows;
  int &cols = in_spec.data.cols;
  PixelSpecifier pixel(in_spec);
  pixel.allocate();
  void *state = initial;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      pixel.read(in_spec);
      functor->DoStateful(i, j, rows, cols, pixel, state);
    }
  pixel.deallocate();
  return state;
}

bool TransformerFactory::Register(
    const std::string name,
    TransformerBuilderInstanceGenerator create_function) {
  auto it = TransformerFactory::create_functions.find(name);
  if (it == TransformerFactory::create_functions.end()) {
    TransformerFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<TransformerBuilder>
TransformerFactory::Create(const std::string &name) {
  auto it = TransformerFactory::create_functions.find(name);
  if (it != TransformerFactory::create_functions.end())
    return it->second();
  return nullptr;
}

TransformerFactory &TransformerFactory::get() {
  static TransformerFactory instance;
  return instance;
}

void Map(InputSpecifier in_spec, OutputSpecifier out_spec, Transformer *map) {
  in_spec.read();
  int &rows = in_spec.data.rows;
  int &cols = in_spec.data.cols;
  out_spec.data.rows = rows;
  out_spec.data.cols = cols;
  out_spec.write();
  PixelSpecifier in_pixel(in_spec);
  in_pixel.allocate();
  PixelSpecifier out_pixel(out_spec);
  out_pixel.allocate();
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      in_pixel.read(in_spec);
      map->Transform(i, j, rows, cols, in_pixel, out_pixel);
      out_pixel.write(out_spec);
    }
  in_pixel.deallocate();
  out_pixel.deallocate();
}

void *MapStateful(InputSpecifier in_spec, OutputSpecifier out_spec,
                  Transformer *map, void *initial) {
  in_spec.read();
  int &rows = in_spec.data.rows;
  int &cols = in_spec.data.cols;
  out_spec.data.rows = rows;
  out_spec.data.cols = cols;
  out_spec.write();
  PixelSpecifier in_pixel(in_spec);
  in_pixel.allocate();
  PixelSpecifier out_pixel(out_spec);
  out_pixel.allocate();
  void *state = initial;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      in_pixel.read(in_spec);
      map->TransformStateful(i, j, rows, cols, in_pixel, out_pixel, state);
      out_pixel.write(out_spec);
    }
  in_pixel.deallocate();
  out_pixel.deallocate();
  return state;
}

void MapNeighborhood(InputSpecifier in_spec, OutputSpecifier out_spec, int span,
                     Transformer *map) {
  in_spec.read();
  int &rows = in_spec.data.rows;
  int &cols = in_spec.data.cols;
  out_spec.data.rows = rows;
  out_spec.data.cols = cols;
  out_spec.write();
  PixelSpecifier out_pixel(out_spec);
  out_pixel.allocate();
  Neighborhood n(span, cols, in_spec.data.data);
  n.SetCenter(0, 0);
  for (int i = 0; i < span; i++) {
    char *row = new char[in_spec.data.PixelSize() * cols];
    in_spec.is->read((char *)row, in_spec.data.PixelSize() * cols);
    n.Push(row);
  }
  for (int i = 0; i < rows; i++) {
    char *row = 0;
    if (i > span) {
      row = n.Front();
      n.Pop();
    }
    if (rows - i > span) {
      if (row == 0)
        row = new char[in_spec.data.PixelSize() * cols];
      in_spec.is->read((char *)row, in_spec.data.PixelSize() * cols);
      n.Push(row);
    } else if (row != 0) {
      delete[] row;
    }
    for (int j = 0; j < cols; j++) {
      n.SetCenter(std::min(i, span), j);
      map->TransformNeighborhood(i, j, rows, cols, n, out_pixel);
      out_pixel.write(out_spec);
    }
  }
  out_pixel.deallocate();
  while (!n.Empty()) {
    char *row = n.Front();
    n.Pop();
    delete[] row;
  }
}

bool AccumulatorFactory::Register(
    const std::string name,
    AccumulatorBuilderInstanceGenerator create_function) {
  auto it = AccumulatorFactory::create_functions.find(name);
  if (it == AccumulatorFactory::create_functions.end()) {
    AccumulatorFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<AccumulatorBuilder>
AccumulatorFactory::Create(const std::string &name) {
  auto it = AccumulatorFactory::create_functions.find(name);
  if (it != AccumulatorFactory::create_functions.end())
    return it->second();
  return nullptr;
}

AccumulatorFactory &AccumulatorFactory::get() {
  static AccumulatorFactory instance;
  return instance;
}

void *Reduce(InputSpecifier in_spec, Accumulator *reducer, void *initial) {
  in_spec.read();
  int &rows = in_spec.data.rows;
  int &cols = in_spec.data.cols;
  PixelSpecifier pixel(in_spec);
  pixel.allocate();
  void *aggregate = initial;
  int n = 0;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      pixel.read(in_spec);
      reducer->Aggregate(i, j, rows, cols, pixel, n, aggregate);
      n++;
    }
  pixel.deallocate();
  return aggregate;
}

bool CombinerFactory::Register(
    const std::string name, CombinerBuilderInstanceGenerator create_function) {
  auto it = CombinerFactory::create_functions.find(name);
  if (it == CombinerFactory::create_functions.end()) {
    CombinerFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<CombinerBuilder>
CombinerFactory::Create(const std::string &name) {
  auto it = CombinerFactory::create_functions.find(name);
  if (it != CombinerFactory::create_functions.end())
    return it->second();
  return nullptr;
}

CombinerFactory &CombinerFactory::get() {
  static CombinerFactory instance;
  return instance;
}

void Combine(InputSpecifier in_spec1, InputSpecifier in_spec2,
             OutputSpecifier out_spec, Combiner *combiner) {
  in_spec1.read();
  in_spec2.read();
  int &rows = in_spec1.data.rows;
  int &cols = in_spec1.data.cols;
  if (rows != in_spec2.data.rows || cols != in_spec2.data.cols) {
    return;
  }
  out_spec.data.rows = rows;
  out_spec.data.cols = cols;
  out_spec.write();
  PixelSpecifier in_pixel1(in_spec1);
  in_pixel1.allocate();
  PixelSpecifier in_pixel2(in_spec2);
  in_pixel2.allocate();
  PixelSpecifier out_pixel(out_spec);
  out_pixel.allocate();
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      in_pixel1.read(in_spec1);
      in_pixel2.read(in_spec2);
      combiner->Combine(i, j, rows, cols, in_pixel1, in_pixel2, out_pixel);
      out_pixel.write(out_spec);
    }
  in_pixel1.deallocate();
  in_pixel2.deallocate();
  out_pixel.deallocate();
}

void *CombineStatefulBin(InputSpecifier in_spec1, InputSpecifier in_spec2,
                         OutputSpecifier out_spec, Combiner *combiner,
                         void *initial) {
  in_spec1.read();
  in_spec2.read();
  int &rows = in_spec1.data.rows;
  int &cols = in_spec1.data.cols;
  if (rows != in_spec2.data.rows || cols != in_spec2.data.cols) {
    return 0;
  }
  out_spec.data.rows = rows;
  out_spec.data.cols = cols;
  out_spec.write();
  PixelSpecifier in_pixel1(in_spec1);
  in_pixel1.allocate();
  PixelSpecifier in_pixel2(in_spec2);
  in_pixel2.allocate();
  PixelSpecifier out_pixel(out_spec);
  out_pixel.allocate();
  void *state = initial;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      in_pixel1.read(in_spec1);
      in_pixel2.read(in_spec2);
      combiner->CombineStateful(i, j, rows, cols, in_pixel1, in_pixel2,
                                out_pixel, state);
      out_pixel.write(out_spec);
    }
  in_pixel1.deallocate();
  in_pixel2.deallocate();
  out_pixel.deallocate();
  return state;
}

bool ColorizerFactory::Register(
    const std::string name, ColorizerBuilderInstanceGenerator create_function) {
  auto it = ColorizerFactory::create_functions.find(name);
  if (it == ColorizerFactory::create_functions.end()) {
    ColorizerFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<ColorizerBuilder>
ColorizerFactory::Create(const std::string &name) {
  auto it = ColorizerFactory::create_functions.find(name);
  if (it != ColorizerFactory::create_functions.end())
    return it->second();
  return nullptr;
}

ColorizerFactory &ColorizerFactory::get() {
  static ColorizerFactory instance;
  return instance;
}

void ToPPM(InputSpecifier in_spec, std::ostream &os, Colorizer *component) {
  in_spec.read();
  int &rows = in_spec.data.rows;
  int &cols = in_spec.data.cols;
  os << "P6\n" << cols << " " << rows << "\n255\n";
  PixelSpecifier pixel(in_spec);
  pixel.allocate();
  float *rgb = new float[3];
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      pixel.read(in_spec);
      component->ToRGB(pixel, rgb);
      os << (unsigned char)(std::max(float(0), std::min(float(1), rgb[0])) *
                            255)
         << (unsigned char)(std::max(float(0), std::min(float(1), rgb[1])) *
                            255)
         << (unsigned char)(std::max(float(0), std::min(float(1), rgb[2])) *
                            255);
    }
  pixel.deallocate();
}

} // namespace binary
} // namespace image
} // namespace graphics