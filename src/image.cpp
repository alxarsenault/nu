#include "nu/image.h"

namespace nu {
#ifdef __APPLE__
image::image(const char* filename) {
  CGDataProviderRef data_provider = CGDataProviderCreateWithFilename(filename);
  _image = CGImageCreateWithPNGDataProvider(data_provider, nullptr, true, kCGRenderingIntentDefault);
  CGDataProviderRelease(data_provider);
}

image::image(const char* filename, float scale_factor)
    : image(filename) {
  _scale_factor = scale_factor;
}

image::image(const image& img) {
  _image = CGImageCreateCopy(img._image);
  _scale_factor = img._scale_factor;
}

image::image(image&& img) {
  _image = img._image;
  _scale_factor = img._scale_factor;
  img._image = nullptr;
  img._scale_factor = 1;
}

image::~image() {
  if (_image) {
    CGImageRelease(_image);
  }
}

image& image::operator=(const image& img) {
  if (_image) {
    CGImageRelease(_image);
    _image = nullptr;
  }
  _image = CGImageCreateCopy(img._image);
  _scale_factor = img._scale_factor;

  return *this;
}

image& image::operator=(image&& img) {
  if (_image) {
    CGImageRelease(_image);
    _image = nullptr;
  }
  _image = img._image;
  _scale_factor = img._scale_factor;
  img._image = nullptr;
  img._scale_factor = 1;
  return *this;
}

bool image::is_valid() const { return _image != nullptr; }

nu::fsize image::get_size() const {
  return nu::fsize((nu::fsize::value_type)CGImageGetWidth(_image), (nu::fsize::value_type)CGImageGetHeight(_image))
      * (1.0f / _scale_factor);
}
#endif // __APPLE__
} // namespace nu.
