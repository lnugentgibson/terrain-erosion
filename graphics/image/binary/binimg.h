#ifndef BINIMG_H
#define BINIMG_H

#include <algorithm>
#include <array>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <optional>

namespace graphics {
namespace image {
namespace binary {

struct DataSpecifier {
  int dim;
  size_t element_size;
  DataSpecifier() {}
  DataSpecifier(size_t _element_size, int _dim = 1) : dim(_dim), element_size(_element_size) {}
  size_t PixelSize() const {
    return dim * element_size;
  }
  void readDim(std::istream& is) {
    is.read((char *) &dim, sizeof(int));
  }
  void writeDim(std::ostream& os) {
    os.write((char *) &dim, sizeof(int));
  }
};

struct InputSpecifier {
  DataSpecifier data;
  std::istream& is;
  InputSpecifier(std::istream& _is, size_t element_size, int dim = 1) : data(element_size, dim), is(_is) {}
  void readDim() {
    data.readDim(is);
  }
  void read(int *rows, int *cols) {
    is.read((char *) rows, sizeof(int));
    is.read((char *) cols, sizeof(int));
    data.readDim(is);
  }
};

struct OutputSpecifier {
  DataSpecifier data;
  std::ostream& os;
  OutputSpecifier(std::ostream& _os, size_t element_size, int dim = 1) : data(element_size, dim), os(_os) {}
  void writeDim() {
    data.writeDim(os);
  }
  void write(int *rows, int *cols) {
    os.write((char *) rows, sizeof(int));
    os.write((char *) cols, sizeof(int));
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
  void read(std::istream& is) {
    is.read(pixel, data.PixelSize());
  }
  void read(InputSpecifier in_spec) {
    in_spec.is.read(pixel, data.PixelSize());
  }
  void write(std::ostream& os) {
    os.write(pixel, data.PixelSize());
  }
  void write(OutputSpecifier out_spec) {
    out_spec.os.write(pixel, data.PixelSize());
  }
};

class Neighborhood {
  std::deque<char *>& buffer;
 public:
  const int span;
  const int cols;
  PixelSpecifier pixel;
 private:
  int center_i, center_j;
 public:
  Neighborhood(std::deque<char *>& _buffer, int _span, int _cols, DataSpecifier _in_spec, int _center_i, int _center_j);
  ~Neighborhood();

  std::array<int, 4> range();
  const PixelSpecifier get(int i, int j);
};

class Generator {
 public:
  virtual void Generate(int i, int j, int rows, int cols, PixelSpecifier pixel) = 0;
  
  virtual void GenerateStateful(int i, int j, int rows, int cols, PixelSpecifier pixel, void *state) = 0;
};

class StatelessGenerator : public Generator {
 public:
  virtual void GenerateStateful(int i, int j, int rows, int cols, PixelSpecifier pixel, void *state) override {
    Generate(i, j, rows, cols, pixel);
  }
};

class Functor {
 public:
  virtual void Do(int i, int j, int rows, int cols, const PixelSpecifier pixel) = 0;
  
  virtual void DoStateful(int i, int j, int rows, int cols, const PixelSpecifier pixel, void *state) = 0;
};

class StatelessFunctor : public Functor {
 public:
  virtual void DoStateful(int i, int j, int rows, int cols, const PixelSpecifier pixel, void *state) override {
    Do(i, j, rows, cols, pixel);
  }
};

class Transformer {
 public:
  virtual void Transform(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel,
    PixelSpecifier out_pixel) = 0;
  
  virtual void TransformStateful(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel,
    PixelSpecifier out_pixel,
    void *state) = 0;
  
