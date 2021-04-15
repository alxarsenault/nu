#include "nu/component_manager.h"
#include "nu/component.h"
#include "nu/view.h"

namespace nu {
void component_manager::set_view(nu::view* v) {
  _view = v;
  assign_manager();
}

void component_manager::assign_manager() {
  if (nu::component* root = get_root()) {
    detail::component_internal_ops(root).set_manager(this);
  }
}

void component_manager::assign_manager(nu::component* c) { detail::component_internal_ops(c).set_manager(this); }

void component_manager::release(nu::component* c) {
  if (c == nullptr) {
    fst_assert(false, "This should never happen.");
    return;
  }

  if (_selected_component == c) {
    _selected_component = nullptr;
  }

  detail::component_internal_ops(c).set_manager(nullptr);
}

void component_manager::set_selected_component(nu::component* c, const nu::mouse_event& evt) {
  if (_selected_component == c) {
    return;
  }

  if (_selected_component) {
    detail::component_internal_ops(_selected_component).handle_mouse_leave();
    _selected_component = nullptr;
  }

  if (c == nullptr) {
    return;
  }

  _selected_component = c;
  detail::component_internal_ops(_selected_component).handle_mouse_enter(evt);
}

found_component_info component_manager::find_under_position(const nu::fpoint& p) {
  if (nu::component* root = get_root()) {
    return detail::component_internal_ops(root).find_under_position(p);
  }

  return { nullptr, { 0, 0 } };
}

nu::component* component_manager::get_root() { return _view == nullptr ? nullptr : _view->get_root(); }
const nu::component* component_manager::get_root() const { return _view == nullptr ? nullptr : _view->get_root(); }

void component_manager::set_dirty_rect(const nu::frect& rect) {
  fst::print("component_manager::set_dirty_rect");
  _view->set_dirty_rect(rect);
}

void component_manager::handle_paint(nu::context& g) {
  if (nu::component* root = get_root()) {
    detail::component_internal_ops(root).handle_paint(g);
  }
}

namespace detail {

  //---------------------------------------------------------
  component_internal_ops::component_internal_ops(component& c)
      : _component(&c) {}

  component_internal_ops::component_internal_ops(component* c)
      : _component(c) {}

  void component_internal_ops::set_manager(component_manager* manager) {

    component_internal_ops(_component).iterate([manager](nu::component& c) { c._manager = manager; });
  }

  found_component_info component_internal_ops::find_under_position(const nu::fpoint& p) {
    if (!_component->get_bounds().contains(p)) {
      return { nullptr, nu::fpoint(0, 0) };
    }

    nu::fpoint c_pos = p - _component->get_position();

    // Look for child.
    for (auto& c : _component->_children) {
      if (found_component_info c_info = component_internal_ops(c).find_under_position(c_pos)) {
        return c_info;
      }
    }

    return { _component, c_pos };
  }

  void component_internal_ops::handle_mouse_down(const nu::mouse_event& evt) {
    if (_component->get_override_info(nu::component::override_info::mouse_down)) {
      _component->mouse_down(evt);
    }
  }

  void component_internal_ops::handle_mouse_up(const nu::mouse_event& evt) {
    if (_component->get_override_info(nu::component::override_info::mouse_up)) {
      _component->mouse_up(evt);
    }
  }

  void component_internal_ops::handle_mouse_move(const nu::mouse_event& evt) {
    if (_component->get_override_info(nu::component::override_info::mouse_move)) {
      _component->mouse_move(evt);
    }
  }

  void component_internal_ops::handle_mouse_drag(const nu::mouse_event& evt) {
    if (_component->get_override_info(nu::component::override_info::mouse_drag)) {
      _component->mouse_drag(evt);
    }
  }

  void component_internal_ops::handle_mouse_wheel(const nu::mouse_event& evt) {}

  void component_internal_ops::handle_mouse_enter(const nu::mouse_event& evt) {
    if (_component->get_override_info(nu::component::override_info::mouse_enter)) {
      _component->mouse_enter(evt);
    }
  }

  void component_internal_ops::handle_mouse_leave() {
    if (_component->get_override_info(nu::component::override_info::mouse_leave)) {
      _component->mouse_leave();
    }
  }

  void component_internal_ops::handle_paint(nu::context& g) {
    g.save_state();

    g.translate(_component->get_position());

    if (_component->is_clipped()) {
      g.clip_to_rect(_component->get_local_bounds());
    }

    if (_component->_needs_update) {
      _component->_needs_update = false;
      if (_component->_override_info[nu::component::override_info::update]) {
        _component->update(_component->_geometries);
      }
    }

    if (_component->_override_info[nu::component::override_info::paint]) {
      _component->paint(g);
    }

    _component->_geometries.draw(g);

    for (auto& c : _component->_children) {
      component_internal_ops(c).handle_paint(g);
    }

    g.restore_state();
  }
} // namespace detail.
} // namespace nu.
