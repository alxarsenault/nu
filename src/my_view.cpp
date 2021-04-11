#include "my_view.h"

my_view::my_view() {
  //  add(_c0);
  //  _c0.set_bounds({0, 10, 500, 10});

  add(_c0, { 0, 10, 500, 10 });
  add(_c00, { 10, 50, 5, 5 });

  add(_c1, { 100, 100, 50, 50 });
}

void my_view::paint(nu::context& g) {
  nu::path p;
  p.add_rect(get_local_bounds());
  g.set_color(0x00FFFFFF);
  g.fill_path(p);
}
