#ifndef GRAPHICS_IMAGE_BINARY_BINIMG_H
#define GRAPHICS_IMAGE_BINARY_BINIMG_H

#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <optional>

#include "graphics/image/pixel.h"

namespace graphics {
namespace image {
namespace binary {

class Neighborhood;

class Generator {
public:
  virtual void Generate(int i, int j, int rows, int cols,
                        PixelSpecifier pixel) = 0;

  virtual void GenerateStateful(int i, int j, int rows, int cols,
                                PixelSpecifier pixel, void *state) = 0;
};

class StatelessGenerator : public Generator {
public:
  virtual void GenerateStateful(int i, int j, int rows, int cols,
                                PixelSpecifier pixel, void *state) override {
    Generate(i, j, rows, cols, pixel);
  }
};

class Functor {
public:
  virtual void Do(int i, int j, int rows, int cols,
                  const PixelSpecifier pixel) = 0;

  virtual void DoStateful(int i, int j, int rows, int cols,
                          const PixelSpecifier pixel, void *state) = 0;
};

class StatelessFunctor : public Functor {
public:
  virtual void DoStateful(int i, int j, int rows, int cols,
                          const PixelSpecifier pixel, void *state) override {
    Do(i, j, rows, cols, pixel);
  }
};

class Transformer {
public:
  virtual void Transform(int i, int j, int rows, int cols,
                         const PixelSpecifier in_pixel,
                         PixelSpecifier out_pixel) = 0;

  virtual void TransformStateful(int i, int j, int rows, int cols,
                                 const PixelSpecifier in_pixel,
                                 PixelSpecifier out_pixel, void *state) = 0;

  virtual void TransformNeighborhood(int i, int j, int rows, int cols,
                                     const Neighborhood &neighborhood,
                                     PixelSpecifier out_pixel) = 0;
};

class StatelessTransformer : public Transformer {
public:
  virtual void TransformStateful(int i, int j, int rows, int cols,
                                 const PixelSpecifier in_pixel,
                                 PixelSpecifier out_pixel,
                                 void *state) override;
};

class PixelTransformer : public Transformer {
public:
  virtual void TransformNeighborhood(int i, int j, int rows, int cols,
                                     const Neighborhood &neighborhood,
                                     PixelSpecifier out_pixel) override;
};

class SimpleTransformer : public Transformer {
public:
  void TransformStateful(int i, int j, int rows, int cols,
                         const PixelSpecifier in_pixel,
                         PixelSpecifier out_pixel, void *state) override;

  void TransformNeighborhood(int i, int j, int rows, int cols,
                             const Neighborhood &neighborhood,
                             PixelSpecifier out_pixel) override;
};

class Accumulator {
public:
  virtual void Aggregate(int i, int j, int rows, int cols,
                         const PixelSpecifier pixel, int n,
                         void *aggregate) = 0;
};

class Combiner {
public:
  virtual void Combine(int i, int j, int rows, int cols,
                       const PixelSpecifier in_pixel1,
                       const PixelSpecifier in_pixel2,
                       PixelSpecifier out_pixel) = 0;

  virtual void CombineStateful(int i, int j, int rows, int cols,
                               const PixelSpecifier in_pixel1,
                               const PixelSpecifier in_pixel2,
                               PixelSpecifier out_pixel, void *state) = 0;
};

class StatelessCombiner : public Combiner {
public:
  virtual void CombineStateful(int i, int j, int rows, int cols,
                               const PixelSpecifier in_pixel1,
                               const PixelSpecifier in_pixel2,
                               PixelSpecifier out_pixel, void *state) override {
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
  virtual std::unique_ptr<Generator> operator()() = 0;
  virtual bool SetIntParam(const std::string &param, int value) {
    return false;
  }
  virtual bool SetFloatParam(const std::string &param, float value) {
    return false;
  }
  virtual bool SetPtrParam(const std::string &param, void *value) {
    return false;
  }
};

class FunctorBuilder {
public:
  FunctorBuilder() = default;
  virtual ~FunctorBuilder() = default;
  virtual std::unique_ptr<Functor> operator()() = 0;
  virtual bool SetIntParam(const std::string &param, int value) {
    return false;
  }
  virtual bool SetFloatParam(const std::string &param, float value) {
    return false;
  }
  virtual bool SetPtrParam(const std::string &param, void *value) {
    return false;
  }
};

class TransformerBuilder {
public:
  TransformerBuilder() = default;
  virtual ~TransformerBuilder() = default;
  virtual std::unique_ptr<Transformer> operator()() = 0;
  virtual bool SetIntParam(const std::string &param, int value) {
    return false;
  }
  virtual bool SetFloatParam(const std::string &param, float value) {
    return false;
  }
  virtual bool SetPtrParam(const std::string &param, void *value) {
    return false;
  }
};

class AccumulatorBuilder {
public:
  AccumulatorBuilder() = default;
  virtual ~AccumulatorBuilder() = default;
  virtual std::unique_ptr<Accumulator> operator()() = 0;
  virtual bool SetIntParam(const std::string &param, int value) {
    return false;
  }
  virtual bool SetFloatParam(const std::string &param, float value) {
    return false;
  }
  virtual bool SetPtrParam(const std::string &param, void *value) {
    return false;
  }
};

class CombinerBuilder {
public:
  CombinerBuilder() = default;
  virtual ~CombinerBuilder() = default;
  virtual std::unique_ptr<Combiner> operator()() = 0;
  virtual bool SetIntParam(const std::string &param, int value) {
    return false;
  }
  virtual bool SetFloatParam(const std::string &param, float value) {
    return false;
  }
  virtual bool SetPtrParam(const std::string &param, void *value) {
    return false;
  }
};

class ColorizerBuilder {
public:
  ColorizerBuilder() = default;
  virtual ~ColorizerBuilder() = default;
  virtual std::unique_ptr<Colorizer> operator()() = 0;
  virtual bool SetIntParam(const std::string &param, int value) {
    return false;
  }
  virtual bool SetFloatParam(const std::string &param, float value) {
    return false;
  }
  virtual bool SetPtrParam(const std::string &param, void *value) {
    return false;
  }
};

typedef std::unique_ptr<GeneratorBuilder> (
    *GeneratorBuilderInstanceGenerator)();

class GeneratorFactory {
public:
  bool Register(const std::string name,
                GeneratorBuilderInstanceGenerator create_function);
  std::unique_ptr<GeneratorBuilder> Create(const std::string &name);
  static GeneratorFactory &get();

private:
  GeneratorFactory() {}
  ~GeneratorFactory() {}

