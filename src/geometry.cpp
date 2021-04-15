#include "nu/geometry.h"
#include "nu/path.h"
#include "fst/unused.h"

namespace nu::geometry {
// Rectangle.
class rectangle::content {
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
  inline nu::frect get_rect() const { return _rect; }
  inline nu::color get_color() const { return _color; }

private:
  nu::path _path;
  nu::frect _rect;
  nu::color _color;
};

// Rectangle contour.
class rectangle_contour::content {
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
  inline nu::frect get_rect() const { return _rect; }
  inline nu::color get_color() const { return _color; }
  inline float get_line_width() const { return _line_width; }

private:
  nu::path _path;
  nu::frect _rect;
  nu::color _color;
  float _line_width;
};

// Rectangle and contour.
class rectangle_and_contour::content {
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
  inline nu::frect get_rect() const { return _rect; }
  inline nu::color get_fill_color() const { return _fill_color; }
  inline nu::color get_contour_color() const { return _contour_color; }
  inline float get_line_width() const { return _line_width; }

private:
  nu::path _path;
  nu::frect _rect;
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
GENERIC_CONTENT_IMPLEMENTATION(rectangle);
rectangle::rectangle(const nu::frect& rect, const nu::color& color) {
  fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");
  new (_content) content(rect, color);
}
void rectangle::set_rect(const nu::frect& rect) { _content->set_rect(rect); }
void rectangle::set_color(const nu::color& color) { _content->set_color(color); }
nu::frect rectangle::get_rect() const { return _content->get_rect(); }
nu::color rectangle::get_color() const { return _content->get_color(); }

//
// Rectangle contour.
//
GENERIC_CONTENT_IMPLEMENTATION(rectangle_contour);
rectangle_contour::rectangle_contour(const nu::frect& rect, const nu::color& color, float line_width) {
  fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");
  new (_content) content(rect, color, line_width);
}
void rectangle_contour::set_rect(const nu::frect& rect) { _content->set_rect(rect); }
void rectangle_contour::set_color(const nu::color& color) { _content->set_color(color); }
void rectangle_contour::set_line_width(float w) { _content->set_line_width(w); }
nu::frect rectangle_contour::get_rect() const { return _content->get_rect(); }
nu::color rectangle_contour::get_color() const { return _content->get_color(); }
float rectangle_contour::get_line_width() const { return _content->get_line_width(); }

//
// Rectangle and contour.
//
GENERIC_CONTENT_IMPLEMENTATION(rectangle_and_contour);
rectangle_and_contour::rectangle_and_contour(
    const nu::frect& rect, const nu::color& fill_color, const nu::color& contour_color, float line_width) {
  fst_assert(reinterpret_cast<void*>(_content) == reinterpret_cast<void*>(this + 1), "You fucked up.");
  new (_content) content(rect, fill_color, contour_color, line_width);
}
void rectangle_and_contour::set_rect(const nu::frect& rect) { _content->set_rect(rect); }
void rectangle_and_contour::set_fill_color(const nu::color& color) { _content->set_fill_color(color); }
void rectangle_and_contour::set_contour_color(const nu::color& color) { _content->set_contour_color(color); }
void rectangle_and_contour::set_line_width(float w) { _content->set_line_width(w); }
nu::frect rectangle_and_contour::get_rect() const { return _content->get_rect(); }
nu::color rectangle_and_contour::get_fill_color() const { return _content->get_fill_color(); }
nu::color rectangle_and_contour::get_contour_color() const { return _content->get_contour_color(); }
float rectangle_and_contour::get_line_width() const { return _content->get_line_width(); }
} // namespace nu::geometry.
