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
#include "fst/print.h"
#include "fst/util.h"

#include "nu/context.h"
#include "nu/geometry.h"
#include "nu/mouse_event.h"

#include <limits>
#include <memory>
#include <vector>

#include <bitset>
namespace fst {
template <class Enum, size_t N = size_t(Enum::count)>
class enum_bitset {
public:
  using enum_type = Enum;

  inline bool operator[](enum_type e) const { return _bitset.test(static_cast<std::size_t>(e)); }

  inline void set(enum_type e, bool value) { _bitset.set(static_cast<std::size_t>(e), value); }

  inline void reset() { _bitset.reset(); }

  inline void set_all() {
    _bitset.reset();
    _bitset.flip();
  }

private:
  std::bitset<N> _bitset;
};
} // namespace fst.

namespace nu {
class component_manager;

namespace detail {
  class component_internal_ops;
} // namespace detail.

class component {
public:
  enum class id_type : std::size_t {};
  static constexpr id_type invalid_id = std::numeric_limits<id_type>::max();
  static constexpr std::size_t invalid_index = std::numeric_limits<std::size_t>::max();

  enum class bounds_status { bounds, position, size };

  // clang-format off
  enum class override_info {
    parent_changed,
    children_changed,
    bounds_changed,

    mouse_down,
    mouse_up,
    mouse_move,
    mouse_drag,
    mouse_wheel,
    mouse_enter,
    mouse_leave,

    update,
    paint,
    count
  };
  // clang-format on

  inline component() {
    _id = generate_id();
    _override_info.set_all();
  }

  virtual ~component();

  void add(component* c);

  inline void add(component& c) { add(&c); }

  inline void add(component* c, const nu::rect& bounds) {
    add(c);
    c->set_bounds(bounds);
  }

  inline void add(component& c, const nu::rect& bounds) {
    add(&c);
    c.set_bounds(bounds);
  }

  void remove(const component* c, bool notify_parent_changed = true);

  inline bool has_child(const component* c) const { return get_child_index(c) != invalid_index; }

  inline bool has_child(id_type c_id) const { return get_child_index(c_id) != invalid_index; }

  inline std::size_t get_child_index(const component* c) const {
    for (std::size_t i = 0; i < _children.size(); i++) {
      if (_children[i] == c) {
        return i;
      }
    }

    return invalid_index;
  }

  inline std::size_t get_child_index(id_type c_id) const {
    for (std::size_t i = 0; i < _children.size(); i++) {
      if (_children[i]->get_id() == c_id) {
        return i;
      }
    }

    return invalid_index;
  }

  inline id_type get_id() const { return _id; }

  inline component* get_parent() { return _parent; }

  inline const component* get_parent() const { return _parent; }

  inline component* get_root_parent() {
    component* p = _parent;

    while (p && p->get_parent()) {
      p = p->get_parent();
    }

    return p;
  }

  inline const component* get_root_parent() const {
    const component* p = _parent;

    while (p && p->get_parent()) {
      p = p->get_parent();
    }

    return p;
  }

  inline void set_position(const nu::point& p) {
    if (fst::assign(_bounds.position, p) && _override_info[override_info::bounds_changed]) {
      bounds_changed(bounds_status::position);
    }
  }

  inline void set_size(const nu::size& s) {
    if (fst::assign(_bounds.size, s) && _override_info[override_info::bounds_changed]) {
      bounds_changed(bounds_status::size);
      if (_update_on_resize) {
        _needs_update = true;
      }
    }
  }

  inline void set_bounds(const nu::rect& r) {
    if (!_override_info[override_info::bounds_changed]) {
      _bounds = r;
      return;
    }

    bool p_changed = fst::assign(_bounds.position, r.position);
    bool s_changed = fst::assign(_bounds.size, r.size);

    if (p_changed && s_changed) {
      bounds_changed(bounds_status::bounds);
      if (_update_on_resize) {
        _needs_update = true;
      }
    }
    else if (p_changed) {
      bounds_changed(bounds_status::position);
    }
    else if (s_changed) {
      bounds_changed(bounds_status::size);
      if (_update_on_resize) {
        _needs_update = true;
      }
    }
  }

  inline const nu::point& get_position() const { return _bounds.position; }

  inline const nu::size& get_size() const { return _bounds.size; }

  inline const nu::rect& get_bounds() const { return _bounds; }