  std::map<std::string, GeneratorBuilderInstanceGenerator> create_functions;
};

typedef std::unique_ptr<FunctorBuilder> (*FunctorBuilderInstanceGenerator)();

class FunctorFactory {
public:
  bool Register(const std::string name,
                FunctorBuilderInstanceGenerator create_function);
  std::unique_ptr<FunctorBuilder> Create(const std::string &name);
  static FunctorFactory &get();

private:
  FunctorFactory() {}
  ~FunctorFactory() {}

  std::map<std::string, FunctorBuilderInstanceGenerator> create_functions;
};

typedef std::unique_ptr<TransformerBuilder> (
    *TransformerBuilderInstanceGenerator)();

class TransformerFactory {
public:
  bool Register(const std::string name,
                TransformerBuilderInstanceGenerator create_function);
  std::unique_ptr<TransformerBuilder> Create(const std::string &name);
  static TransformerFactory &get();

private:
  TransformerFactory() {}
  ~TransformerFactory() {}

  std::map<std::string, TransformerBuilderInstanceGenerator> create_functions;
};

typedef std::unique_ptr<AccumulatorBuilder> (
    *AccumulatorBuilderInstanceGenerator)();

class AccumulatorFactory {
public:
  bool Register(const std::string name,
                AccumulatorBuilderInstanceGenerator create_function);
  std::unique_ptr<AccumulatorBuilder> Create(const std::string &name);
  static AccumulatorFactory &get();

private:
  AccumulatorFactory() {}
  ~AccumulatorFactory() {}

  std::map<std::string, AccumulatorBuilderInstanceGenerator> create_functions;
};

typedef std::unique_ptr<CombinerBuilder> (*CombinerBuilderInstanceGenerator)();

class CombinerFactory {
public:
  bool Register(const std::string name,
                CombinerBuilderInstanceGenerator create_function);
  std::unique_ptr<CombinerBuilder> Create(const std::string &name);
  static CombinerFactory &get();

private:
  CombinerFactory() {}
  ~CombinerFactory() {}

  std::map<std::string, CombinerBuilderInstanceGenerator> create_functions;
};

typedef std::unique_ptr<ColorizerBuilder> (
    *ColorizerBuilderInstanceGenerator)();

class ColorizerFactory {
public:
  bool Register(const std::string name,
                ColorizerBuilderInstanceGenerator create_function);
  std::unique_ptr<ColorizerBuilder> Create(const std::string &name);
  static ColorizerFactory &get();

private:
  ColorizerFactory() {}
  ~ColorizerFactory() {}

  std::map<std::string, ColorizerBuilderInstanceGenerator> create_functions;
};

void Generate(OutputSpecifier out_spec, Generator *generator);

void *GenerateStateful(OutputSpecifier out_spec, Generator *generator,
                       void *initial);

void ForEach(InputSpecifier in_spec, Functor *functor);

void *ForEachStateful(InputSpecifier in_spec, Functor *func, void *initial);

void Map(InputSpecifier in_spec, OutputSpecifier out_spec, Transformer *map);

void *MapStateful(InputSpecifier in_spec, OutputSpecifier out_spec,
                  Transformer *map);

void MapNeighborhood(InputSpecifier in_spec, OutputSpecifier out_spec, int span,
                     Transformer *map);

void *Reduce(InputSpecifier in_spec, Accumulator *reducer, void *initial);

void Combine(InputSpecifier in_spec1, InputSpecifier in_spec2,
             OutputSpecifier out_spec, Combiner *combiner);

void *CombineStateful(InputSpecifier in_spec1, InputSpecifier in_spec2,
                      OutputSpecifier out_spec, Combiner *combiner);

void ToPPM(InputSpecifier in_spec, std::ostream &os, Colorizer *component);

class Neighborhood {
  std::deque<char *> buffer;
  int center_i, center_j;

public:
  const int span;
  const int cols;
  PixelSpecifier pixel;

public:
  Neighborhood(int _span, int _cols, DataSpecifier _in_spec);
  ~Neighborhood();

  std::array<int, 4> range() const;
  const PixelSpecifier get(int i, int j) const;

  int GetCenterI() { return center_i; }
  int GetCenterJ() { return center_j; }
  void SetCenterI(int i) { center_i = i; }
  void SetCenterJ(int j) { center_j = j; }
  void SetCenter(int i, int j) {
    center_i = i;
    center_j = j;
  }
  void Push() { buffer.push_back(new char[cols]); }
  void Push(char *row) { buffer.push_back(row); }
  void Pop() { buffer.pop_front(); }
  char *Front() { return buffer.front(); }
  bool Empty() { return buffer.empty(); }
  void Set(int i, int j, char v) { buffer[i][j] = v; }
};

} // namespace binary
} // namespace image
} // namespace graphics

#endif // GRAPHICS_IMAGE_BINARY_BINIMG_H