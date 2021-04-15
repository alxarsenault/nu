#include "nu/view.h"
#include "fst/print.h"

#import <Cocoa/Cocoa.h>

@interface CocoaView : NSView {
  nu::component_manager* _manager;
//  @public nu::component_manager _manager;
//  nu::component _root;
}

- (id)initWithFrameAndManager:(nu::component_manager*) manager frame:(NSRect)frame;
- (id)initWithCoder:(NSCoder *)coder;
- (void)viewDidMoveToWindow;
- (BOOL)isFlipped;

- (BOOL)isOpaque;
- (BOOL)wantsDefaultClipping;

- (void)mouseMoved:(NSEvent*)evt;
- (void)mouseDragged:(NSEvent*)evt;
- (void)mouseUp:(NSEvent*)evt;
- (void)rightMouseDown:(NSEvent*)evt;
- (void)mouseDown:(NSEvent*)evt;
- (void)scrollWheel:(NSEvent*)evt;

- (void)display;
- (void)displayIfNeeded;
- (void)displayRect:(NSRect)rect;
- (void)displayIfNeededInRect:(NSRect)rect;
- (void)drawRect:(NSRect)rect;

- (void) OnWindowResize;
@end

@implementation CocoaView

//styleMask:(NSWindowStyleMask)style
- (id)initWithFrameAndManager:(nu::component_manager*) manager frame:(NSRect)frame {

  self = [super initWithFrame:frame];
//  std::cout << "FDLFJDKLFJD " << std::endl;
  if (self) {
    _manager = manager;
    fst::print("FDLKJFDKLJFKLDJFD", _manager->get_root() == nullptr);
    
//    [self setOpaque:TRUE];
//    nu::component& root = _manager.get_root();
//    root.set_bounds(nu::rect(0, 0, frame.size.width, frame.size.height));
//    nu::detail::component_internal_ops(root).set_manager(&_manager);
//
////    _root.set_bounds(nu::rect(0, 0, frame.size.width, frame.size.height));
////    nu::detail::component_internal_ops(&_root).set_manager(&_manager);
////
//    _my_view = std::make_shared<my_view>();
//    _my_view->set_bounds(nu::rect(0, 0, frame.size.width, frame.size.height));
//    root.add(_my_view.get());
  }

  return self;
}

- (id)initWithCoder:(NSCoder *)coder {
  self = [super initWithCoder:coder];
  return self;
}

- (void)viewDidMoveToWindow {
  //  [self addTrackingRect:[self bounds] owner:self userData:NULL
  //  assumeInside:YES];
  [[self window] makeFirstResponder:self];
  [[self window] setAcceptsMouseMovedEvents:YES];

//  [self setWantsLayer:YES];
  //  _view_manager.get_root().emplace_delegate<root_delegate>();
  //  NSRect rect = [self bounds];
  //  _view_manager.get_root().set_position(0, 0);
  //  _view_manager.get_root().set_size(rect.size.width, rect.size.height);
  //  std::cout << "viewDidMoveToWindow " << [self bounds].size.width <<
  //  std::endl;

  //  [[NSNotificationCenter defaultCenter] addObserver:[self window]
  //  selector:@selector(OnWindowResize) name:NSWindowDidResizeNotification
  //  object:nil];

  SEL onWindowResizeSelector = @selector(OnWindowResize);
  [[NSNotificationCenter defaultCenter]
      addObserver:self
         selector:onWindowResizeSelector
             name:NSWindowDidResizeNotification
           object:[self window]];

  //  [[NSNotificationCenter defaultCenter] addObserver:self
  //  selector:@selector(windowDidResize:) name:NSWindowDidResizeNotification
  //  object:theWindow];

  //  [object performSelectorOnMainThread:@selector(methodName)]
  //  _view_manager.get_root().
}
- (void)dealloc {
  // call super
  
  fst::print("VIEW DEALLOC");
      [super dealloc];
}

- (BOOL)isFlipped {
  return YES;
}

- (BOOL)isOpaque {
  return YES;
}

- (BOOL)wantsDefaultClipping {
  return NO;
}

// CGAffineTransform

// struct CGAffineTransform {
//  CGFloat a, b, c, d;
//  CGFloat tx, ty;
//};

- (void)OnWindowResize {
    std::cout << "OnWindowResize" << std::endl;
    NSRect rect = [self bounds];
    fst::print("recty", rect.size.width, rect.size.height);
  //  _view_manager.get_root().set_size(rect.size.width, rect.size.height);
}

//-----------------------------------------------------------------------------
// Mouse events.
//-----------------------------------------------------------------------------
- (void)mouseMoved:(NSEvent *)evt {
  NSPoint locationInView = [self convertPoint:[evt locationInWindow] fromView:nil];

  const nu::fpoint pos(locationInView.x, locationInView.y);
  nu::found_component_info c_info = _manager->find_under_position(pos);

//  using internal = nu::detail::component_internal_ops;

  if(c_info) {
    _manager->set_selected_component(c_info.component, nu::mouse_event(c_info.relative_position));
  }
  else {
    _manager->set_selected_component(nullptr, nu::mouse_event{nu::fpoint{0, 0}});
  }
}

- (void)mouseDown:(NSEvent *)event {
  NSPoint locationInView = [self convertPoint:[event locationInWindow] fromView:nil];

  const nu::fpoint pos(locationInView.x, locationInView.y);
  nu::found_component_info c_info = _manager->find_under_position(pos);

  if(c_info) {
    using internal = nu::detail::component_internal_ops;
    internal(c_info.component).handle_mouse_down(nu::mouse_event(c_info.relative_position));
  }

//  // Double click.
//  if (event.clickCount == 2) {
//  }
//  else {
//
//  }
}

