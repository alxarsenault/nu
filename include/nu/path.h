///
/// BSD 3-Clause License
///
/// Copyright (c) 2020, Alexandre Arsenault
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// * Redistributions of source code must retain the above copyright notice, this
///   list of conditions and the following disclaimer.
///
/// * Redistributions in binary form must reproduce the above copyright notice,
///   this list of conditions and the following disclaimer in the documentation
///   and/or other materials provided with the distribution.
///
/// * Neither the name of the copyright holder nor the names of its
///   contributors may be used to endorse or promote products derived from
///   this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
///

#pragma once

#include "fst/assert.h"
#include "fst/range.h"
#include "fst/math.h"

#include "nu/color.h"
#include "nu/point.h"
#include "nu/size.h"
#include "nu/rect.h"

#include <span>

#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>
namespace nu {
using native_path = CGMutablePathRef;
using const_native_path = CGPathRef;
using native_path_value_type = CGFloat;
} // namespace nu.
#endif // __APPLE__

namespace nu {
class path {
public:
  using value_type = native_path_value_type;
  using point = nu::detail::point<value_type>;
  using size = nu::detail::size<value_type>;
  using rect = nu::detail::rect<value_type>;
  using range = fst::range<value_type>;

   path();
  
   path(const path& p);
  
   path(path&& p);
  
   ~path();
  
   path& operator=(const path& p);
  
   path& operator=(path&& p);
  
   bool operator==(const path& p) const;

   bool operator!=(const path& p) const;

   inline explicit operator bool() const {
   return !is_empty();
   }
  
   const_native_path get_native_path() const {
   return _path;
   }

   bool is_empty() const;

   point get_current_point() const;

  /// The bounding box is the smallest rectangle completely enclosing all points in the path,
  /// including control points for Bézier and quadratic curves.
   rect get_bounding_box() const;

  /// The path bounding box is the smallest rectangle completely enclosing all points in the path but
  /// not including control points for Bézier and quadratic curves.
   rect get_path_bounding_box() const;
  
   bool contains(const point& p) const;

   void start_subpath(const point& p);
   void close_subpath();
   void line_to(const point& p);
   void add_line(const point& p0, const point& p1);

  /// This is a convenience function that adds a sequence of connected line segments to a path.
  /// This function is equivalent to calling the 'start_subpath' function to move to the starting
  /// point of the first line segment, then repeatedly calling the 'line_to'
  /// to append the sequence of line segments.
   void add_lines(const std::span<const point>& pts);

  /// Appends a cubic Bézier curve from the current point in a path to the specified location using two control
  /// points, after an optional transformation. Before returning, this function updates the current point to the
  /// specified location (x,y).
   void add_curve_to_point(const point& ctrl_0, const point& ctrl_1, const point& end_point);
  
  /// Appends a quadratic Bézier curve.
   void add_curve_to_point(const point& ctrl, const point& end_point);
   void add_rect(const rect& r);
   void add_rects(const std::span<const rect>& rects);
   void add_rounded_rect(const rect& r, value_type corner_size) ;
   void add_rounded_rect(const rect& r, const size& corner_size);

  /// @param angle_range in radians.
   void add_arc(const point& center, value_type radius, const range& angle_range, bool clockwise = true);

   void add_circle(const point& center, value_type radius);

   void add_ellipse(const rect& r);
  
   void add_triangle(const point& p0, const point& p1, const point& p2);
  
   void add_path(const path& p);

private:
  native_path _path = nullptr;
};


} // namespace nu.
