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

#include "nu/color.h"
#include "nu/rect.h"
#include "nu/context.h"
#include "fst/pointer.h"
#include "fst/print.h"
#include "tbb/scalable_allocator.h"

#define TBB_PREVIEW_MEMORY_POOL 1
#include "tbb/memory_pool.h"

namespace nu::geometry {
class vector;

using memory_pool = tbb::memory_pool<std::allocator<char>>;

class base {
public:
  virtual ~base() = default;
  virtual bool intersect(const nu::frect& rect) const = 0;
  virtual void draw(nu::context& g) const = 0;
  virtual void destroy(memory_pool& pool) = 0;
};

class filled_rectangle;
class stroked_rectangle;
class filled_and_stroked_rectangle;
class image;
class image_in_rect;
class stroked_path;
class filled_path;
class filled_and_stroked_path;

class vector {
public:
  inline filled_rectangle* add_filled_rectangle(const nu::frect& rect, const nu::color& color) {
    return add<filled_rectangle>(rect, color);
  }

  inline stroked_rectangle* add_stroked_rectangle(const nu::frect& rect, const nu::color& color, float line_width = 1) {
    return add<stroked_rectangle>(rect, color, line_width);
  }

  inline filled_and_stroked_rectangle* add_filled_and_stroked_rectangle(
      const nu::frect& rect, const nu::color& fill_color, const nu::color& contour_color, float line_width = 1) {
    return add<filled_and_stroked_rectangle>(rect, fill_color, contour_color, line_width);
  }

  inline image* add_image(fst::not_null<nu::image*> img, const nu::fpoint& pos) { return add<image>(img, pos); }

  inline image_in_rect* add_image_in_rect(fst::not_null<nu::image*> img, const nu::frect& rect) {
    return add<image_in_rect>(img, rect);
  }

  inline stroked_path* add_stroked_path(nu::path&& p, const nu::color& color, float line_width) {
    return add<stroked_path>(std::move(p), color, line_width);
  }

  inline filled_path* add_filled_path(nu::path&& p, const nu::color& color) {
    return add<filled_path>(std::move(p), color);
  }

  inline filled_and_stroked_path* add_filled_and_stroked_path(
      nu::path&& p, const nu::color& fill_color, const nu::color& contour_color, float line_width = 1) {
    return add<filled_and_stroked_path>(std::move(p), fill_color, contour_color, line_width);
  }

  template <typename T, typename... Args>
  inline T* add(Args&&... args) {
    return static_cast<T*>(
        _geometries.emplace_back(data_type(new (_pool) T(std::forward<Args>(args)...), _pool)).get());
  }

  inline std::size_t size() const { return _geometries.size(); }
  inline bool is_empty() const { return _geometries.empty(); }

  inline void clear() { _geometries.clear(); }
  inline void reset() { _geometries = vector_type(); }

  inline base* operator[](std::size_t index) { return _geometries[index].get(); }
  inline const base* operator[](std::size_t index) const { return _geometries[index].get(); }

  inline void erase(std::size_t index) { _geometries.erase(_geometries.begin() + index); }

  inline void erase(const base* obj) {
    for (std::size_t i = 0; i < _geometries.size(); i++) {
      if (obj == _geometries[i].get()) {
        erase(i);
        return;
      }
    }
  }

  inline void draw(nu::context& g) const {
    for (const auto& n : _geometries) {
      n->draw(g);
    }
  }

  inline void draw(nu::context& g, const nu::frect& dirty_rect) const {
    for (const auto& n : _geometries) {
      if (n->intersect(dirty_rect)) {
        n->draw(g);
      }
    }
  }

private:
  struct deleter {
    deleter(memory_pool& p)
        : pool(&p){};

    deleter(const deleter&) = default;
    deleter(deleter&&) = default;

    deleter& operator=(const deleter&) = default;
    deleter& operator=(deleter&&) = default;

    inline void operator()(base* p) const { p->destroy(*pool); };

    memory_pool* pool;
  };

  using data_type = std::unique_ptr<base, deleter>;
  using allocator_type = tbb::scalable_allocator<data_type>;
  using vector_type = std::vector<data_type, allocator_type>;

  vector_type _geometries;
  memory_pool _pool;
};

//
// Filled rectangle.
//
class filled_rectangle : public base {
public:
  virtual ~filled_rectangle();
  virtual bool intersect(const nu::frect& rect) const override;
  virtual void draw(nu::context& g) const override;
  virtual void destroy(memory_pool& pool) override;

  void set(const nu::frect& rect, const nu::color& color);
  void set_rect(const nu::frect& rect);
  void set_color(const nu::color& color);

  nu::frect get_rect() const;
  nu::color get_color() const;

private:
  class content;
  content* _content;

  friend class vector;
  filled_rectangle(const nu::frect& rect, const nu::color& color);
  static void* operator new(size_t size, memory_pool& pool);
  static void* operator new(size_t) = delete;
};

//
// Stroked rectangle.
//
class stroked_rectangle : public base {
public:
  virtual ~stroked_rectangle();
  virtual bool intersect(const nu::frect& rect) const override;
  virtual void draw(nu::context& g) const override;
  virtual void destroy(memory_pool& pool) override;

