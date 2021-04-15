#include "nu/context.h"

namespace nu {
#ifdef __APPLE__
inline void flip(native_context c, float flip_height) {
  CGContextConcatCTM(c, CGAffineTransformMake(1.0f, 0.0f, 0.0f, -1.0f, 0.0f, flip_height));
}

context::context(native_context c)
    : _context(c) {
  init();
}

context::context(native_context c, reset_transform_tag)
    : _context(c) {
  CGContextSaveGState(_context);
  CGAffineTransform t = CGContextGetCTM(_context);
  t = CGAffineTransformInvert(t);
  CGContextConcatCTM(_context, t);
  CGPoint pp = CGContextConvertPointToUserSpace(_context, CGPoint{ 0, 0 });
  CGContextRestoreGState(_context);
  CGSize ss = CGContextConvertSizeToUserSpace(_context, CGSize{ 0, pp.y });

  int height = std::abs(ss.height);
  init(height);
}

context::context(native_context c, int top_level_height)
    : _context(c) {
  init(top_level_height);
}

context::~context() { CGContextRestoreGState(_context); }

void context::save_state() { CGContextSaveGState(_context); }

void context::restore_state() { CGContextRestoreGState(_context); }

void context::translate(const nu::fpoint& pos) { CGContextTranslateCTM(_context, pos.x, pos.y); }

void context::clip_to_rect(const nu::frect& rect) {
  CGContextClipToRect(_context, CGRectMake(rect.x, rect.y, rect.width, rect.height));
}

nu::frect context::get_clipping_rect() {
  CGRect r = CGContextGetClipBoundingBox(_context);
  return nu::frect(r.origin.x, r.origin.y, r.size.width, r.size.width);
}

void context::set_line_width(value_type width) { CGContextSetLineWidth(_context, width); }

void context::set_color(const nu::color& c) {
  CGContextSetRGBFillColor(_context, c.f_red(), c.f_green(), c.f_blue(), c.f_alpha());
  CGContextSetAlpha(_context, 1);
}

void context::set_stroke_color(const nu::color& c) {
  CGContextSetRGBStrokeColor(_context, c.f_red(), c.f_green(), c.f_blue(), c.f_alpha());
  CGContextSetAlpha(_context, 1);
}

void context::fill_path(const nu::path& p) {
  CGContextAddPath(_context, p.get_native_path());
  CGContextFillPath(_context);
}

void context::draw_path(const nu::path& p) {
  CGContextAddPath(_context, p.get_native_path());
  CGContextStrokePath(_context);
}

void context::draw_image(const nu::image& img, const nu::frect& rect) {
  CGContextTranslateCTM(_context, rect.x, rect.y);
  flip(_context, rect.height);
  CGContextDrawImage(_context, CGRectMake(0, 0, rect.width, rect.height), img.get_native_image());
  flip(_context, rect.height);
  CGContextTranslateCTM(_context, -rect.x, -rect.y);
}

// void context::draw_image(const nu::image& img, const nu::frect& rect) {
////  flip(_context, rect.height);
//  CGContextDrawImage(_context, CGRectMake(rect.x, rect.y, rect.width, rect.height), img.get_native_image());
////  flip(_context, rect.height);
//}

void context::banana(const nu::path& p, const nu::fpoint& pos) {
  CGLayerRef layer = CGLayerCreateWithContext(_context, CGSize{ 100, 100 }, nullptr);
  context cc(CGLayerGetContext(layer));

  cc.set_color(0x00FF00FF);
  cc.fill_path(p);
  CGContextDrawLayerAtPoint(_context, CGPoint{ pos.x, pos.y }, layer);
}

void context::init() { CGContextSaveGState(_context); }

void context::init(int top_level_height) {
  CGContextSaveGState(_context);
  CGContextTranslateCTM(_context, 0.0f, top_level_height);
  CGContextScaleCTM(_context, 1.0f, -1.0f);
}
#endif // __APPLE__
} // namespace nu.