  virtual void TransformNeighborhood(
    int i, int j, int rows, int cols,
    Neighborhood&& neighborhood,
    PixelSpecifier out_pixel) = 0;
};

class StatelessTransformer : public Transformer {
 public:
  virtual void TransformStateful(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel,
    PixelSpecifier out_pixel,
    void *state) override {
      Transform(i, j, rows, cols, in_pixel, out_pixel);
    }
};

class PixelTransformer : public Transformer {
 public:
  virtual void TransformNeighborhood(
    int i, int j, int rows, int cols,
    Neighborhood&& neighborhood,
    PixelSpecifier out_pixel) override {
      const PixelSpecifier in_pixel = neighborhood.get(0, 0);
      Transform(i, j, rows, cols, in_pixel, out_pixel);
    };
};

class SimpleTransformer : public Transformer {
 public:
  void TransformStateful(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel,
    PixelSpecifier out_pixel,
    void *state) override {
      Transform(i, j, rows, cols, in_pixel, out_pixel);
    }
  
  void TransformNeighborhood(
    int i, int j, int rows, int cols,
    Neighborhood&& neighborhood,
    PixelSpecifier out_pixel) override {
      const PixelSpecifier in_pixel = neighborhood.get(0, 0);
      Transform(i, j, rows, cols, in_pixel, out_pixel);
    };
};

class Accumulator {
 public:
  virtual void Aggregate(int i, int j, int rows, int cols, const PixelSpecifier pixel, int n, void *aggregate) = 0;
};

class Combiner {
 public:
  virtual void Combine(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel1,
    const PixelSpecifier in_pixel2,
    PixelSpecifier out_pixel) = 0;
    
  virtual void CombineStateful(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel1,
    const PixelSpecifier in_pixel2,
    PixelSpecifier out_pixel,
    void *state) = 0;
};

class StatelessCombiner : public Combiner {
 public:
  virtual void CombineStateful(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel1,
    const PixelSpecifier in_pixel2,
    PixelSpecifier out_pixel,
    void *state) override {
      Combine(i, j, rows, cols, in_pixel1, in_pixel2, out_pixel);
    }
};

class Colorizer {
 public:
  virtual void ToRGB(const PixelSpecifier pixel, float *rgb) = 0;
};

class GeneratorBuilder {
 public:
  GeneratorBuilder() = default;
  virtual ~GeneratorBuilder() = default;
  virtual std::unique_ptr<Generator> operator ()() = 0;
  virtual bool SetIntParam(const std::string& param, int value) = 0;
  virtual bool SetFloatParam(const std::string& param, float value) = 0;
};

class FunctorBuilder {
 public:
  FunctorBuilder() = default;
  virtual ~FunctorBuilder() = default;
  virtual std::unique_ptr<Functor> operator ()() = 0;
  virtual bool SetIntParam(const std::string& param, int value) = 0;
  virtual bool SetFloatParam(const std::string& param, float value) = 0;
};

class TransformerBuilder {
 public:
  TransformerBuilder() = default;
  virtual ~TransformerBuilder() = default;
  virtual std::unique_ptr<Transformer> operator ()() = 0;
  virtual bool SetIntParam(const std::string& param, int value) { return false; }
  virtual bool SetFloatParam(const std::string& param, float value) { return false; }
};

class AccumulatorBuilder {
 public:
  AccumulatorBuilder() = default;
  virtual ~AccumulatorBuilder() = default;
  virtual std::unique_ptr<Accumulator> operator ()() = 0;
  virtual bool SetIntParam(const std::string& param, int value) = 0;
  virtual bool SetFloatParam(const std::string& param, float value) = 0;
};

class CombinerBuilder {
 public:
  CombinerBuilder() = default;
  virtual ~CombinerBuilder() = default;
  virtual std::unique_ptr<Combiner> operator ()() = 0;
  virtual bool SetIntParam(const std::string& param, int value) = 0;
  virtual bool SetFloatParam(const std::string& param, float value) = 0;
};

class ColorizerBuilder {
 public:
  ColorizerBuilder() = default;
  virtual ~ColorizerBuilder() = default;
  virtual std::unique_ptr<Colorizer> operator ()() = 0;
  virtual bool SetIntParam(const std::string& param, int value) = 0;
  virtual bool SetFloatParam(const std::string& param, float value) = 0;
};

typedef std::unique_ptr<GeneratorBuilder> (*GeneratorBuilderInstanceGenerator)();

class GeneratorFactory {
 public:
  bool Register(const std::string name, GeneratorBuilderInstanceGenerator create_function);
  std::unique_ptr<GeneratorBuilder> Create(const std::string& name);
	static GeneratorFactory& get();
 private:
  GeneratorFactory() {}
  ~GeneratorFactory() {}
  
