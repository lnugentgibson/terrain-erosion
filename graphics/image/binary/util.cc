#include "graphics/color/color.h"
#include "graphics/image/binary/binimg.h"
#include "graphics/image/binary/binimg_registration.h"

namespace graphics {
namespace image {
namespace binary {

class ScalingTransformer : public SimpleTransformer {
  float from_min, from_max, from_range, to_min, to_max, to_range;
 public:
  ScalingTransformer(float fn, float fx, float tn, float tx) : from_min(fn), from_max(fx), from_range(from_max - from_min), to_min(tn), to_max(tx), to_range(to_max - to_min) {}
  void Transform(
    int i, int j, int rows, int cols,
    const void *pixel1, DataSpecifier in_spec,
    void *pixel2, DataSpecifier out_spec) override;
};
  
class ScalingTransformerBuilder : public TransformerBuilder {
  float fn, fx, tn, tx;
 public:
  std::unique_ptr<Transformer> operator ()() override {
    return std::unique_ptr<Transformer>(new ScalingTransformer(fn, fx, tn, tx));
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override;
};

class MinMaxAccumulator : public image::binary::Accumulator {
  void Aggregate(int i, int j, int rows, int cols, const void *pixel, DataSpecifier in_spec, int n, void *aggregate) override;
};
  
class MinMaxAccumulatorBuilder : public AccumulatorBuilder {
 public:
  std::unique_ptr<Accumulator> operator ()() override {
    return std::unique_ptr<Accumulator>(new MinMaxAccumulator());
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

class SumCombiner : public graphics::image::binary::StatelessCombiner {
  float weight_a, weight_b;
 public:
  SumCombiner(float wa, float wb) : weight_a(wa), weight_b(wb) {}
  void Combine(
    int i, int j, int rows, int cols,
    const void *pixel1, DataSpecifier in_spec1,
    const void *pixel2, DataSpecifier in_spec2,
    void *pixel3, DataSpecifier out_spec) override;
};
  
class SumCombinerBuilder : public CombinerBuilder {
  float wa, wb;
 public:
  std::unique_ptr<Combiner> operator ()() override {
    return std::unique_ptr<Combiner>(new SumCombiner(wa, wb));
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override;
};

class GrayscaleColorizer : public graphics::image::binary::Colorizer {
 public:
  void ToRGB(const void *pixel, DataSpecifier in_spec, float *rgb) override;
};
  
class GrayscaleColorizerBuilder : public ColorizerBuilder {
 public:
  std::unique_ptr<Colorizer> operator ()() override {
    return std::unique_ptr<Colorizer>(new GrayscaleColorizer());
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

class SignedColorizer : public graphics::image::binary::Colorizer {
 public:
  void ToRGB(const void *pixel, DataSpecifier in_spec, float *rgb) override;
};
  
class SignedColorizerBuilder : public ColorizerBuilder {
 public:
  std::unique_ptr<Colorizer> operator ()() override {
    return std::unique_ptr<Colorizer>(new SignedColorizer());
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

class ColorColorizer : public graphics::image::binary::Colorizer {
 public:
  void ToRGB(const void *pixel, DataSpecifier in_spec, float *rgb) override;
};
  
class ColorColorizerBuilder : public ColorizerBuilder {
 public:
  std::unique_ptr<Colorizer> operator ()() override {
    return std::unique_ptr<Colorizer>(new ColorColorizer());
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

class VectorColorizer : public graphics::image::binary::Colorizer {
 public:
  void ToRGB(const void *pixel, DataSpecifier in_spec, float *rgb) override;
};
  
class VectorColorizerBuilder : public ColorizerBuilder {
 public:
  std::unique_ptr<Colorizer> operator ()() override {
    return std::unique_ptr<Colorizer>(new VectorColorizer());
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

class DirectionColorizer : public graphics::image::binary::Colorizer {
 public:
  void ToRGB(const void *pixel, DataSpecifier in_spec, float *rgb) override;
};
  
class DirectionColorizerBuilder : public ColorizerBuilder {
 public:
  std::unique_ptr<Colorizer> operator ()() override {
    return std::unique_ptr<Colorizer>(new DirectionColorizer());
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

void ScalingTransformer::Transform(
  int i, int j, int rows, int cols,
  const void *pixel1, DataSpecifier in_spec,
  void *pixel2, DataSpecifier out_spec) {
  *static_cast<float *>(pixel2) = (static_cast<const float *>(pixel1)[0] - from_min) / from_range * to_range + to_min;
}

bool ScalingTransformerBuilder::SetFloatParam(const std::string& param, float value) {
  if(param == "fn") {
    fn = value;
    return true;
  }
  if(param == "fx") {
    fx = value;
    return true;
  }
  if(param == "tn") {
    tn = value;
    return true;
  }
  if(param == "tx") {
    tx = value;
    return true;
  }
  return false;
}

namespace TransformerRegistrations {
	TransformerFactoryRegistration<ScalingTransformerBuilder> _ScalingTransformerBuilder("ScalingTransformer");
}

void MinMaxAccumulator::Aggregate(int i, int j, int rows, int cols, const void *pixel, DataSpecifier in_spec, int n, void *aggregate) {
  float v = static_cast<const float *>(pixel)[0];
  auto minmax = static_cast<std::pair<float, float>*>(aggregate);
  if(n == 0) {
    minmax->first = v;
    minmax->second = v;
  } else {
    if(v < minmax->first) {
      minmax->first = v;
    }
    if(v > minmax->second) {
      minmax->second = v;
    }
  }
}

namespace AccumulatorRegistrations {
	AccumulatorFactoryRegistration<MinMaxAccumulatorBuilder> _MinMaxAccumulatorBuilder("MinMaxAccumulator");
}

void SumCombiner::Combine(
  int i, int j, int rows, int cols,
  const void *pixel1, DataSpecifier in_spec1,
  const void *pixel2, DataSpecifier in_spec2,
  void *pixel3, DataSpecifier out_spec) {
  *static_cast<float *>(pixel3) = weight_a * static_cast<const float *>(pixel1)[0] + weight_b * static_cast<const float *>(pixel2)[0];
}

bool SumCombinerBuilder::SetFloatParam(const std::string& param, float value) {
  if(param == "wa") {
    wa = value;
    return true;
  }
  if(param == "wb") {
    wb = value;
    return true;
  }
  return false;
}

namespace CombinerRegistrations {
	CombinerFactoryRegistration<SumCombinerBuilder> _SumCombinerBuilder("SumCombiner");
}

void GrayscaleColorizer::ToRGB(const void *pixel, DataSpecifier in_spec, float *rgb) {
  auto *v = static_cast<const float*>(pixel);
  rgb[0] = *v;
  rgb[1] = *v;
  rgb[2] = *v;
}

void SignedColorizer::ToRGB(const void *pixel, DataSpecifier in_spec, float *rgb) {
  auto *v = static_cast<const float*>(pixel);
  rgb[0] = *v * 0.5 + 0.5;
  rgb[1] = *v * 0.5 + 0.5;
  rgb[2] = *v * 0.5 + 0.5;
}

void ColorColorizer::ToRGB(const void *pixel, DataSpecifier in_spec, float *rgb) {
  auto *v = static_cast<const Color*>(pixel);
  rgb[0] = v->r();
  rgb[1] = v->g();
  rgb[2] = v->b();
}

void VectorColorizer::ToRGB(const void *pixel, DataSpecifier in_spec, float *rgb) {
  auto *v = static_cast<const float*>(pixel);
  rgb[0] = *v;
  rgb[1] = in_spec.dim > 0 ? *v : 0.0;
  rgb[2] = in_spec.dim > 1 ? *v : 0.0;
}

void DirectionColorizer::ToRGB(const void *pixel, DataSpecifier in_spec, float *rgb) {
  auto *v = static_cast<const float*>(pixel);
  rgb[0] = *v * 0.5 + 0.5;
  rgb[1] = in_spec.dim > 0 ? *v * 0.5 + 0.5 : 0.5;
  rgb[2] = in_spec.dim > 1 ? *v * 0.5 + 0.5 : 0.5;
}

namespace ColorizerRegistrations {
	ColorizerFactoryRegistration<GrayscaleColorizerBuilder> _GrayscaleColorizerBuilder("GrayscaleColorizer");
	ColorizerFactoryRegistration<SignedColorizerBuilder> _SignedColorizerBuilder("SignedColorizer");
	ColorizerFactoryRegistration<ColorColorizerBuilder> _ColorColorizerBuilder("ColorColorizer");
	ColorizerFactoryRegistration<VectorColorizerBuilder> _VectorColorizerBuilder("VectorColorizer");
	ColorizerFactoryRegistration<DirectionColorizerBuilder> _DirectionColorizerBuilder("DirectionColorizer");
}

} // namespace binary
} // namespace image
} // namespace graphics