  void set_rect(const nu::frect& rect);
  void set_color(const nu::color& color);
  void set_line_width(float w);
  nu::frect get_rect() const;
  nu::color get_color() const;
  float get_line_width() const;

private:
  class content;
  content* _content;

  friend class vector;
  stroked_rectangle(const nu::frect& rect, const nu::color& color, float line_width);
  static void* operator new(size_t size, memory_pool& pool);
  static void* operator new(size_t) = delete;
};

//
// Filled and stroked rectangle.
//
class filled_and_stroked_rectangle : public base {
public:
  virtual ~filled_and_stroked_rectangle();
  virtual bool intersect(const nu::frect& rect) const override;
  virtual void draw(nu::context& g) const override;
  virtual void destroy(memory_pool& pool) override;

  void set_rect(const nu::frect& rect);
  void set_fill_color(const nu::color& color);
  void set_contour_color(const nu::color& color);
  void set_line_width(float w);
  nu::frect get_rect() const;
  nu::color get_fill_color() const;
  nu::color get_contour_color() const;
  float get_line_width() const;

private:
  class content;
  content* _content;

  friend class vector;
  filled_and_stroked_rectangle(
      const nu::frect& rect, const nu::color& fill_color, const nu::color& contour_color, float line_width);
  static void* operator new(size_t size, memory_pool& pool);
  static void* operator new(size_t) = delete;
};

//
// Image.
//
class image : public base {
public:
  virtual ~image();
  virtual bool intersect(const nu::frect& rect) const override;
  virtual void draw(nu::context& g) const override;
  virtual void destroy(memory_pool& pool) override;

private:
  class content;
  content* _content;

  friend class vector;
  image(fst::not_null<nu::image*> img, const nu::fpoint& pos);
  static void* operator new(size_t size, memory_pool& pool);
  static void* operator new(size_t) = delete;
};

//
// Image in rect.
//
class image_in_rect : public base {
public:
  virtual ~image_in_rect();
  virtual bool intersect(const nu::frect& rect) const override;
  virtual void draw(nu::context& g) const override;
  virtual void destroy(memory_pool& pool) override;

  void set_rect(const nu::frect& rect);
  nu::frect get_rect() const;

private:
  class content;
  content* _content;

  friend class vector;
  image_in_rect(fst::not_null<nu::image*> img, const nu::frect& rect);
  static void* operator new(size_t size, memory_pool& pool);
  static void* operator new(size_t) = delete;
};

//
// Stroked path.
//
class stroked_path : public base {
public:
  virtual ~stroked_path();
  virtual bool intersect(const nu::frect& rect) const override;
  virtual void draw(nu::context& g) const override;
  virtual void destroy(memory_pool& pool) override;

  void set(nu::path&& p, const nu::color& color, float line_width);
  void set_path(nu::path&& p);
  void set_color(const nu::color& color);
  void set_line_width(float w);

  const nu::path& get_path() const;
  nu::color get_color() const;
  float get_line_width() const;

private:
  class content;
  content* _content;

  friend class vector;
  stroked_path(nu::path&& p, const nu::color& color, float line_width);
  static void* operator new(size_t size, memory_pool& pool);
  static void* operator new(size_t) = delete;
};

//
// Filled path.
//
class filled_path : public base {
public:
  virtual ~filled_path();
  virtual bool intersect(const nu::frect& rect) const override;
  virtual void draw(nu::context& g) const override;
  virtual void destroy(memory_pool& pool) override;

  void set(nu::path&& p, const nu::color& color);
  void set_path(nu::path&& p);
  void set_color(const nu::color& color);
  const nu::path& get_path() const;
  nu::color get_color() const;

private:
  class content;
  content* _content;

  friend class vector;
  filled_path(nu::path&& p, const nu::color& color);
  static void* operator new(size_t size, memory_pool& pool);
  static void* operator new(size_t) = delete;
};

//
// Filled and stroked path.
//
class filled_and_stroked_path : public base {
public:
  virtual ~filled_and_stroked_path();
  virtual bool intersect(const nu::frect& rect) const override;
  virtual void draw(nu::context& g) const override;
  virtual void destroy(memory_pool& pool) override;

  void set_path(nu::path&& p);
  void set_fill_color(const nu::color& color);
  void set_contour_color(const nu::color& color);
  void set_line_width(float w);
  const nu::path& get_path() const;
  nu::color get_fill_color() const;
  nu::color get_contour_color() const;
  float get_line_width() const;

private:
  class content;
  content* _content;

  friend class vector;
  filled_and_stroked_path(nu::path&& p, const nu::color& fill_color, const nu::color& contour_color, float line_width);
  static void* operator new(size_t size, memory_pool& pool);
  static void* operator new(size_t) = delete;
};
} // namespace nu::geometry.
