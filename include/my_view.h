#pragma once

#include "nu/component.h"
#include "nu/window.h"
#include "nu/geometry.h"

class red_component : public nu::component {
public:
  red_component() {
    _color = 0xFF000055;
    _img = nu::image(NU_RESOURCES_DIRECTORY "/start@2x.png", 2);

    setup();
  }

  virtual void mouse_down(const nu::mouse_event& evt) override {
    fst::print("red_component - mouse_down", evt.get_position());
    _color = 0x00FF0055;
    repaint();
  }
  
  virtual void mouse_up(const nu::mouse_event& evt) override {
    fst::print("red_component - mouse_up", evt.get_position());
    _color = 0xFF000055;
    repaint();
  }

  void setup() {
    auto& g = _geometries;
    _bg_img = g.add_image_in_rect(&_img, get_local_bounds());
    _bg_rect = g.add_filled_rectangle(get_local_bounds(), _color);
    g.add_filled_rectangle(nu::frect(5.0f, 5.0f, 10.0f, 10.0f), 0x0000FFFF);
    g.add_stroked_rectangle(nu::frect(5.0f, 5.0f, 10.0f, 10.0f), 0xFFFFFFFF, 1.0f);
    g.add_filled_and_stroked_rectangle(nu::frect(25.0f, 5.0f, 10.0f, 10.0f), 0x00FF00FF, 0x000000FF, 1.0f);

    namespace geo = nu::geometry;
    using fs_rect = geo::filled_and_stroked_rectangle;
    g.add<fs_rect>(nu::frect(30.0f, 10.0f, 10.0f, 10.0f), 0x00FFFFFF, 0x000000FF, 2.0f);

    nu::path p0;
    p0.add_circle({ 20, 20 }, 5);
    g.add_filled_path(std::move(p0), 0x33FF33FF);

    nu::path p1;
    p1.add_circle({ 20, 20 }, 5);
    g.add_stroked_path(std::move(p1), 0x000000FF, 2.0f);

    nu::path p2;
    p2.add_circle({ 30, 30 }, 5);
    g.add_filled_and_stroked_path(std::move(p2), 0xFFFF00FF, 0x000000FF, 1.0f);
  }

  void update() {
    nu::rect bounds = get_local_bounds();
    _bg_img->set_rect(bounds);
    _bg_rect->set(bounds, _color);
  }

  virtual void paint(nu::context& g) override {
    update();
    _geometries.draw(g);
//    _geometries.draw(g, {0, 0, 10, 10});
  }
  
  private:
  nu::color _color;
  nu::image _img;
  nu::geometry::vector _geometries;
  nu::geometry::image_in_rect* _bg_img = nullptr;
  nu::geometry::filled_rectangle* _bg_rect = nullptr;
};

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
  
  virtual void mouse_down(const nu::mouse_event& evt) override {
    fst::print("MY_VIEW MOUSE DOWN", evt.get_position());
//    nu::window* w = new nu::window(nu::size(200, 200), "Banana");
//    nu::component& root = w->get_root();
//    
//    root.add(new red_component(), {0, 0, 200, 200});
//
//    _window.reset(w);
//    _window->show();
  }
  
  virtual void paint(nu::context& g) override;
  
private:
  my_view_content_0 _c0;
  my_view_content_0 _c00;
  my_view_content_1 _c1;
  
  nu::image _img;
//  std::unique_ptr<nu::window> _window;
};


class main_component : public nu::component {
public:
  main_component() {
    set_size({500, 500});
    
    add(_r0, {10, 10, 50, 50});
  }
  
  main_component(nu::color c)
  : _color(c) {
    set_size({500, 500});
    
    add(_r0, {10, 10, 50, 50});
  }

  virtual void mouse_down(const nu::mouse_event& evt) override {
    fst::print("main_component - mouse_down", evt.get_position());
  }
  
  virtual void paint(nu::context& g) override {
    nu::path p;
    p.add_rect(get_local_bounds());
    g.set_color(_color);
    g.fill_path(p);
  }
  
private:
  red_component _r0;
  nu::color _color = 0x00FFFFFF;
};
