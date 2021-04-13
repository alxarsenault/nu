#import "view.h"
#include <iostream>

#include "fst/print.h"
#include "nu/context.h"
//
//#include "nu/context.h"
//#include "nu/renderer.h"
//#include "nu/fmath.h"
//#include "nu/view.h"

@implementation View

- (id)initWithFrame:(NSRect)frame {

  self = [super initWithFrame:frame];
  std::cout << "FDLFJDKLFJD " << std::endl;
  if (self) {

    nu::component& root = _manager.get_root();
    root.set_bounds(nu::rect(0, 0, frame.size.width, frame.size.height));
    nu::detail::component_internal_ops(root).set_manager(&_manager);
  
//    _root.set_bounds(nu::rect(0, 0, frame.size.width, frame.size.height));
//    nu::detail::component_internal_ops(&_root).set_manager(&_manager);
//
    _my_view = std::make_shared<my_view>();
    _my_view->set_bounds(nu::rect(0, 0, frame.size.width, frame.size.height));
    root.add(_my_view.get());
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

  NSPoint locationInView = [self convertPoint:[evt locationInWindow] fromView:nil];
  
  const nu::fpoint pos(locationInView.x, locationInView.y);

  using internal = nu::detail::component_internal_ops;
  
//  nu::component& root = _manager.get_root();
  internal::found_component_info c_info = internal(_manager.get_root()).find_under_position(pos);
  
  if(c_info) {
    _manager.set_selected_component(c_info.component, nu::mouse_event(c_info.relative_position));
  }
  else {
    _manager.set_selected_component(nullptr, nu::mouse_event{nu::fpoint{0, 0}});
  }

//    fst::print("mouse moved", pos);
}

- (void)mouseDown:(NSEvent *)event {
  	NSPoint locationInView = [self convertPoint:[event locationInWindow]
  fromView:nil];
  
  const nu::fpoint pos(locationInView.x, locationInView.y);
  
  using internal = nu::detail::component_internal_ops;

  internal::found_component_info c_info = internal(_manager.get_root()).find_under_position(pos);
  
  if(c_info) {
    fst::print("FOUNDC OMPONENT", (std::size_t)c_info.component->get_id(), c_info.relative_position);
    internal(c_info.component).handle_mouse_down(nu::mouse_event(c_info.relative_position));
  }
//  get_component_at_position();
  //
  //	// Double click.
  //	if (event.clickCount == 2) {
  //		_core->GetPopupManager()->OnMouseLeftDoubleClick(pos);
  //		if (_core->GetPopupManager()->IsEventReachWindow() == false) {
  //			_core->GetWindowManager()->OnMouseLeftDoubleClick(pos);
  //		}
  //	}
  //
  //	// Simple click.
  //	else {
  //		_core->GetPopupManager()->OnMouseLeftDown(pos);
  //
  //		if (_core->GetPopupManager()->IsEventReachWindow() == false) {
  //			_core->GetWindowManager()->OnMouseLeftDown(pos);
  //		}
  //	}
}

- (void)rightMouseDown:(NSEvent *)event {
  //	NSPoint locationInView = [self convertPoint:[event locationInWindow]
  //fromView:nil]; 	const ax::Point pos(locationInView.x, locationInView.y);
  //
  //	// Double click.
  //	if (event.clickCount == 2) {
  //		// _axApp->GetPopupManager()->OnMouseRightDoubleClick(pos);
  //		//        if(_axApp->GetPopupManager()->IsEventReachWindow() ==
  //false)
  //		//        {
  //		// _axApp->GetWindowManager()->OnMouseRightDoubleClick(pos);
  //		//        }
  //	}
  //
  //	// Simple click.
  //	else {
  //		_core->GetPopupManager()->OnMouseRightDown(pos);
  //
  //		if (_core->GetPopupManager()->IsEventReachWindow() == false) {
  //			_core->GetWindowManager()->OnMouseRightDown(pos);
  //		}
  //	}
}

// Working.
- (void)mouseUp:(NSEvent *)anEvent {
  //	NSPoint locationInView = [self convertPoint:[anEvent locationInWindow]
  //fromView:nil]; 	const ax::Point pos(locationInView.x, locationInView.y);
  //
  //	_core->GetPopupManager()->OnMouseLeftUp(pos);
  //
  //	if (_core->GetPopupManager()->IsEventReachWindow() == false) {
  //		_core->GetWindowManager()->OnMouseLeftUp(pos);
  //	}
}

- (void)mouseDragged:(NSEvent *)theEvent {
  //	NSPoint locationInView = [self convertPoint:[theEvent locationInWindow]
  //fromView:nil]; 	const ax::Point pos(locationInView.x, locationInView.y);
  //
  //	_core->GetPopupManager()->OnMouseLeftDragging(pos);
  //
  //	if (_core->GetPopupManager()->IsEventReachWindow() == false) {
  //		_core->GetWindowManager()->OnMouseLeftDragging(pos);
  //	}
}

// Mouse scroll wheel movement.
- (void)scrollWheel:(NSEvent *)event {
  //	double dx = -[event deltaX];
  //	double dy = -[event deltaY];
  //
  //	if (dy == 0 && dx == 0) {
  //		return;
  //	}
  //
  //	ax::Point delta(dx, dy);
  //	_core->GetPopupManager()->OnScrollWheel(delta);
  //
  //	if (_core->GetPopupManager()->IsEventReachWindow() == false) {
  //		_core->GetWindowManager()->OnScrollWheel(delta);
  //	}
}

- (void)drawRect:(NSRect)rect {
  nu::context g([NSGraphicsContext currentContext].CGContext);
  
  fst::print("DRAW RECT", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
  _manager.handle_paint(g);

  //  [[NSColor colorWithCalibratedRed:1.0f green:0 blue:0 alpha:1.0f] setFill];
  //  [NSBezierPath fillRect:NSMakeRect(0, 0, rect.size.width,
  //  rect.size.height)];
}

@end
