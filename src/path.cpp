#include "nu/path.h"

namespace nu {
#ifdef __APPLE__
static inline CGPoint to_cg_point(const path::point& p) { return *(const CGPoint*)(&p); }

static inline CGRect to_cg_rect(const path::rect& r) { return *(const CGRect*)(&r); }

static inline path::point to_fst_point(const CGPoint& p) { return *(const path::point*)(&p); }

static inline path::rect to_fst_rect(const CGRect& r) { return *(const path::rect*)(&r); }

path::path() { _path = CGPathCreateMutable(); }

path::path(const path& p) {
  fst_assert(p._path != nullptr, "Invalid path.");
  _path = CGPathCreateMutableCopy(p._path);
}

path::path(path&& p) {
  _path = p._path;
  p._path = nullptr;
}

path::~path() {
  if (_path) {
    CGPathRelease(_path);
  }
}

path& path::operator=(const path& p) {
  if (_path) {
    CGPathRelease(_path);
    _path = nullptr;
  }

  fst_assert(p._path != nullptr, "Invalid path.");
  _path = CGPathCreateMutableCopy(p._path);
  return *this;
}

path& path::operator=(path&& p) {
  if (_path) {
    CGPathRelease(_path);
    _path = nullptr;
  }

  _path = p._path;
  p._path = nullptr;
  return *this;
}

bool path::operator==(const path& p) const {
  fst_assert(_path != nullptr, "Invalid path.");
  fst_assert(p._path != nullptr, "Invalid path.");
  return CGPathEqualToPath(_path, p._path);
}

bool path::operator!=(const path& p) const { return !operator==(p); }

//   path::operator bool() const {
//    return !is_empty();
//  }

//   const_native_path path::get_native_path() const {
//    return _path;
//  }

//
//
//
bool path::is_empty() const { return CGPathIsEmpty(_path); }

path::point path::get_current_point() const { return to_fst_point(CGPathGetCurrentPoint(_path)); }

path::rect path::get_bounding_box() const {
  fst_assert(_path != nullptr, "Invalid path.");
  return to_fst_rect(CGPathGetBoundingBox(_path));
}

path::rect path::get_path_bounding_box() const {
  fst_assert(_path != nullptr, "Invalid path.");
  return to_fst_rect(CGPathGetPathBoundingBox(_path));
}

bool path::contains(const path::point& p) const {
  fst_assert(_path != nullptr, "Invalid path.");
  return CGPathContainsPoint(_path, nullptr, to_cg_point(p), false);
}

void path::start_subpath(const path::point& p) {
  fst_assert(_path != nullptr, "Invalid path.");
  CGPathMoveToPoint(_path, nullptr, p.x, p.y);
}

void path::close_subpath() {
  fst_assert(_path != nullptr, "Invalid path.");
  CGPathCloseSubpath(_path);
}

void path::line_to(const path::point& p) {
  fst_assert(_path != nullptr, "Invalid path.");
  CGPathAddLineToPoint(_path, nullptr, p.x, p.y);
}

void path::add_line(const path::point& p0, const path::point& p1) {
  fst_assert(_path != nullptr, "Invalid path.");
  CGPathMoveToPoint(_path, nullptr, p0.x, p0.y);
  CGPathAddLineToPoint(_path, nullptr, p1.x, p1.y);
}

// This is a convenience function that adds a sequence of connected line segments to a path.
// This function is equivalent to calling the 'start_subpath' function to move to the starting
// point of the first line segment, then repeatedly calling the 'line_to'
// to append the sequence of line segments.
void path::add_lines(const std::span<const path::point>& pts) {
  fst_assert(_path != nullptr, "Invalid path.");
  CGPathAddLines(_path, nullptr, (const CGPoint*)pts.data(), pts.size());
}

void path::add_curve_to_point(const path::point& ctrl_0, const path::point& ctrl_1, const path::point& end_point) {
  CGPathAddCurveToPoint(_path, nullptr, ctrl_0.x, ctrl_0.y, ctrl_1.x, ctrl_1.y, end_point.x, end_point.y);
}

void path::add_curve_to_point(const path::point& ctrl, const path::point& end_point) {
  CGPathAddQuadCurveToPoint(_path, nullptr, ctrl.x, ctrl.y, end_point.x, end_point.y);
}

void path::add_rect(const path::rect& r) {
  fst_assert(_path != nullptr, "Invalid path.");
  CGPathAddRect(_path, nullptr, to_cg_rect(r));
}

void path::add_rects(const std::span<const path::rect>& rects) {
  fst_assert(_path != nullptr, "Invalid path.");
  CGPathAddRects(_path, nullptr, (const CGRect*)rects.data(), rects.size());
}

void path::add_rounded_rect(const path::rect& r, value_type corner_size) {
  fst_assert(_path != nullptr, "Invalid path.");
  CGPathAddRoundedRect(_path, nullptr, to_cg_rect(r), corner_size, corner_size);
}

void path::add_rounded_rect(const path::rect& r, const path::size& corner_size) {
  fst_assert(_path != nullptr, "Invalid path.");
  CGPathAddRoundedRect(_path, nullptr, to_cg_rect(r), corner_size.width, corner_size.height);
}

/// @param angle_range in radians.
void path::add_arc(const path::point& center, value_type radius, const path::range& angle_range, bool clockwise) {
  CGPathAddArc(_path, nullptr, center.x, center.y, radius, angle_range.min, angle_range.max, clockwise);
}

void path::add_circle(const path::point& center, value_type radius) {
  CGPathAddArc(_path, nullptr, center.x, center.y, radius, 0, fst::math::two_pi<value_type>, true);
}

void path::add_ellipse(const path::rect& r) { CGPathAddEllipseInRect(_path, nullptr, to_cg_rect(r)); }

void path::add_triangle(const path::point& p0, const path::point& p1, const path::point& p2) {
  start_subpath(p0);
  line_to(p1);
  line_to(p2);
  close_subpath();
}

void path::add_path(const path& p) {
  fst_assert(_path != nullptr, "Invalid path.");
  fst_assert(p._path != nullptr, "Invalid path.");
  CGPathAddPath(_path, nullptr, p._path);
}
#endif // __APPLE__.
} // namespace nu.