  std::map<std::string, GeneratorBuilderInstanceGenerator> create_functions;
};

typedef std::unique_ptr<FunctorBuilder> (*FunctorBuilderInstanceGenerator)();

class FunctorFactory {
 public:
  bool Register(const std::string name, FunctorBuilderInstanceGenerator create_function);
  std::unique_ptr<FunctorBuilder> Create(const std::string& name);
	static FunctorFactory& get();
 private:
  FunctorFactory() {}
  ~FunctorFactory() {}
  
  std::map<std::string, FunctorBuilderInstanceGenerator> create_functions;
};

typedef std::unique_ptr<TransformerBuilder> (*TransformerBuilderInstanceGenerator)();

class TransformerFactory {
 public:
  bool Register(const std::string name, TransformerBuilderInstanceGenerator create_function);
  std::unique_ptr<TransformerBuilder> Create(const std::string& name);
	static TransformerFactory& get();
 private:
  TransformerFactory() {}
  ~TransformerFactory() {}
  
  std::map<std::string, TransformerBuilderInstanceGenerator> create_functions;
};

typedef std::unique_ptr<AccumulatorBuilder> (*AccumulatorBuilderInstanceGenerator)();

class AccumulatorFactory {
 public:
  bool Register(const std::string name, AccumulatorBuilderInstanceGenerator create_function);
  std::unique_ptr<AccumulatorBuilder> Create(const std::string& name);
	static AccumulatorFactory& get();
 private:
  AccumulatorFactory() {}
  ~AccumulatorFactory() {}
  
  std::map<std::string, AccumulatorBuilderInstanceGenerator> create_functions;
};

typedef std::unique_ptr<CombinerBuilder> (*CombinerBuilderInstanceGenerator)();

class CombinerFactory {
 public:
  bool Register(const std::string name, CombinerBuilderInstanceGenerator create_function);
  std::unique_ptr<CombinerBuilder> Create(const std::string& name);
	static CombinerFactory& get();
 private:
  CombinerFactory() {}
  ~CombinerFactory() {}
  
  std::map<std::string, CombinerBuilderInstanceGenerator> create_functions;
};

typedef std::unique_ptr<ColorizerBuilder> (*ColorizerBuilderInstanceGenerator)();

class ColorizerFactory {
 public:
  bool Register(const std::string name, ColorizerBuilderInstanceGenerator create_function);
  std::unique_ptr<ColorizerBuilder> Create(const std::string& name);
	static ColorizerFactory& get();
 private:
  ColorizerFactory() {}
  ~ColorizerFactory() {}
  
  std::map<std::string, ColorizerBuilderInstanceGenerator> create_functions;
};

void Generate(int rows, int cols, OutputSpecifier out_spec, Generator *generator);

void *GenerateStateful(int rows, int cols, OutputSpecifier out_spec, Generator *generator, void *initial);

void ForEach(InputSpecifier in_spec, Functor *functor);

void *ForEachStateful(InputSpecifier in_spec, Functor *func, void *initial);

void Map(InputSpecifier in_spec, OutputSpecifier out_spec, Transformer *map);

void *MapStateful(InputSpecifier in_spec, OutputSpecifier out_spec, Transformer *map);

void MapNeighborhood(InputSpecifier in_spec, OutputSpecifier out_spec, int span, Transformer *map);

void *Reduce(InputSpecifier in_spec, Accumulator *reducer, void *initial);

void Combine(InputSpecifier in_spec1, InputSpecifier in_spec2, OutputSpecifier out_spec, Combiner *combiner);

void *CombineStateful(InputSpecifier in_spec1, InputSpecifier in_spec2, OutputSpecifier out_spec, Combiner *combiner);

void ToPPM(InputSpecifier in_spec, std::ostream& os, Colorizer *component);

} // namespace binary
} // namespace image
} // namespace graphics

#endif // BINIMG_H