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

class Neighborhood {
  std::deque<char *>& buffer;
 public:
  const int span;
  const int cols;
  const int dim;
  const size_t element_size;
 private:
  int center_i, center_j;
 public:
  Neighborhood(std::deque<char *>& _buffer, int _span, int _cols, int _dim, size_t _element_size, int _center_i, int _center_j);

  std::array<int, 4> range();
  char* get(int i, int j);
};

class Generator {
 public:
  virtual void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) = 0;
  
  virtual void GenerateStateful(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size, void *state) = 0;
};

class StatelessGenerator : public Generator {
 public:
  virtual void GenerateStateful(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size, void *state) override {
    Generate(i, j, rows, cols, pixel, dim, element_size);
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

void Generate(int rows, int cols, int dim, size_t element_size, std::ostream& os, Generator *generator);

void *GenerateStateful(int rows, int cols, int dim, size_t element_size, std::ostream& os, Generator *generator, void *initial);

class Functor {
 public:
  virtual void Do(int i, int j, int rows, int cols, const void *pixel, int dim, size_t element_size) = 0;
  
  virtual void DoStateful(int i, int j, int rows, int cols, const void *pixel, int dim, size_t element_size, void *state) = 0;
};

class StatelessFunctor : public Functor {
 public:
  virtual void DoStateful(int i, int j, int rows, int cols, const void *pixel, int dim, size_t element_size, void *state) override {
    Do(i, j, rows, cols, pixel, dim, element_size);
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

void ForEach(size_t element_size, std::istream& is, Functor *functor);

void *ForEachStateful(size_t element_size, std::istream& is, Functor *func, void *initial);

class Transformer {
 public:
  virtual void Transform(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    void *pixel2, int dim2, size_t element_size2) = 0;
  
  virtual void TransformStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    void *pixel2, int dim2, size_t element_size2,
    void *state) = 0;
  
  virtual void TransformNeighborhood(
    int i, int j, int rows, int cols,
    Neighborhood&& neighborhood,
    void *pixel2, int dim2, size_t element_size2) = 0;
};

class StatelessTransformer : public Transformer {
 public:
  virtual void TransformStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    void *pixel2, int dim2, size_t element_size2,
    void *state) override {
      Transform(i, j, rows, cols, pixel1, dim1, element_size1, pixel2, dim2, element_size2);
    }
};

class PixelTransformer : public Transformer {
 public:
  virtual void TransformNeighborhood(
    int i, int j, int rows, int cols,
    Neighborhood&& neighborhood,
    void *pixel2, int dim2, size_t element_size2) override {
      char *pixel1 = neighborhood.get(0, 0);
      Transform(i, j, rows, cols, pixel1, neighborhood.dim, neighborhood.element_size, pixel2, dim2, element_size2);
      delete[] pixel1;
    };
};

class SimpleTransformer : public Transformer {
 public:
  virtual void TransformStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    void *pixel2, int dim2, size_t element_size2,
    void *state) override {
      Transform(i, j, rows, cols, pixel1, dim1, element_size1, pixel2, dim2, element_size2);
    }
  
  virtual void TransformNeighborhood(
    int i, int j, int rows, int cols,
    Neighborhood&& neighborhood,
    void *pixel2, int dim2, size_t element_size2) override {
      char *pixel1 = neighborhood.get(0, 0);
      Transform(i, j, rows, cols, pixel1, neighborhood.dim, neighborhood.element_size, pixel2, dim2, element_size2);
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

void Map(size_t element_size1, std::istream& is, size_t element_size2, int dim2, std::ostream& os, Transformer *map);

void *MapStateful(size_t element_size1, std::istream& is, size_t element_size2, int dim2, std::ostream& os, Transformer *map);

void MapNeighborhood(size_t element_size1, std::istream& is, size_t element_size2, int dim2, std::ostream& os, int span, Transformer *map);

class Accumulator {
 public:
  virtual void Aggregate(int i, int j, int rows, int cols, const void *pixel, int dim, size_t element_size, int n, void *aggregate) = 0;
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

void *Reduce(size_t element_size, std::istream& is, Accumulator *reducer, void *initial);

class Combiner {
 public:
  virtual void Combine(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    const void *pixel2, int dim2, size_t element_size2,
    void *pixel3, int dim3, size_t element_size3) = 0;
    
  virtual void CombineStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    const void *pixel2, int dim2, size_t element_size2,
    void *pixel3, int dim3, size_t element_size3,
    void *state) = 0;
};

class StatelessCombiner : public Combiner {
 public:
  virtual void CombineStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    const void *pixel2, int dim2, size_t element_size2,
    void *pixel3, int dim3, size_t element_size3,
    void *state) override {
      Combine(i, j, rows, cols, pixel1, dim1, element_size1, pixel2, dim2, element_size2, pixel3, dim3, element_size3);
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

void Combine(size_t element_size1, std::istream& is1, size_t element_size2, std::istream& is2, size_t element_size3, int dim3, std::ostream& os, Combiner *combiner);

void *CombineStateful(size_t element_size1, std::istream& is1, size_t element_size2, std::istream& is2, size_t element_size3, int dim3, std::ostream& os, Combiner *combiner);

class Colorizer {
 public:
  virtual void ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) = 0;
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

void ToPPM(size_t element_size, std::istream& is, std::ostream& os, Colorizer *component);

} // namespace binary
} // namespace image
} // namespace graphics

#endif // BINIMG_H