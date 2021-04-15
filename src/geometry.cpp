#include "nu/geometry.h"
#include "nu/path.h"
#include "fst/unused.h"

namespace nu::geometry {
//
// Filled rectangle.
//
class filled_rectangle::content {
public:
  content(const nu::frect& rect, const nu::color& color)
      : _rect(rect)
      , _color(color) {
    _path.add_rect(_rect);
  }

  ~content() = default;

  inline bool intersect(const nu::frect& rect) const { return _rect.intersect(rect); }

  inline void draw(nu::context& g) const {
    g.set_color(_color);
    g.fill_path(_path);
  }

  inline void set_rect(const nu::frect& rect) {
    _rect = rect;
    _path = nu::path();
    _path.add_rect(_rect);
  }

  inline void set_color(const nu::color& color) { _color = color; }
  inline const nu::frect& get_rect() const { return _rect; }
  inline const nu::color& get_color() const { return _color; }

private:
  nu::path _path;
  nu::frect _rect;
  nu::color _color;
};

//
// Stroked rectangle.
//
class stroked_rectangle::content {
public:
  content(const nu::frect& rect, const nu::color& color, float line_width)
      : _rect(rect)
      , _color(color)
      , _line_width(line_width) {
    _path.add_rect(_rect);
  }

  ~content() = default;

  inline bool intersect(const nu::frect& rect) const { return _rect.intersect(rect); }

  inline void draw(nu::context& g) const {
    g.set_line_width(_line_width);
    g.set_stroke_color(_color);
    g.draw_path(_path);
  }

  inline void set_rect(const nu::frect& rect) {
    _rect = rect;
    _path = nu::path();
    _path.add_rect(_rect);
  }

  inline void set_color(const nu::color& color) { _color = color; }
  inline void set_line_width(float w) { _line_width = w; }
  inline const nu::frect& get_rect() const { return _rect; }
  inline const nu::color& get_color() const { return _color; }
  inline float get_line_width() const { return _line_width; }

private:
  nu::path _path;
  nu::frect _rect;
  nu::color _color;
  float _line_width;
};

//
// Rectangle and contour.
//
class filled_and_stroked_rectangle::content {
public:
  content(const nu::frect& rect, const nu::color& fill_color, const nu::color& contour_color, float line_width)
      : _rect(rect)
      , _fill_color(fill_color)
      , _contour_color(contour_color)
      , _line_width(line_width) {
    _path.add_rect(_rect);
  }

  ~content() = default;

  inline bool intersect(const nu::frect& rect) const { return _rect.intersect(rect); }

  inline void draw(nu::context& g) const {
    g.set_color(_fill_color);
    g.fill_path(_path);

    g.set_line_width(_line_width);
    g.set_stroke_color(_contour_color);
    g.draw_path(_path);
  }

  inline void set_rect(const nu::frect& rect) {
    _rect = rect;
    _path = nu::path();
    _path.add_rect(_rect);
  }

  inline void set_fill_color(const nu::color& color) { _fill_color = color; }
  inline void set_contour_color(const nu::color& color) { _contour_color = color; }
  inline void set_line_width(float w) { _line_width = w; }
  inline const nu::frect& get_rect() const { return _rect; }
  inline const nu::color& get_fill_color() const { return _fill_color; }
  inline const nu::color& get_contour_color() const { return _contour_color; }
  inline float get_line_width() const { return _line_width; }

private:
  nu::path _path;
  nu::frect _rect;
  nu::color _fill_color;
  nu::color _contour_color;
  float _line_width;
};

//
// Image.
//
class image::content {
public:
  content(fst::not_null<nu::image*> img, const nu::fpoint& pos)
      : _image(img)
      , _rect(pos, img->get_size()) {}

  ~content() = default;

  inline bool intersect(const nu::frect& rect) const { return _rect.intersect(rect); }

  inline void draw(nu::context& g) const { g.draw_image(*_image, _rect); }

private:
  nu::image* _image;
  nu::frect _rect;
};

//
// Image in rect.
//
class image_in_rect::content {
public:
  content(fst::not_null<nu::image*> img, const nu::frect& rect)
      : _image(img)
      , _rect(rect) {}

  ~content() = default;

  inline bool intersect(const nu::frect& rect) const { return _rect.intersect(rect); }

  inline void draw(nu::context& g) const { g.draw_image(*_image, _rect); }

  inline void set_rect(const nu::frect& rect) { _rect = rect; }
  inline const nu::frect& get_rect() const { return _rect; }

private:
  nu::image* _image;
  nu::frect _rect;
};

//
// Stroked path.
//
class stroked_path::content {
public:
  content(nu::path&& path, const nu::color& color, float line_width)
      : _path(std::move(path))
      , _color(color)
      , _line_width(line_width) {}

  ~content() = default;

  inline bool intersect(const nu::frect& rect) const { return _path.get_bounding_box().intersect(rect); }

