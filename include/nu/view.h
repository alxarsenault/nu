#pragma once

#include "fst/pointer.h"
#include "nu/rect.h"
#include "nu/component_manager.h"

namespace nu {
using native_view = void*;

class view {
public:
  class pimpl;

  view(const nu::rect& bounds);
  view(fst::optional_owned_ptr<nu::component>&& root);
  view(nu::component* root, bool owned);
  view(nu::component& root);

  ~view();

  const nu::rect& get_bounds() const { return get_root().get_bounds(); }

  inline nu::component& get_root() { return _manager.get_root(); }
  inline const nu::component& get_root() const { return _manager.get_root(); }

  native_view get_native_view();
  const native_view get_native_view() const;

private:
  fst::optional_owned_ptr<nu::component> _root;
  nu::component_manager _manager;
  pimpl* _pimpl = nullptr;
};

} // namespace nu.
