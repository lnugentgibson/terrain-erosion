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
  size_t PixelSize() const {
    return dim * element_size;
  }
};

struct InputSpecifier {
  DataSpecifier data;
  std::istream& is;
};

struct OutputSpecifier {
  DataSpecifier data;
  std::ostream& os;
};

class Neighborhood {
  std::deque<char *>& buffer;
 public:
  const int span;
  const int cols;
  const DataSpecifier in_spec;
 private:
  int center_i, center_j;
 public:
  Neighborhood(std::deque<char *>& _buffer, int _span, int _cols, DataSpecifier _in_spec, int _center_i, int _center_j);

  std::array<int, 4> range();
  char* get(int i, int j);
};

class Generator {
 public:
  virtual void Generate(int i, int j, int rows, int cols, void *pixel, DataSpecifier out_spec) = 0;
  
  virtual void GenerateStateful(int i, int j, int rows, int cols, void *pixel, DataSpecifier out_spec, void *state) = 0;
};

class StatelessGenerator : public Generator {
 public:
  virtual void GenerateStateful(int i, int j, int rows, int cols, void *pixel, DataSpecifier out_spec, void *state) override {
    Generate(i, j, rows, cols, pixel, out_spec);
  }
};

class GeneratorBuilder {
 public:
  GeneratorBuilder() = default;
  virtual ~GeneratorBuilder() = default;
  virtual std::unique_ptr<Generator> operator ()() = 0;
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

void Generate(int rows, int cols, OutputSpecifier out_spec, Generator *generator);

void *GenerateStateful(int rows, int cols, OutputSpecifier out_spec, Generator *generator, void *initial);

class Functor {
 public:
  virtual void Do(int i, int j, int rows, int cols, const void *pixel, DataSpecifier in_spec) = 0;
  
  virtual void DoStateful(int i, int j, int rows, int cols, const void *pixel, DataSpecifier in_spec, void *state) = 0;
};

class StatelessFunctor : public Functor {
 public:
  virtual void DoStateful(int i, int j, int rows, int cols, const void *pixel, DataSpecifier in_spec, void *state) override {
    Do(i, j, rows, cols, pixel, in_spec);
  }
};

class FunctorBuilder {
 public:
  FunctorBuilder() = default;
  virtual ~FunctorBuilder() = default;
  virtual std::unique_ptr<Functor> operator ()() = 0;
  virtual bool SetIntParam(const std::string& param, int value) = 0;
  virtual bool SetFloatParam(const std::string& param, float value) = 0;
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

void ForEach(InputSpecifier in_spec, Functor *functor);

void *ForEachStateful(InputSpecifier in_spec, Functor *func, void *initial);

class Transformer {
 public:
  virtual void Transform(
    int i, int j, int rows, int cols,
    const void *pixel1, DataSpecifier in_spec,
    void *pixel2, DataSpecifier out_spec) = 0;
  
  virtual void TransformStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, DataSpecifier in_spec,
    void *pixel2, DataSpecifier out_spec,
    void *state) = 0;
  
  virtual void TransformNeighborhood(
    int i, int j, int rows, int cols,
    Neighborhood&& neighborhood,
    void *pixel2, DataSpecifier out_spec) = 0;
};

class StatelessTransformer : public Transformer {
 public:
  virtual void TransformStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, DataSpecifier in_spec,
    void *pixel2, DataSpecifier out_spec,
    void *state) override {
      Transform(i, j, rows, cols, pixel1, in_spec, pixel2, out_spec);
    }
};

class PixelTransformer : public Transformer {
 public:
  virtual void TransformNeighborhood(
    int i, int j, int rows, int cols,
    Neighborhood&& neighborhood,
    void *pixel2, DataSpecifier out_spec) override {
      char *pixel1 = neighborhood.get(0, 0);
      Transform(i, j, rows, cols, pixel1, neighborhood.in_spec, pixel2, out_spec);
      delete[] pixel1;
    };
};