  inline void draw(nu::context& g) const {
    g.set_color(_color);
    g.set_line_width(_line_width);
    g.draw_path(_path);
  }

  inline void set(nu::path&& p, const nu::color& color, float line_width) {
    _path = std::move(p);
    _color = color;
    _line_width = line_width;
  }

  inline void set_path(nu::path&& p) { _path = std::move(p); }

  inline void set_color(const nu::color& color) { _color = color; }
  inline void set_line_width(float w) { _line_width = w; }

  inline const nu::path& get_path() const { return _path; }
  inline const nu::color& get_color() const { return _color; }
  inline float get_line_width() const { return _line_width; }

private:
  nu::path _path;
  nu::color _color;
  float _line_width;
};

//
// Filled path.
//
class filled_path::content {
public:
  content(nu::path&& path, const nu::color& color)
      : _path(std::move(path))
      , _color(color) {}

  ~content() = default;

  inline bool intersect(const nu::frect& rect) const { return _path.get_bounding_box().intersect(rect); }

  inline void draw(nu::context& g) const {
    g.set_color(_color);
    g.fill_path(_path);
  }

  inline void set(nu::path&& p, const nu::color& color) {
    _path = std::move(p);
    _color = color;
  }

  inline void set_path(nu::path&& p) { _path = std::move(p); }

  inline void set_color(const nu::color& color) { _color = color; }

  inline const nu::path& get_path() const { return _path; }
  inline const nu::color& get_color() const { return _color; }

private:
  nu::path _path;
  nu::color _color;
};

//
// Path and contour.
//
class filled_and_stroked_path::content {
public:
  content(nu::path&& p, const nu::color& fill_color, const nu::color& contour_color, float line_width)
      : _path(std::move(p))
      , _fill_color(fill_color)
      , _contour_color(contour_color)
      , _line_width(line_width) {}

  ~content() = default;

  inline bool intersect(const nu::frect& rect) const { return _path.get_bounding_box().intersect(rect); }

  inline void draw(nu::context& g) const {
    g.set_color(_fill_color);
    g.fill_path(_path);

    g.set_line_width(_line_width);
    g.set_stroke_color(_contour_color);
    g.draw_path(_path);
  }

