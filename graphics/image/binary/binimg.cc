#include "binimg.h"

#include <algorithm>
#include <array>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>

namespace graphics {
namespace image {
namespace binary {

Neighborhood::Neighborhood(std::deque<char *>& _buffer, int _span, int _cols, DataSpecifier _in_spec, int _center_i, int _center_j) :
  buffer(_buffer), span(_span), cols(_cols), in_spec(_in_spec), center_i(_center_i), center_j(_center_j) {}

std::array<int, 4> Neighborhood::range() {
  if(buffer.empty()) return {0, 0, 0, 0};
  return std::array<int, 4>({-center_i, (int) buffer.size() - center_i, -std::min(center_j, span), std::min(cols - center_j, span)});
}

char *Neighborhood::get(int i, int j) {
  i += center_i;
  if(i < 0 || i >= (int) buffer.size()) {
    return nullptr;
  }
  j += center_j;
  if(j < 0 || j >= cols) {
    return nullptr;
  }
  char *row = buffer[i];
  char *pixel = new char[in_spec.PixelSize()];
  std::copy(row + j * in_spec.PixelSize(), row + (j + 1) * in_spec.PixelSize(), pixel);
  return pixel;
}

bool GeneratorFactory::Register(const std::string name, GeneratorBuilderInstanceGenerator create_function) {
  auto it = GeneratorFactory::create_functions.find(name);
  if(it == GeneratorFactory::create_functions.end())
  {
    GeneratorFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<GeneratorBuilder> GeneratorFactory::Create(const std::string& name) {
  auto it = GeneratorFactory::create_functions.find(name);
  if(it != GeneratorFactory::create_functions.end()) 
    return it->second();
  return nullptr;
}

GeneratorFactory& GeneratorFactory::get()
{
	static GeneratorFactory instance;
	return instance;
}

void Generate(int rows, int cols, OutputSpecifier out_spec, Generator *generator) {
  out_spec.os.write((char *) &rows, sizeof(int));
  out_spec.os.write((char *) &cols, sizeof(int));
  out_spec.os.write((char *) &out_spec.data.dim, sizeof(int));
  char *pixel = new char[out_spec.data.PixelSize()];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      generator->Generate(i, j, rows, cols, pixel, out_spec.data);
      out_spec.os.write((char *) pixel, out_spec.data.PixelSize());
    }
}

void *GenerateStateful(int rows, int cols, OutputSpecifier out_spec, Generator *generator, void *initial) {
  out_spec.os.write((char *) &rows, sizeof(int));
  out_spec.os.write((char *) &cols, sizeof(int));
  out_spec.os.write((char *) &out_spec.data.dim, sizeof(int));
  char *pixel = new char[out_spec.data.PixelSize()];
  void *state = initial;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      generator->GenerateStateful(i, j, rows, cols, pixel, out_spec.data, state);
      out_spec.os.write((char *) pixel, out_spec.data.PixelSize());
    }
  return state;
}

bool FunctorFactory::Register(const std::string name, FunctorBuilderInstanceGenerator create_function) {
  auto it = FunctorFactory::create_functions.find(name);
  if(it == FunctorFactory::create_functions.end())
  {
    FunctorFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<FunctorBuilder> FunctorFactory::Create(const std::string& name) {
  auto it = FunctorFactory::create_functions.find(name);
  if(it != FunctorFactory::create_functions.end()) 
    return it->second();
  return nullptr;
}

FunctorFactory& FunctorFactory::get()
{
	static FunctorFactory instance;
	return instance;
}

void ForEach(InputSpecifier in_spec, Functor *functor) {
  int rows, cols;
  in_spec.is.read((char *) &rows, sizeof(int));
  in_spec.is.read((char *) &cols, sizeof(int));
  in_spec.is.read((char *) &in_spec.data.dim, sizeof(int));
  char *pixel = new char[in_spec.data.PixelSize()];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      in_spec.is.read((char *) pixel, in_spec.data.PixelSize());
      functor->Do(i, j, rows, cols, pixel, in_spec.data);
    }
}

void *ForEachStateful(InputSpecifier in_spec, Functor *functor, void *initial) {
  int rows, cols;
  in_spec.is.read((char *) &rows, sizeof(int));
  in_spec.is.read((char *) &cols, sizeof(int));
  in_spec.is.read((char *) &in_spec.data.dim, sizeof(int));
  char *pixel = new char[in_spec.data.PixelSize()];
  void *state = initial;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      in_spec.is.read((char *) pixel, in_spec.data.PixelSize());
      functor->DoStateful(i, j, rows, cols, pixel, in_spec.data, state);
    }
  return state;
}

bool TransformerFactory::Register(const std::string name, TransformerBuilderInstanceGenerator create_function) {
  auto it = TransformerFactory::create_functions.find(name);
  if(it == TransformerFactory::create_functions.end())
  {
    TransformerFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<TransformerBuilder> TransformerFactory::Create(const std::string& name) {
  auto it = TransformerFactory::create_functions.find(name);
  if(it != TransformerFactory::create_functions.end()) 
    return it->second();
  return nullptr;
}

TransformerFactory& TransformerFactory::get()
{
	static TransformerFactory instance;
	return instance;
}

void Map(InputSpecifier in_spec, OutputSpecifier out_spec, Transformer *map) {
  int rows, cols;
  in_spec.is.read((char *) &rows, sizeof(int));
  in_spec.is.read((char *) &cols, sizeof(int));
  in_spec.is.read((char *) &in_spec.data.dim, sizeof(int));
  out_spec.os.write((char *) &rows, sizeof(int));
  out_spec.os.write((char *) &cols, sizeof(int));
  out_spec.os.write((char *) &out_spec.data.dim, sizeof(int));
  char *pixel1 = new char[in_spec.data.PixelSize()];
  char *pixel2 = new char[out_spec.data.PixelSize()];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      in_spec.is.read((char *) pixel1, in_spec.data.PixelSize());
      map->Transform(i, j, rows, cols, pixel1, in_spec.data, pixel2, out_spec.data);
      out_spec.os.write((char *) pixel2, out_spec.data.PixelSize());
    }
}

void *MapStateful(InputSpecifier in_spec, OutputSpecifier out_spec, Transformer *map, void *initial) {
  int rows, cols;
  in_spec.is.read((char *) &rows, sizeof(int));
  in_spec.is.read((char *) &cols, sizeof(int));
  in_spec.is.read((char *) &in_spec.data.dim, sizeof(int));
  out_spec.os.write((char *) &rows, sizeof(int));
  out_spec.os.write((char *) &cols, sizeof(int));
  out_spec.os.write((char *) &out_spec.data.dim, sizeof(int));
  char *pixel1 = new char[in_spec.data.PixelSize()];
  char *pixel2 = new char[out_spec.data.PixelSize()];
  void *state = initial;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      in_spec.is.read((char *) pixel1, in_spec.data.PixelSize());
      map->TransformStateful(i, j, rows, cols, pixel1, in_spec.data, pixel2, out_spec.data, state);
      out_spec.os.write((char *) pixel2, out_spec.data.PixelSize());
    }
  return state;
}

void MapNeighborhood(InputSpecifier in_spec, OutputSpecifier out_spec, int span, Transformer *map) {
  int rows, cols;
  in_spec.is.read((char *) &rows, sizeof(int));
  in_spec.is.read((char *) &cols, sizeof(int));
  in_spec.is.read((char *) &in_spec.data.dim, sizeof(int));
  out_spec.os.write((char *) &rows, sizeof(int));
  out_spec.os.write((char *) &cols, sizeof(int));
  out_spec.os.write((char *) &out_spec.data.dim, sizeof(int));
  char *pixel2 = new char[out_spec.data.PixelSize()];
  std::deque<char *> buffer;
  for(int i = 0; i < span; i++) {
    char *row = new char[in_spec.data.PixelSize() * cols];
    in_spec.is.read((char *) row, in_spec.data.PixelSize());
    buffer.push_back(row);
  }
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      char *row = 0;
      if(i > span) {
        row = buffer.front();
        buffer.pop_front();
      }
      if(rows - i > span) {
        if(row == 0) row = new char[in_spec.data.PixelSize() * cols];
        in_spec.is.read((char *) row, in_spec.data.PixelSize());
        buffer.push_back(row);
      } else if(row != 0) {
        delete[] row;
      }
      map->TransformNeighborhood(i, j, rows, cols,
        Neighborhood(buffer, span, cols, in_spec.data, std::min(i, span), j),
        pixel2, out_spec.data);
      out_spec.os.write((char *) pixel2, out_spec.data.PixelSize());
    }
  while(!buffer.empty()) {
    char *row = buffer.front();
    buffer.pop_front();
    delete[] row;
  }
}

bool AccumulatorFactory::Register(const std::string name, AccumulatorBuilderInstanceGenerator create_function) {
  auto it = AccumulatorFactory::create_functions.find(name);
  if(it == AccumulatorFactory::create_functions.end())
  {
    AccumulatorFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<AccumulatorBuilder> AccumulatorFactory::Create(const std::string& name) {
  auto it = AccumulatorFactory::create_functions.find(name);
  if(it != AccumulatorFactory::create_functions.end()) 
    return it->second();
  return nullptr;
}

AccumulatorFactory& AccumulatorFactory::get()
{
	static AccumulatorFactory instance;
	return instance;
}

void *Reduce(InputSpecifier in_spec, Accumulator *reducer, void *initial) {
  int rows, cols;
  in_spec.is.read((char *) &rows, sizeof(int));
  in_spec.is.read((char *) &cols, sizeof(int));
  in_spec.is.read((char *) &in_spec.data.dim, sizeof(int));
  char *pixel = new char[in_spec.data.PixelSize()];
  void *aggregate = initial;
  int n = 0;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      in_spec.is.read((char *) pixel, in_spec.data.PixelSize());
      reducer->Aggregate(i, j, rows, cols, pixel, in_spec.data, n, aggregate);
      n++;
    }
  return aggregate;
}

bool CombinerFactory::Register(const std::string name, CombinerBuilderInstanceGenerator create_function) {
  auto it = CombinerFactory::create_functions.find(name);
  if(it == CombinerFactory::create_functions.end())
  {
    CombinerFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<CombinerBuilder> CombinerFactory::Create(const std::string& name) {
  auto it = CombinerFactory::create_functions.find(name);
  if(it != CombinerFactory::create_functions.end()) 
    return it->second();
  return nullptr;
}

CombinerFactory& CombinerFactory::get()
{
	static CombinerFactory instance;
	return instance;
}

void Combine(InputSpecifier in_spec1, InputSpecifier in_spec2, OutputSpecifier out_spec, Combiner *combiner) {
  int rows, cols;
  int _rows, _cols;
  in_spec1.is.read((char *) &rows, sizeof(int));
  in_spec1.is.read((char *) &cols, sizeof(int));
  in_spec1.is.read((char *) &in_spec1.data.dim, sizeof(int));
  in_spec2.is.read((char *) &_rows, sizeof(int));
  in_spec2.is.read((char *) &_cols, sizeof(int));
  in_spec2.is.read((char *) &in_spec2.data.dim, sizeof(int));
  if(rows != _rows || cols != _cols) {
    return;
  }
  out_spec.os.write((char *) &rows, sizeof(int));
  out_spec.os.write((char *) &cols, sizeof(int));
  out_spec.os.write((char *) &out_spec.data.dim, sizeof(int));
  char *pixel1 = new char[in_spec1.data.PixelSize()];
  char *pixel2 = new char[in_spec2.data.PixelSize()];
  char *pixel3 = new char[out_spec.data.PixelSize()];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      in_spec1.is.read((char *) pixel1, in_spec1.data.PixelSize());
      in_spec2.is.read((char *) pixel2, in_spec2.data.PixelSize());
      combiner->Combine(i, j, rows, cols, pixel1, in_spec1.data, pixel2, in_spec2.data, pixel3, out_spec.data);
      out_spec.os.write((char *) pixel3, out_spec.data.PixelSize());
    }
}

void *CombineStatefulBin(InputSpecifier in_spec1, InputSpecifier in_spec2, OutputSpecifier out_spec, Combiner *combiner, void *initial) {
  int rows, cols;
  int _rows, _cols;
  in_spec1.is.read((char *) &rows, sizeof(int));
  in_spec1.is.read((char *) &cols, sizeof(int));
  in_spec1.is.read((char *) &in_spec1.data.dim, sizeof(int));
  in_spec2.is.read((char *) &_rows, sizeof(int));
  in_spec2.is.read((char *) &_cols, sizeof(int));
  in_spec2.is.read((char *) &in_spec2.data.dim, sizeof(int));
  if(rows != _rows || cols != _cols) {
    return 0;
  }
  out_spec.os.write((char *) &rows, sizeof(int));
  out_spec.os.write((char *) &cols, sizeof(int));
  out_spec.os.write((char *) &out_spec.data.dim, sizeof(int));
  char *pixel1 = new char[in_spec1.data.PixelSize()];
  char *pixel2 = new char[in_spec2.data.PixelSize()];
  char *pixel3 = new char[out_spec.data.PixelSize()];
  void *state = initial;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      in_spec1.is.read((char *) pixel1, in_spec1.data.PixelSize());
      in_spec2.is.read((char *) pixel2, in_spec2.data.PixelSize());
      combiner->CombineStateful(i, j, rows, cols, pixel1, in_spec1.data, pixel2, in_spec2.data, pixel3, out_spec.data, state);
      out_spec.os.write((char *) pixel3, out_spec.data.PixelSize());
    }
  return state;
}

bool ColorizerFactory::Register(const std::string name, ColorizerBuilderInstanceGenerator create_function) {
  auto it = ColorizerFactory::create_functions.find(name);
  if(it == ColorizerFactory::create_functions.end())
  {
    ColorizerFactory::create_functions[name] = create_function;
    return true;
  }
  return false;
}

std::unique_ptr<ColorizerBuilder> ColorizerFactory::Create(const std::string& name) {
  auto it = ColorizerFactory::create_functions.find(name);
  if(it != ColorizerFactory::create_functions.end()) 
    return it->second();
  return nullptr;
}

ColorizerFactory& ColorizerFactory::get()
{
	static ColorizerFactory instance;
	return instance;
}

void ToPPM(InputSpecifier in_spec, std::ostream& os, Colorizer *component) {
  int rows, cols;
  in_spec.is.read((char *) &rows, sizeof(int));
  in_spec.is.read((char *) &cols, sizeof(int));
  in_spec.is.read((char *) &in_spec.data.dim, sizeof(int));
  os << "P6\n" << cols << " " << rows << "\n255\n";
  char *pixel = new char[in_spec.data.PixelSize()];
  float *rgb = new float[3];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      in_spec.is.read((char *) pixel, in_spec.data.PixelSize());
      component->ToRGB(pixel, in_spec.data, rgb);
      os << (unsigned char)(std::max(float(0), std::min(float(1), rgb[0])) * 255) <<
             (unsigned char)(std::max(float(0), std::min(float(1), rgb[1])) * 255) <<
             (unsigned char)(std::max(float(0), std::min(float(1), rgb[2])) * 255);
    }
}

} // namespace binary
} // namespace image
} // namespace graphics