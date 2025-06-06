#include <cstdint>
#include <stdexcept>
#include <vector>

#include <onnxruntime/onnxruntime_cxx_api.h>

#include "detection.h"
#include "models.h"

struct ColorDetector::ONNXImpl {
    Ort::Env env;
    Ort::MemoryInfo memory_info;
    std::unique_ptr<Ort::Session> session;

    ONNXImpl()
        : env(ORT_LOGGING_LEVEL_ERROR, "ColorDetector"),
          memory_info(Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator,
                                                 OrtMemTypeDefault)) {
        Ort::SessionOptions session_options;
        session = std::make_unique<Ort::Session>(
            env, models_logistic_regression_onnx,
            models_logistic_regression_onnx_len, session_options);
    }
};

ColorDetector::ColorDetector() : onnx_impl_(std::make_shared<ONNXImpl>()) {}

ColorDetector::~ColorDetector() = default;

enum Color ColorDetector::DetectColor(
    const std::vector<std::vector<std::vector<float>>> &image) const {
    return DetectColor(ExtractFeatureVector(image));
}

enum Color ColorDetector::DetectColor(const float *pixel_data,
                                      size_t pixel_data_len) const {
    return DetectColor(ExtractFeatureVector(pixel_data, pixel_data_len));
}

enum Color
ColorDetector::DetectColor(const std::vector<float> &features) const {
    // Extract the feature vector and convert it to an ONNX tensor.
    std::vector<int64_t> shape = {1, static_cast<int64_t>(features.size())};
    Ort::Value input = {Ort::Value::CreateTensor<float>(
        onnx_impl_->memory_info, const_cast<float *>(features.data()),
        features.size(), shape.data(), shape.size())};

    // Run the model on the input tensor. The model has 1 input and
    // 1 output.
    Ort::AllocatorWithDefaultOptions allocator;
    auto input_name = onnx_impl_->session->GetInputNameAllocated(0, allocator);
    auto output_name =
        onnx_impl_->session->GetOutputNameAllocated(0, allocator);
    const char *input_names[] = {input_name.get()};
    const char *output_names[] = {output_name.get()};
    auto outputs = onnx_impl_->session->Run(
        Ort::RunOptions{nullptr}, input_names, &input, 1, output_names, 1);
    Ort::Value &output = outputs[0];

    // The model outputs a single-character string that corresponds to the
    // color. We must retrieve this output and interpret it as an enum Color.
    char color;
    size_t offset = 0;
    (void)Ort::GetApi().GetStringTensorContent(output, &color, 1, &offset, 1);
    switch (color) {
    case 'w':
        return Color::WHITE;
    case 'y':
        return Color::YELLOW;
    case 'g':
        return Color::GREEN;
    case 'b':
        return Color::BLUE;
    case 'r':
        return Color::RED;
    case 'o':
        return Color::ORANGE;
    default:
        // This error will never be thrown if our models are correct.
        throw std::runtime_error("Model output an unknown value.");
    }
}

std::vector<float> ColorDetector::ExtractFeatureVector(
    const std::vector<std::vector<std::vector<float>>> &image) {
    size_t n_pixels = image.size() * image[0].size();

    float l_sum = 0;
    float l_square_sum = 0;
    float a_sum = 0;
    float a_square_sum = 0;
    float b_sum = 0;
    float b_square_sum = 0;
    for (auto row : image) {
        for (auto pixel : row) {
            pixel = RGBToLAB(pixel);
            l_sum += pixel[0];
            l_square_sum += pixel[0] * pixel[0];
            a_sum += pixel[1];
            a_square_sum += pixel[1] * pixel[1];
            b_sum += pixel[2];
            b_square_sum += pixel[2] * pixel[2];
        }
    }
    float l_mean = l_sum / n_pixels;
    float a_mean = a_sum / n_pixels;
    float b_mean = b_sum / n_pixels;
    float l_stdev = l_square_sum / n_pixels - l_mean * l_mean;
    float a_stdev = a_square_sum / n_pixels - a_mean * a_mean;
    float b_stdev = b_square_sum / n_pixels - b_mean * b_mean;

    return {l_mean, a_mean, b_mean, l_stdev, a_stdev, b_stdev};
}

