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


- (void)mouseMoved:(NSEvent*)evt;
- (void)mouseDragged:(NSEvent*)evt;
- (void)mouseUp:(NSEvent*)evt;
- (void)rightMouseDown:(NSEvent*)evt;
- (void)mouseDown:(NSEvent*)evt;
- (void)scrollWheel:(NSEvent*)evt;

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

//  NSPoint locationInView = [self convertPoint:[evt locationInWindow] fromView:nil];
//
//  const nu::fpoint pos(locationInView.x, locationInView.y);
//
//  using internal = nu::detail::component_internal_ops;
//
////  nu::component& root = _manager.get_root();
//  internal::found_component_info c_info = internal(_manager.get_root()).find_under_position(pos);
//
//  if(c_info) {
//    _manager.set_selected_component(c_info.component, nu::mouse_event(c_info.relative_position));
//  }
//  else {
//    _manager.set_selected_component(nullptr, nu::mouse_event{nu::fpoint{0, 0}});
//  }
//
////    fst::print("mouse moved", pos);
}

- (void)mouseDown:(NSEvent *)event {
//  	NSPoint locationInView = [self convertPoint:[event locationInWindow]
//  fromView:nil];
//
//  const nu::fpoint pos(locationInView.x, locationInView.y);
//
//  using internal = nu::detail::component_internal_ops;
//
//  internal::found_component_info c_info = internal(_manager.get_root()).find_under_position(pos);
//
//  if(c_info) {
//    fst::print("FOUNDC OMPONENT", (std::size_t)c_info.component->get_id(), c_info.relative_position);
//    internal(c_info.component).handle_mouse_down(nu::mouse_event(c_info.relative_position));
//  }
////  get_component_at_position();
//  //
//  //	// Double click.
//  //	if (event.clickCount == 2) {
//  //		_core->GetPopupManager()->OnMouseLeftDoubleClick(pos);
//  //		if (_core->GetPopupManager()->IsEventReachWindow() == false) {
//  //			_core->GetWindowManager()->OnMouseLeftDoubleClick(pos);
//  //		}
//  //	}
//  //
//  //	// Simple click.
//  //	else {
//  //		_core->GetPopupManager()->OnMouseLeftDown(pos);
//  //
//  //		if (_core->GetPopupManager()->IsEventReachWindow() == false) {
//  //			_core->GetWindowManager()->OnMouseLeftDown(pos);
//  //		}
//  //	}
}

- (void)rightMouseDown:(NSEvent *)event {
  
}

// Working.
- (void)mouseUp:(NSEvent *)anEvent {

}

- (void)mouseDragged:(NSEvent *)theEvent {
}

// Mouse scroll wheel movement.
- (void)scrollWheel:(NSEvent *)event {

}

- (void)drawRect:(NSRect)rect {
  nu::context g([NSGraphicsContext currentContext].CGContext);
//  
//  fst::print("DRAW RECT", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
//  _manager->handle_paint(g);
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
  _manager.get_root().set_bounds(bounds);
  _pimpl = new pimpl(&_manager, bounds);
}

view::view(fst::optional_owned_ptr<nu::component>&& root)
: _root(std::move(root))
{
  _pimpl = new pimpl(&_manager, _root->get_bounds());
}

view::view(nu::component* root, bool owned)
: _root(root, owned)
{
  _pimpl = new pimpl(&_manager, _root->get_bounds());
}

view::view(nu::component& root)
: _root(&root, false)
{
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
} // namespace nu.