class SimpleTransformer : public Transformer {
 public:
  virtual void TransformStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, DataSpecifier in_spec,
    void *pixel2, DataSpecifier out_spec,
    void *state) override {
      Transform(i, j, rows, cols, pixel1, in_spec, pixel2, out_spec);
    }
  
  virtual void TransformNeighborhood(
    int i, int j, int rows, int cols,
    Neighborhood&& neighborhood,
    void *pixel2, DataSpecifier out_spec) override {
      char *pixel1 = neighborhood.get(0, 0);
      Transform(i, j, rows, cols, pixel1, neighborhood.in_spec, pixel2, out_spec);
      delete[] pixel1;
    };
};

class TransformerBuilder {
 public:
  TransformerBuilder() = default;
  virtual ~TransformerBuilder() = default;
  virtual std::unique_ptr<Transformer> operator ()() = 0;
  virtual bool SetIntParam(const std::string& param, int value) = 0;
  virtual bool SetFloatParam(const std::string& param, float value) = 0;
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

void Map(InputSpecifier in_spec, OutputSpecifier out_spec, Transformer *map);

void *MapStateful(InputSpecifier in_spec, OutputSpecifier out_spec, Transformer *map);

void MapNeighborhood(InputSpecifier in_spec, OutputSpecifier out_spec, int span, Transformer *map);

class Accumulator {
 public:
  virtual void Aggregate(int i, int j, int rows, int cols, const void *pixel, DataSpecifier in_spec, int n, void *aggregate) = 0;
};

class AccumulatorBuilder {
 public:
  AccumulatorBuilder() = default;
  virtual ~AccumulatorBuilder() = default;
  virtual std::unique_ptr<Accumulator> operator ()() = 0;
  virtual bool SetIntParam(const std::string& param, int value) = 0;
  virtual bool SetFloatParam(const std::string& param, float value) = 0;
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

void *Reduce(InputSpecifier in_spec, Accumulator *reducer, void *initial);

class Combiner {
 public:
  virtual void Combine(
    int i, int j, int rows, int cols,
    const void *pixel1, DataSpecifier in_spec1,
    const void *pixel2, DataSpecifier in_spec2,
    void *pixel3, DataSpecifier out_spec) = 0;
    
  virtual void CombineStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, DataSpecifier in_spec1,
    const void *pixel2, DataSpecifier in_spec2,
    void *pixel3, DataSpecifier out_spec,
    void *state) = 0;
};

class StatelessCombiner : public Combiner {
 public:
  virtual void CombineStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, DataSpecifier in_spec1,
    const void *pixel2, DataSpecifier in_spec2,
    void *pixel3, DataSpecifier out_spec,
    void *state) override {
      Combine(i, j, rows, cols, pixel1, in_spec1, pixel2, in_spec2, pixel3, out_spec);
    }
};

class CombinerBuilder {
 public:
  CombinerBuilder() = default;
  virtual ~CombinerBuilder() = default;
  virtual std::unique_ptr<Combiner> operator ()() = 0;
  virtual bool SetIntParam(const std::string& param, int value) = 0;
  virtual bool SetFloatParam(const std::string& param, float value) = 0;
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

void Combine(InputSpecifier in_spec1, InputSpecifier in_spec2, OutputSpecifier out_spec, Combiner *combiner);

void *CombineStateful(InputSpecifier in_spec1, InputSpecifier in_spec2, OutputSpecifier out_spec, Combiner *combiner);

class Colorizer {
 public:
  virtual void ToRGB(const void *pixel, DataSpecifier in_spec, float *rgb) = 0;
};

class ColorizerBuilder {
 public:
  ColorizerBuilder() = default;
  virtual ~ColorizerBuilder() = default;
  virtual std::unique_ptr<Colorizer> operator ()() = 0;
  virtual bool SetIntParam(const std::string& param, int value) = 0;
  virtual bool SetFloatParam(const std::string& param, float value) = 0;
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

void ToPPM(InputSpecifier in_spec, std::ostream& os, Colorizer *component);

} // namespace binary
} // namespace image
} // namespace graphics

#endif // BINIMG_H