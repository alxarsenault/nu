#include "nu/component.h"
#include "nu/component_manager.h"

namespace nu {
component::~component() {
  if (component* p = get_parent()) {
    p->remove(this);
  }

  if (_manager) {
    _manager->release(this);
  }
}

void component::add(component* c) {
  // Remove from old parent.
  if (component* p = c->get_parent()) {
    p->remove(c, false);
  }

  // Release from old manager.
  if (c->_manager && c->_manager != _manager) {
    c->_manager->release(c);
  }

  _manager->assign_manager(c);
  //  c->_manager = _manager;

  _children.push_back(c);
  _children.back()->_parent = this;

  if (c->_override_info[override_info::parent_changed]) {
    c->parent_changed();
  }

  if (_override_info[override_info::children_changed]) {
    children_changed();
  }
}

void component::remove(const component* c, bool notify_parent_changed) {
  std::size_t index = get_child_index(c);
  if (index == invalid_index) {
    return;
  }

  component* cc = _children[index];
  cc->_parent = nullptr;
  _children.erase(_children.begin() + index);
  if (cc->_manager) {
    cc->_manager->release(this);
    cc->_manager = nullptr;
  }

  if (_override_info[override_info::parent_changed] && notify_parent_changed) {
    cc->parent_changed();
  }

  if (_override_info[override_info::children_changed]) {
    children_changed();
  }
}

void component::repaint() {
  _needs_repaint = true;
  //  _dirty_bounds = get_local_bounds();
  if (_manager) {
    fst::print("JJJJ");
    //      _manager->set_dirty_rect(_dirty_bounds);
    _manager->set_dirty_rect({ 0, 0, 50, 50 });
    //    _manager->set_dirty_rect({200, 0, 20, 20});
    //    _manager->set_dirty_rect({50, 50, 10, 10});
  }
}

void component::repaint(const nu::rect& r) {
  _needs_repaint = true;
  //  _dirty_bounds = r;
  //  _dirty_bounds.clip_to(get_local_bounds());

  if (_manager) {
    //    _manager->set_dirty_rect(_dirty_bounds);
    //    _manager->set_dirty_rect({0, 0, 500, 500});
  }
}
} // namespace nu.