  inline void set_path(nu::path&& p) { _path = std::move(p); }
  inline void set_fill_color(const nu::color& color) { _fill_color = color; }
  inline void set_contour_color(const nu::color& color) { _contour_color = color; }
  inline void set_line_width(float w) { _line_width = w; }
  inline const nu::path& get_path() const { return _path; }
  inline const nu::color& get_fill_color() const { return _fill_color; }
  inline const nu::color& get_contour_color() const { return _contour_color; }
  inline float get_line_width() const { return _line_width; }

private:
  nu::path _path;
  nu::color _fill_color;
  nu::color _contour_color;
  float _line_width;
};

#define GENERIC_CONTENT_IMPLEMENTATION(class_name)                                                                     \
  class_name::~class_name() {                                                                                          \
    fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");              \
  }                                                                                                                    \
  bool class_name::intersect(const nu::frect& rect) const { return _content->intersect(rect); }                        \
  void class_name::draw(nu::context& g) const { _content->draw(g); }                                                   \
  void class_name::destroy(memory_pool& pool) {                                                                        \
    _content->~content();                                                                                              \
    pool.free(this);                                                                                                   \
  }                                                                                                                    \
  void* class_name::operator new(size_t, memory_pool& pool) {                                                          \
    void* ret = pool.malloc(sizeof(class_name) + sizeof(content));                                                     \
    auto obj = reinterpret_cast<class_name*>(ret);                                                                     \
    obj->_content = reinterpret_cast<content*>(obj + 1);                                                               \
    return ret;                                                                                                        \
  }

//
// Rectangle.
//
GENERIC_CONTENT_IMPLEMENTATION(filled_rectangle);
filled_rectangle::filled_rectangle(const nu::frect& rect, const nu::color& color) {
  fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");
  new (_content) content(rect, color);
}
void filled_rectangle::set(const nu::frect& rect, const nu::color& color) { *_content = content(rect, color); }
void filled_rectangle::set_rect(const nu::frect& rect) { _content->set_rect(rect); }
void filled_rectangle::set_color(const nu::color& color) { _content->set_color(color); }
nu::frect filled_rectangle::get_rect() const { return _content->get_rect(); }
nu::color filled_rectangle::get_color() const { return _content->get_color(); }

//
// Rectangle contour.
//
GENERIC_CONTENT_IMPLEMENTATION(stroked_rectangle);
stroked_rectangle::stroked_rectangle(const nu::frect& rect, const nu::color& color, float line_width) {
  fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");
  new (_content) content(rect, color, line_width);
}
void stroked_rectangle::set_rect(const nu::frect& rect) { _content->set_rect(rect); }
void stroked_rectangle::set_color(const nu::color& color) { _content->set_color(color); }
void stroked_rectangle::set_line_width(float w) { _content->set_line_width(w); }
nu::frect stroked_rectangle::get_rect() const { return _content->get_rect(); }
nu::color stroked_rectangle::get_color() const { return _content->get_color(); }
float stroked_rectangle::get_line_width() const { return _content->get_line_width(); }

//
// Rectangle and contour.
//
GENERIC_CONTENT_IMPLEMENTATION(filled_and_stroked_rectangle);
filled_and_stroked_rectangle::filled_and_stroked_rectangle(
    const nu::frect& rect, const nu::color& fill_color, const nu::color& contour_color, float line_width) {
  fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");
  new (_content) content(rect, fill_color, contour_color, line_width);
}
void filled_and_stroked_rectangle::set_rect(const nu::frect& rect) { _content->set_rect(rect); }
void filled_and_stroked_rectangle::set_fill_color(const nu::color& color) { _content->set_fill_color(color); }
void filled_and_stroked_rectangle::set_contour_color(const nu::color& color) { _content->set_contour_color(color); }
void filled_and_stroked_rectangle::set_line_width(float w) { _content->set_line_width(w); }
nu::frect filled_and_stroked_rectangle::get_rect() const { return _content->get_rect(); }
nu::color filled_and_stroked_rectangle::get_fill_color() const { return _content->get_fill_color(); }
nu::color filled_and_stroked_rectangle::get_contour_color() const { return _content->get_contour_color(); }
float filled_and_stroked_rectangle::get_line_width() const { return _content->get_line_width(); }

//
// Image.
//
GENERIC_CONTENT_IMPLEMENTATION(image);
image::image(fst::not_null<nu::image*> img, const nu::fpoint& pos) {
  fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");
  new (_content) content(img, pos);
}

//
// Image in rect.
//
GENERIC_CONTENT_IMPLEMENTATION(image_in_rect);
image_in_rect::image_in_rect(fst::not_null<nu::image*> img, const nu::frect& rect) {
  fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");
  new (_content) content(img, rect);
}
void image_in_rect::set_rect(const nu::frect& rect) { _content->set_rect(rect); }
nu::frect image_in_rect::get_rect() const { return _content->get_rect(); }

//
// Stroked path.
//
GENERIC_CONTENT_IMPLEMENTATION(stroked_path);
stroked_path::stroked_path(nu::path&& p, const nu::color& color, float line_width) {
  fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");
  new (_content) content(std::move(p), color, line_width);
}
void stroked_path::set(nu::path&& p, const nu::color& color, float line_width) {
  _content->set(std::move(p), color, line_width);
}
void stroked_path::set_path(nu::path&& p) { _content->set_path(std::move(p)); }
void stroked_path::set_color(const nu::color& color) { _content->set_color(color); }
void stroked_path::set_line_width(float w) { _content->set_line_width(w); }
const nu::path& stroked_path::get_path() const { return _content->get_path(); }
nu::color stroked_path::get_color() const { return _content->get_color(); }
float stroked_path::get_line_width() const { return _content->get_line_width(); }

//
// Filled path.
//
GENERIC_CONTENT_IMPLEMENTATION(filled_path);
filled_path::filled_path(nu::path&& p, const nu::color& color) {
  fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");
  new (_content) content(std::move(p), color);
}
void filled_path::set(nu::path&& p, const nu::color& color) { _content->set(std::move(p), color); }
void filled_path::set_path(nu::path&& p) { _content->set_path(std::move(p)); }
void filled_path::set_color(const nu::color& color) { _content->set_color(color); }
const nu::path& filled_path::get_path() const { return _content->get_path(); }
nu::color filled_path::get_color() const { return _content->get_color(); }

//
// Path and contour.
//
GENERIC_CONTENT_IMPLEMENTATION(filled_and_stroked_path);
filled_and_stroked_path::filled_and_stroked_path(
    nu::path&& p, const nu::color& fill_color, const nu::color& contour_color, float line_width) {
  fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");
  new (_content) content(std::move(p), fill_color, contour_color, line_width);
}
void filled_and_stroked_path::set_path(nu::path&& p) { _content->set_path(std::move(p)); }
void filled_and_stroked_path::set_fill_color(const nu::color& color) { _content->set_fill_color(color); }
void filled_and_stroked_path::set_contour_color(const nu::color& color) { _content->set_contour_color(color); }
void filled_and_stroked_path::set_line_width(float w) { _content->set_line_width(w); }
const nu::path& filled_and_stroked_path::get_path() const { return _content->get_path(); }
nu::color filled_and_stroked_path::get_fill_color() const { return _content->get_fill_color(); }
nu::color filled_and_stroked_path::get_contour_color() const { return _content->get_contour_color(); }
float filled_and_stroked_path::get_line_width() const { return _content->get_line_width(); }

} // namespace nu::geometry.