std::vector<float> ColorDetector::ExtractFeatureVector(const float *pixel_data,
                                                       size_t pixel_data_len) {
    size_t n_pixels = pixel_data_len;

    float l_sum = 0;
    float l_square_sum = 0;
    float a_sum = 0;
    float a_square_sum = 0;
    float b_sum = 0;
    float b_square_sum = 0;
    for (size_t i = 0; i < pixel_data_len; i += 3) {
        auto pixel =
            RGBToLAB({pixel_data[i], pixel_data[i + 1], pixel_data[i + 2]});
        l_sum += pixel[0];
        l_square_sum += pixel[0] * pixel[0];
        a_sum += pixel[1];
        a_square_sum += pixel[1] * pixel[1];
        b_sum += pixel[2];
        b_square_sum += pixel[2] * pixel[2];
    }
    float l_mean = l_sum / n_pixels;
    float a_mean = a_sum / n_pixels;
    float b_mean = b_sum / n_pixels;
    float l_stdev = l_square_sum / n_pixels - l_mean * l_mean;
    float a_stdev = a_square_sum / n_pixels - a_mean * a_mean;
    float b_stdev = b_square_sum / n_pixels - b_mean * b_mean;

    return {l_mean, a_mean, b_mean, l_stdev, a_stdev, b_stdev};
}

/* CHAT-GPT GENERATED BEGIN */
std::vector<float> ColorDetector::RGBToLAB(const std::vector<float> &pixel) {
    float r = pixel[0];
    float g = pixel[1];
    float b = pixel[2];

    // Step 1: Gamma correction (sRGB nonlinear to linear RGB)
    // Formula defined by the sRGB IEC 61966-2-1:1999 standard
    // Reference: https://en.wikipedia.org/wiki/SRGB#The_sRGB_transfer_function
    auto gamma_correct = [](float c) {
        return (c > 0.04045f) ? std::powf((c + 0.055f) / 1.055f, 2.4f)
                              : c / 12.92f;
    };

    r = gamma_correct(r);
    g = gamma_correct(g);
    b = gamma_correct(b);

    // Step 2: Convert linear RGB to XYZ using the D65 standard illuminant
    // matrix The matrix here converts linear RGB in sRGB space to CIE 1931 XYZ
    // Source:
    // https://www.image-engineering.de/library/technotes/958-how-to-convert-between-srgb-and-ciexyz
    float x = r * 0.4124564f + g * 0.3575761f + b * 0.1804375f;
    float y = r * 0.2126729f + g * 0.7151522f + b * 0.0721750f;
    float z = r * 0.0193339f + g * 0.1191920f + b * 0.9503041f;

    // Step 3: Scale XYZ values to match the standard scale ([0, 100])
    // This is conventional in CIE color space representations
    x *= 100.0f;
    y *= 100.0f;
    z *= 100.0f;

    // Step 4: Normalize XYZ by D65 reference white point
    // These constants represent the tristimulus values for the D65 illuminant
    // Source: https://en.wikipedia.org/wiki/Illuminant_D65
    constexpr float Xn = 95.047f;
    constexpr float Yn = 100.000f;
    constexpr float Zn = 108.883f;

    // Step 5: Nonlinear transformation to Lab using the CIE 1976 formula
    // f(t) is defined piecewise to ensure continuity and differentiability
    // Source:
    // https://en.wikipedia.org/wiki/CIELAB_color_space#Forward_transformation
    auto f = [](float t) {
        return (t > 0.008856f) ? std::cbrtf(t) : (7.787f * t + 16.0f / 116.0f);
    };

    float fx = f(x / Xn);
    float fy = f(y / Yn);
    float fz = f(z / Zn);

    // Step 6: Compute Lab components from transformed XYZ
    // L* is lightness, a* is green–red, b* is blue–yellow
    // The conditional in L* handles low-light compensation using a linear
    // approximation Source for the constant 903.3:
    // https://www.color.org/chardata/rgb/cielab.xalter
    float L =
        (y / Yn > 0.008856f) ? (116.0f * fy - 16.0f) : (903.3f * (y / Yn));

    float a = 500.0f * (fx - fy);
    float b_ = 200.0f * (fy - fz);

    // Return Lab as a vector
    return {L, a, b_};
}
///* CHAT-GPT GENERATED END */
