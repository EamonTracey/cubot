#ifndef DETECTION_H
#define DETECTION_H

#include <cstdint>
#include <memory>
#include <vector>

enum struct Color { WHITE, YELLOW, GREEN, BLUE, RED, ORANGE };

class ColorDetector {
  public:
    ColorDetector();
    ~ColorDetector();

    // Detect the most dominant color in the image.
    enum Color DetectColor(const std::vector<float> &image) const;
    enum Color DetectColor(const float *pixel_data,
                           size_t pixel_data_len) const;

  private:
    // We use the PImpl idiom for onnxruntime types to strengthen language
    // interoperability (e.g., Swift).
    struct ONNXImpl;
    std::shared_ptr<struct ONNXImpl> onnx_impl_;

    // Extract the color detection feature vector from an RGB image. The RGB
    // components must be normalized to the range [0, 1].
    static std::vector<float> ExtractFeatureVector(const float *pixel_data,
                                                   size_t pixel_data_len);

    // Convert an sRGB pixel to a CIELAB pixel (D65 illuminant). The RGB
    // components must be normalized to the range [0, 1]. The LAB components are
    // normalized to the ranges [0, 100], [-128, 127], [-128, 127],
    // respectively.
    static std::vector<float> RGBToLAB(float r, float g, float b);
};

#endif
