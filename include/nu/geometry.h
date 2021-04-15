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

class rectangle;
class rectangle_contour;
class rectangle_and_contour;

class vector {
public:
  inline rectangle* add_rect(const nu::frect& rect, const nu::color& color) { return add<rectangle>(rect, color); }

  inline rectangle_contour* add_rect_contour(const nu::frect& rect, const nu::color& color, float line_width = 1) {
    return add<rectangle_contour>(rect, color, line_width);
  }

  inline rectangle_and_contour* add_rect_and_contour(
      const nu::frect& rect, const nu::color& fill_color, const nu::color& contour_color, float line_width = 1) {
    return add<rectangle_and_contour>(rect, fill_color, contour_color, line_width);
  }

  template <typename T, typename... Args>
  inline T* add(Args&&... args) {
    return static_cast<T*>(
        _geometries.emplace_back(data_type(new (_pool) T(std::forward<Args>(args)...), _pool)).get());
  }

  inline std::size_t size() const { return _geometries.size(); }

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

class rectangle : public base {
public:
  virtual ~rectangle();
  virtual bool intersect(const nu::frect& rect) const override;
  virtual void draw(nu::context& g) const override;
  virtual void destroy(memory_pool& pool) override;

  void set_rect(const nu::frect& rect);
  void set_color(const nu::color& color);

  nu::frect get_rect() const;
  nu::color get_color() const;

private:
  class content;
  content* _content;

  friend class vector;
  rectangle(const nu::frect& rect, const nu::color& color);
  static void* operator new(size_t size, memory_pool& pool);
  static void* operator new(size_t) = delete;
};

class rectangle_contour : public base {
public:
  virtual ~rectangle_contour();
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
  rectangle_contour(const nu::frect& rect, const nu::color& color, float line_width);
  static void* operator new(size_t size, memory_pool& pool);
  static void* operator new(size_t) = delete;
};

class rectangle_and_contour : public base {
public:
  virtual ~rectangle_and_contour();
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
  rectangle_and_contour(
      const nu::frect& rect, const nu::color& fill_color, const nu::color& contour_color, float line_width);
  static void* operator new(size_t size, memory_pool& pool);
  static void* operator new(size_t) = delete;
};
} // namespace nu::geometry.
