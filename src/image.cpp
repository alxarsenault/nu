#include "nu/image.h"

namespace nu {
#ifdef __APPLE__
image::image(const char* filename) {
  CGDataProviderRef data_provider = CGDataProviderCreateWithFilename(filename);
  _image = CGImageCreateWithPNGDataProvider(data_provider, nullptr, true, kCGRenderingIntentDefault);
  CGDataProviderRelease(data_provider);
}

image::image(const image& img) { _image = CGImageCreateCopy(img._image); }

image::image(image&& img) {
  _image = img._image;
  img._image = nullptr;
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

  return *this;
}

image& image::operator=(image&& img) {
  if (_image) {
    CGImageRelease(_image);
    _image = nullptr;
  }
  _image = img._image;
  img._image = nullptr;
  return *this;
}
#endif // __APPLE__
} // namespace nu.
