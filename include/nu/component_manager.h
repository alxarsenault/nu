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

#include "nu/component.h"
#include "nu/context.h"
#include "nu/mouse_event.h"

#include <limits>
#include <memory>
#include <vector>

namespace nu {
class component;

class component_manager {
public:
  void release(nu::component* c);

  void set_selected_component(nu::component* c, const nu::mouse_event& evt);

  inline nu::component& get_root() { return _root; }

  inline const nu::component& get_root() const { return _root; }

  void handle_paint(nu::context& g);

private:
  nu::component _root;
  nu::component* _selected_component = nullptr;
};

namespace detail {
  class component_internal_ops {
  public:
    component_internal_ops(component& c);
    component_internal_ops(component* c);

    void set_manager(component_manager* manager);

    struct found_component_info {
      nu::component* component;
      nu::fpoint relative_position;

      inline explicit operator bool() const { return component != nullptr; }
    };

    found_component_info find_under_position(const nu::fpoint& p);

    void handle_mouse_down(const nu::mouse_event& evt);
    void handle_mouse_up(const nu::mouse_event& evt);
    void handle_mouse_move(const nu::mouse_event& evt);
    void handle_mouse_drag(const nu::mouse_event& evt);
    void handle_mouse_wheel(const nu::mouse_event& evt);

    void handle_mouse_enter(const nu::mouse_event& evt);
    void handle_mouse_leave();

    void handle_paint(nu::context& g);

  private:
    component* _component;
  };
} // namespace detail.

} // namespace nu.
