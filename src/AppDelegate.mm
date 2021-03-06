//
//  AppDelegate.m
//  fst_ui
//
//  Created by Alexandre Arsenault on 2021-04-09.
//

#import "AppDelegate.h"
#include "my_view.h"

@implementation AppDelegate

- (id)initWithRect:(NSRect)contentSize {
  if ( self = [super init] ) {
    _ns_window = [[NSWindow alloc] initWithContentRect:contentSize
			styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable backing:NSBackingStoreBuffered defer:YES];

		[_ns_window setTitle:@"test string"];
  
    main_component* m_view = new main_component();
    m_view->set_bounds(nu::rect(0, 0, contentSize.size.width, contentSize.size.height));
    _view = std::make_unique<nu::view>(m_view, true);

    
//    
//    main_component* m_view2 = new main_component(0x00FF00FF);
//    m_view2->set_bounds(nu::rect(70, 70, 100, 100));
//    nu::view* view2 = new nu::view(m_view2, true);
//    [(NSView*)_view->get_native_view() addSubview:(NSView*)view2->get_native_view()];
//
////    [(NSView*)view2->get_native_view() setFrameOrigin:CGPointMake(70, 70)];
//    [(NSView*)view2->get_native_view() setBoundsOrigin:CGPointMake(70, 70)];
  
//    _view = std::make_unique<nu::view>(nu::rect(0, 0, contentSize.size.width, contentSize.size.height));
  
//    _view = [[View alloc] initWithFrame:contentSize];
  }
  
  return self;
}

//-(void)init {
//  _ns_window = [[NSWindow alloc] initWithContentRect:contentSize
//			styleMask:NSWindowStyleMaskTitled backing:NSBackingStoreBuffered defer:YES];
//		
//		[_ns_window setTitle:@"test string"];
//}

- (void)applicationWillFinishLaunching:(NSNotification *)notification {
//	std::cout << "applicationWillFinishLaunching\n";
	// Attach the view to the window
	[_ns_window setContentView:(NSView*)_view->get_native_view()];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  // Insert code here to initialize your application
NSLog(@"The code runs through here!");
  // Make the window visible.
	[_ns_window makeKeyAndOrderFront:self];
	[_ns_window center];
//	[_ns_window orderFrontRegardless];
 
 
// NSWindow* w = [[NSWindow alloc] initWithContentRect:CGRectMake(0, 0, 200, 200)
//			styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable backing:NSBackingStoreBuffered defer:YES];
//
//		[w setTitle:@"test dsdsad"];
//  
//    View* view = [[View alloc] initWithFrame:CGRectMake(0, 0, 200, 200)];
//    [w setContentView:view];
////    [w center];
//    [w orderFrontRegardless];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
  // Insert code here to tear down your application
}

// Close application when clicking on the top left red "x" button.
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication
{
	return TRUE;
}


//- (id)initWithRect:(NSRect)contentSize {
//
//	if ( self = [super initWithFrame:contentSize] ) {
////		_core = core;
////		_cpp_view = cpp_view;
//		
////		[self installRunLoopObserver];
//		
////		std::cout << "Bang\n";
//		
//		// allocate window
//		unsigned int frame_style_flags = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
//			NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;
//		
//		_ns_window = [[NSWindow alloc] initWithContentRect:contentSize
//			styleMask:frame_style_flags backing:NSBackingStoreBuffered defer:YES];
//		
//		[_ns_window setTitle:@"test string"];
//		
////		[self wantsBestResolutionOpenGLSurface];
//		
//		// allocate view
//		//		view = [[NSView alloc] initWithFrame:contentSize];
//	}
//	return self;
//}

@end
