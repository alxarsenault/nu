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

#include "nu/path.h"
#include "nu/image.h"

#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>
namespace nu {
using native_context = CGContextRef;
//using const_native_path = CGPathRef;
using native_context_value_type = CGFloat;
} // namespace nu.
#endif // __APPLE__

namespace nu {
class context {
public:
  using value_type = native_context_value_type;
  struct reset_transform_tag{};

   context(native_context c);
   context(native_context c, reset_transform_tag);

   context(native_context c, int top_level_height);

   ~context();
  
   void save_state();
   void restore_state();
  
   void translate(const nu::fpoint& pos);
  
   void clip_to_rect(const nu::frect& rect);
   nu::frect get_clipping_rect();

   void set_color(const nu::color& c);
   void fill_path(const nu::path& p);
   void draw_image(const nu::image& img, const nu::frect& rect);
   void banana(const nu::path& p, const nu::fpoint& pos);
  
private:
  native_context _context;
  
  void init();
  void init(int top_level_height);
};
} // namespace nu.