  inline nu::rect get_local_bounds() const { return nu::rect({ 0, 0 }, _bounds.size); }

  inline nu::point get_absolute_position() const {
    nu::point abs_pos = _bounds.position;
    const component* p = _parent;

    while (p) {
      abs_pos += p->get_position();
      p = p->get_parent();
    }

    return abs_pos;
  }

  inline nu::rect get_absolute_bounds() const { return nu::rect(get_absolute_position(), _bounds.size); }

  inline void set_clipping(bool clipping) { _is_clipped = clipping; }

  inline bool is_clipped() const { return _is_clipped; }

  inline void set_update_on_resize(bool value) { _update_on_resize = value; }

  inline bool get_update_on_resize() const { return _update_on_resize; }

  virtual void parent_changed() {}
  virtual void children_changed() {}
  virtual void bounds_changed(bounds_status bs) {}

  // Mouse events.
  virtual void mouse_down(const nu::mouse_event& evt) {}
  virtual void mouse_up(const nu::mouse_event& evt) {}
  virtual void mouse_move(const nu::mouse_event& evt) {}
  virtual void mouse_drag(const nu::mouse_event& evt) {}
  virtual void mouse_wheel(const nu::mouse_event& evt) {}

  /// When the mouse button is pressed and held down while being moved in or out of a component,
  /// none of mouse_enter or mouse_leave are called.
  virtual void mouse_enter(const nu::mouse_event& evt) {}
  virtual void mouse_leave() {}

  virtual void update(nu::geometry::vector& geometries) {}
  virtual void paint(nu::context& gc) {}

public:
  template <typename T>
  inline void init() {
#define HAS_OVERRIDE(fct) !std::is_same_v<decltype(&component::fct), decltype(&T::fct)>
    constexpr bool has_parent_changed = HAS_OVERRIDE(parent_changed);
    constexpr bool has_children_changed = HAS_OVERRIDE(children_changed);
    constexpr bool has_bounds_changed = HAS_OVERRIDE(bounds_changed);
    constexpr bool has_mouse_down = HAS_OVERRIDE(mouse_down);
    constexpr bool has_mouse_up = HAS_OVERRIDE(mouse_up);
    constexpr bool has_mouse_move = HAS_OVERRIDE(mouse_move);
    constexpr bool has_mouse_drag = HAS_OVERRIDE(mouse_drag);
    constexpr bool has_mouse_wheel = HAS_OVERRIDE(mouse_wheel);
    constexpr bool has_mouse_enter = HAS_OVERRIDE(mouse_enter);
    constexpr bool has_mouse_leave = HAS_OVERRIDE(mouse_leave);
    constexpr bool has_update = HAS_OVERRIDE(update);
    constexpr bool has_paint = HAS_OVERRIDE(paint);
#undef HAS_OVERRIDE

    _override_info.set(override_info::parent_changed, has_parent_changed);
    _override_info.set(override_info::children_changed, has_children_changed);
    _override_info.set(override_info::bounds_changed, has_bounds_changed);

    // Mouse.
    _override_info.set(override_info::mouse_down, has_mouse_down);
    _override_info.set(override_info::mouse_up, has_mouse_up);
    _override_info.set(override_info::mouse_move, has_mouse_move);
    _override_info.set(override_info::mouse_drag, has_mouse_drag);
    _override_info.set(override_info::mouse_wheel, has_mouse_wheel);
    _override_info.set(override_info::mouse_enter, has_mouse_enter);
    _override_info.set(override_info::mouse_leave, has_mouse_leave);

    //
    _override_info.set(override_info::update, has_update);
    _override_info.set(override_info::paint, has_paint);
  }

  inline bool get_override_info(override_info o_info) const { return _override_info[o_info]; }

  void repaint();

  void repaint(const nu::rect& r);

private:
  friend class detail::component_internal_ops;

  std::vector<component*> _children;
  nu::geometry::vector _geometries;
  fst::enum_bitset<override_info> _override_info;
  component* _parent = nullptr;
  component_manager* _manager = nullptr;
  nu::rect _bounds;
  id_type _id;
  bool _is_clipped = true;
  bool _needs_update = true;
  bool _update_on_resize = true;

  inline static id_type generate_id() {
    static id_type __id = (id_type)0;
    __id = id_type(std::size_t(__id) + 1);
    return __id;
  }
};
} // namespace nu.
