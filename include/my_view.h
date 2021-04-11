#pragma once

#include "nu/component.h"


class my_view_content_0 : public nu::component {
public:
  virtual void bounds_changed(nu::component::bounds_status bs) override {
    fst::print("MOVED", get_position());
  }

  virtual void paint(nu::context& g) override {
    nu::path p;
    p.add_rect(get_local_bounds());
    g.set_color(0x0000FFFF);
    g.fill_path(p);
  }
};

class my_view_content_1 : public nu::component {
public:
  my_view_content_1() {
    add(_c0, {10, 10, 10, 10});
    init<my_view_content_1>();
  }

  virtual void bounds_changed(nu::component::bounds_status bs) override {
    fst::print("my_view_content_1 -- MOVED", get_position());
  }

  virtual void mouse_enter(const nu::mouse_event& evt) override {
    fst::print("MOUSE ENTER EVENT", evt.get_position());
  }
  
  virtual void mouse_leave() override {
    fst::print("MOUSE LEAVE EVENT");
  }

  virtual void mouse_down(const nu::mouse_event& evt) override {
    fst::print("MOUSE DOWN EVENT", evt.get_position());
  }

  virtual void paint(nu::context& g) override {
    nu::path p;
    p.add_rect(get_local_bounds());
    g.set_color(0xFF00FFFF);
    g.fill_path(p);
  }
  
  private:
  my_view_content_0 _c0;
};

class my_view : public nu::component {
public:
  my_view();
  
  
  virtual void mouse_enter(const nu::mouse_event& evt) override {
    fst::print("MY_VIEW -- MOUSE ENTER EVENT", evt.get_position());
  }
  
  virtual void mouse_leave() override {
    fst::print("MY_VIEW -- MOUSE LEAVE EVENT");
  }
  
  virtual void paint(nu::context& g) override;
  
private:
  my_view_content_0 _c0;
  my_view_content_0 _c00;
  my_view_content_1 _c1;
  
  nu::image _img;
};