- (void)mouseUp:(NSEvent *)event {
  NSPoint locationInView = [self convertPoint:[event locationInWindow] fromView:nil];
  const nu::fpoint pos(locationInView.x, locationInView.y);

  nu::component* s_component = _manager->get_selected_component();

  if(s_component) {
    // Call mouse up on selected component.
    nu::fpoint abs_pos = s_component->get_absolute_position();
    using internal = nu::detail::component_internal_ops;
    internal(s_component).handle_mouse_up(nu::mouse_event(pos - abs_pos));

    // Assign new selected component.
    nu::found_component_info c_info = _manager->find_under_position(pos);
    if(c_info) {
      _manager->set_selected_component(c_info.component, nu::mouse_event(c_info.relative_position));
    }
    else {
      _manager->set_selected_component(nullptr, nu::mouse_event{nu::fpoint{0, 0}});
    }
    return;
  }

  // If no component is selected.
  nu::found_component_info c_info = _manager->find_under_position(pos);
  if(c_info) {
    using internal = nu::detail::component_internal_ops;
    internal(c_info.component).handle_mouse_up(nu::mouse_event(c_info.relative_position));
  }
}

- (void)rightMouseDown:(NSEvent *)event {
  
}



- (void)mouseDragged:(NSEvent *)evt {

  nu::component* s_component = _manager->get_selected_component();
  if(s_component == nullptr) {
    return;
  }

  NSPoint locationInView = [self convertPoint:[evt locationInWindow] fromView:nil];
  const nu::fpoint pos(locationInView.x, locationInView.y);
  
  nu::fpoint abs_pos = s_component->get_absolute_position();
  
  using internal = nu::detail::component_internal_ops;
  internal(s_component).handle_mouse_drag(nu::mouse_event(pos - abs_pos));
}

// Mouse scroll wheel movement.
- (void)scrollWheel:(NSEvent *)event {

}

- (void)display {
  fst::print("display");
}

- (void)displayIfNeeded {
  fst::print("displayIfNeeded");
}

- (void)displayIfNeededInRect:(NSRect)rect {
  fst::print("displayIfNeededInRect");
}

- (void)displayRect:(NSRect)rect {
  fst::print("displayRect");
}

- (void)drawRect:(NSRect)rect {
  nu::context g([NSGraphicsContext currentContext].CGContext);
//
  nu::frect c_rect = g.get_clipping_rect();
  
  
  bool nnn = [self needsToDrawRect:NSMakeRect(100, 100, 20, 20)];
//  bool needs_display = [self needsDisplay];
  
  const NSRect *rects;
  NSInteger count;
    
    [self getRectsBeingDrawn:&rects count:&count];
    
    nu::frect rr(rects[0].origin.x, rects[0].origin.y, rects[0].size.width, rects[0].size.height);
    fst::print("RRR", nnn, count, rr, c_rect);
//  const NSRect ** rects;
//  NSInteger* count;
//  [self getRectsBeingDrawn:rects count:count];
  fst::print("DRAW RECT", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
  _manager->handle_paint(g);
//
//  //  [[NSColor colorWithCalibratedRed:1.0f green:0 blue:0 alpha:1.0f] setFill];
//  //  [NSBezierPath fillRect:NSMakeRect(0, 0, rect.size.width,
//  //  rect.size.height)];
}

@end


namespace nu {
class view::pimpl {
public:
  pimpl(nu::component_manager* manager, const nu::rect& bounds)
  : _manager(manager)
  {
    CGRect content_size = CGRectMake(bounds.x, bounds.y, bounds.width, bounds.height);
    _view = [[CocoaView alloc] initWithFrameAndManager:manager frame:content_size];
    
  }

  void set_dirty_rect(const nu::frect& rect) {
    [_view setNeedsDisplay:TRUE];
//    [_view setNeedsDisplayInRect:CGRectMake(rect.x, rect.y, rect.width, rect.height)];
  }

  CocoaView* get_view() {
    return _view;
  }

  const CocoaView* get_view() const {
    return _view;
  }

private:
  CocoaView* _view = nullptr;
  nu::component_manager* _manager = nullptr;
};

view::view(const nu::rect& bounds) {
//  _manager.get_root().set_bounds(bounds);
  _manager.set_view(this);
  _pimpl = new pimpl(&_manager, bounds);
}

view::view(fst::optional_owned_ptr<nu::component>&& root)
: _root(std::move(root))
{
  _manager.set_view(this);
  _pimpl = new pimpl(&_manager, _root->get_bounds());
  
}

view::view(nu::component* root, bool owned)
: _root(root, owned)
{
  _manager.set_view(this);
  _pimpl = new pimpl(&_manager, _root->get_bounds());
}

view::view(nu::component& root)
: _root(&root, false)
{
  _manager.set_view(this);
  _pimpl = new pimpl(&_manager, _root->get_bounds());
}

view::~view() {
  if(_pimpl) {
    delete _pimpl;
  }
}

native_view view::get_native_view() {
  return _pimpl->get_view();
}

const native_view view::get_native_view() const {
  return _pimpl->get_view();
}

void view::set_dirty_rect(const nu::frect& rect) {
  _pimpl->set_dirty_rect(rect);
}
} // namespace nu.
