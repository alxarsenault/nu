#pragma once

#import <Cocoa/Cocoa.h>

#include "nu/component.h"
#include "nu/component_manager.h"
#include "my_view.h"

@interface View : NSView {
  nu::component_manager _manager;
  nu::component _root;
  std::shared_ptr<my_view> _my_view;
}

- (id)initWithFrame:(NSRect)frame;
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
