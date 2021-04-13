//#include "nu/window.h"
//#include "view.h"
//
//
////#ifdef __APPLE__
//////@interface Window : NSWindow {
////@interface Window : NSWindow <NSWindowDelegate> {
////}
////
////@end
////#endif // __APPLE__
////
////@implementation Window
////- (void)windowDidResize:(NSNotification *)notification {
////  fst::print("windowDidResize");
////}
////
////- (void)windowDidExpose:(NSNotification *)notification {
////  fst::print("windowDidExpose");
////}
////
////- (void)windowDidBecomeKey:(NSNotification *)notification {
////  fst::print("windowDidBecomeKey");
////}
////
////- (void)windowDidResignKey:(NSNotification *)notification {
////  fst::print("windowDidResignKey");
////}
////
////- (void)windowDidBecomeMain:(NSNotification *)notification {
////  fst::print("windowDidBecomeMain");
////}
////
////- (void)windowDidResignMain:(NSNotification *)notification {
////  fst::print("windowDidResignMain");
////}
////
////
////- (void)windowWillClose:(NSNotification *)notification {
////  fst::print("windowWillClose");
////}
////
//////- (void)windowDidResize:(NSNotification *)notification;
//////- (void)windowDidExpose:(NSNotification *)notification;
//////- (void)windowWillMove:(NSNotification *)notification;
//////- (void)windowDidMove:(NSNotification *)notification;
//////- (void)windowDidBecomeKey:(NSNotification *)notification;
//////- (void)windowDidResignKey:(NSNotification *)notification;
//////- (void)windowDidBecomeMain:(NSNotification *)notification;
//////- (void)windowDidResignMain:(NSNotification *)notification;
//////- (void)windowWillClose:(NSNotification *)notification;
//////- (void)windowWillMiniaturize:(NSNotification *)notification;
//////- (void)windowDidMiniaturize:(NSNotification *)notification;
//////- (void)windowDidDeminiaturize:(NSNotification *)notification;
//////- (void)windowDidUpdate:(NSNotification *)notification;
//////- (void)windowDidChangeScreen:(NSNotification *)notification;
//////- (void)windowDidChangeScreenProfile:(NSNotification *)notification;
//////- (void)windowDidChangeBackingProperties:(NSNotification *)notification API_AVAILABLE(macos(10.7)); // added in 10.7.3
//////- (void)windowWillBeginSheet:(NSNotification *)notification;
//////- (void)windowDidEndSheet:(NSNotification *)notification;
//////- (void)windowWillStartLiveResize:(NSNotification *)notification API_AVAILABLE(macos(10.6));
//////- (void)windowDidEndLiveResize:(NSNotification *)notification API_AVAILABLE(macos(10.6));
//////- (void)windowWillEnterFullScreen:(NSNotification *)notification API_AVAILABLE(macos(10.7));
//////- (void)windowDidEnterFullScreen:(NSNotification *)notification API_AVAILABLE(macos(10.7));
//////- (void)windowWillExitFullScreen:(NSNotification *)notification API_AVAILABLE(macos(10.7));
//////- (void)windowDidExitFullScreen:(NSNotification *)notification API_AVAILABLE(macos(10.7));
//////- (void)windowWillEnterVersionBrowser:(NSNotification *)notification API_AVAILABLE(macos(10.7));
//////- (void)windowDidEnterVersionBrowser:(NSNotification *)notification API_AVAILABLE(macos(10.7));
//////- (void)windowWillExitVersionBrowser:(NSNotification *)notification API_AVAILABLE(macos(10.7));
//////- (void)windowDidExitVersionBrowser:(NSNotification *)notification API_AVAILABLE(macos(10.7));
//////- (void)windowDidChangeOcclusionState:(NSNotification *)notification API_AVAILABLE(macos(10.9));
////@end
////
////namespace nu {
////#ifdef __APPLE__
////
////
////
////class window::pimpl {
////public:
////  pimpl(const nu::rect& bounds, const std::string& title) {
////    CGRect content_size = CGRectMake(bounds.x, bounds.y, bounds.width, bounds.height);
////     _window = [[Window alloc] initWithContentRect:content_size
////			styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable backing:NSBackingStoreBuffered defer:YES];
////
//////    NSString* ns_title = [NSString stringWithUTF8String:title.c_str()];
////    NSString* ns_title = [[NSString alloc] initWithUTF8String:title.c_str()];
////    [_window setTitle:ns_title];
////    [ns_title release];
////
////    [_window setDelegate:_window];
////
////    _view = [[View alloc] initWithFrame:content_size];
////    [_window setContentView:_view];
////  }
////
////  nu::size get_size() const {
////    NSRect rect = [_view bounds];
////    return nu::size(rect.size.width, rect.size.height);
////  }
////
////  inline void show() {
//////    [_window orderFrontRegardless];
////    [_window orderFront:_window];
////  }
////
////
////  View* get_view() {
////    return _view;
////  }
////
////  const View* get_view() const {
////    return _view;
////  }
////
////  nu::component& get_root() {
////  return _view->_manager.get_root();
////}
////
////const nu::component& get_root() const {
////  return _view->_manager.get_root();
////}
////
////private:
////  Window* _window = nullptr;
////  View* _view = nullptr;
////};
////#endif // __APPLE__
//
//
//
//
//
//
//
//#ifdef __APPLE__
////@interface Window : NSWindow {
//@interface Window : NSWindow <NSWindowDelegate> {
//}
//
//@end
//#endif // __APPLE__
//
//@implementation Window
//- (void)windowDidResize:(NSNotification *)notification {
//  fst::print("windowDidResize");
//}
//
//- (void)windowDidExpose:(NSNotification *)notification {
//  fst::print("windowDidExpose");
//}
//
//- (void)windowDidBecomeKey:(NSNotification *)notification {
//  fst::print("windowDidBecomeKey");
//}
//
//- (void)windowDidResignKey:(NSNotification *)notification {
//  fst::print("windowDidResignKey");
//}
//
//- (void)windowDidBecomeMain:(NSNotification *)notification {
//  fst::print("windowDidBecomeMain");
//}
//
//- (void)windowDidResignMain:(NSNotification *)notification {
//  fst::print("windowDidResignMain");
//}
//
//
//- (void)windowWillClose:(NSNotification *)notification {
//  fst::print("windowWillClose");
//}
//
////- (void)windowDidResize:(NSNotification *)notification;
////- (void)windowDidExpose:(NSNotification *)notification;
////- (void)windowWillMove:(NSNotification *)notification;
////- (void)windowDidMove:(NSNotification *)notification;
////- (void)windowDidBecomeKey:(NSNotification *)notification;
////- (void)windowDidResignKey:(NSNotification *)notification;
////- (void)windowDidBecomeMain:(NSNotification *)notification;
////- (void)windowDidResignMain:(NSNotification *)notification;
////- (void)windowWillClose:(NSNotification *)notification;
////- (void)windowWillMiniaturize:(NSNotification *)notification;
////- (void)windowDidMiniaturize:(NSNotification *)notification;
////- (void)windowDidDeminiaturize:(NSNotification *)notification;
////- (void)windowDidUpdate:(NSNotification *)notification;
////- (void)windowDidChangeScreen:(NSNotification *)notification;
////- (void)windowDidChangeScreenProfile:(NSNotification *)notification;
////- (void)windowDidChangeBackingProperties:(NSNotification *)notification API_AVAILABLE(macos(10.7)); // added in 10.7.3
////- (void)windowWillBeginSheet:(NSNotification *)notification;
////- (void)windowDidEndSheet:(NSNotification *)notification;
////- (void)windowWillStartLiveResize:(NSNotification *)notification API_AVAILABLE(macos(10.6));
////- (void)windowDidEndLiveResize:(NSNotification *)notification API_AVAILABLE(macos(10.6));
////- (void)windowWillEnterFullScreen:(NSNotification *)notification API_AVAILABLE(macos(10.7));
////- (void)windowDidEnterFullScreen:(NSNotification *)notification API_AVAILABLE(macos(10.7));
////- (void)windowWillExitFullScreen:(NSNotification *)notification API_AVAILABLE(macos(10.7));
////- (void)windowDidExitFullScreen:(NSNotification *)notification API_AVAILABLE(macos(10.7));
////- (void)windowWillEnterVersionBrowser:(NSNotification *)notification API_AVAILABLE(macos(10.7));
////- (void)windowDidEnterVersionBrowser:(NSNotification *)notification API_AVAILABLE(macos(10.7));
////- (void)windowWillExitVersionBrowser:(NSNotification *)notification API_AVAILABLE(macos(10.7));
////- (void)windowDidExitVersionBrowser:(NSNotification *)notification API_AVAILABLE(macos(10.7));
////- (void)windowDidChangeOcclusionState:(NSNotification *)notification API_AVAILABLE(macos(10.9));
//@end
//
//namespace nu {
//#ifdef __APPLE__
//
//
//
//class window::pimpl {
//public:
//  pimpl(nu::view* v, const nu::rect& bounds, const std::string& title)
//  : _view(v) {
//  
////    nu::rect bounds = _view->get_bounds();
//    CGRect content_size = CGRectMake(bounds.x, bounds.y, bounds.width, bounds.height);
//     _window = [[Window alloc] initWithContentRect:content_size
//			styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable backing:NSBackingStoreBuffered defer:YES];
//   
////    NSString* ns_title = [NSString stringWithUTF8String:title.c_str()];
//    NSString* ns_title = [[NSString alloc] initWithUTF8String:title.c_str()];
//    [_window setTitle:ns_title];
//    [ns_title release];
//    
//    [_window setDelegate:_window];
//    
////    _view = [[View alloc] initWithFrame:content_size];
//    [_window setContentView:get_ns_view()];
//  }
//
//  nu::size get_size() const {
//    NSRect rect = [get_ns_view() bounds];
//    return nu::size(rect.size.width, rect.size.height);
//  }
//  
//  inline void show() {
////    [_window orderFrontRegardless];
//    [_window orderFront:_window];
//  }
//
////
////  View* get_view() {
////    return _view;
////  }
////
////  const View* get_view() const {
////    return _view;
////  }
////
////  nu::component& get_root() {
////  return _view->_manager.get_root();
////}
////
////const nu::component& get_root() const {
////  return _view->_manager.get_root();
////}
//
//private:
//  Window* _window = nullptr;
////  View* _view = nullptr;
//  nu::view* _view = nullptr;
//  
//  inline NSView* get_ns_view() {
//    return (NSView*)_view->get_native_view();
//  }
//  
//  inline const NSView* get_ns_view() const {
//    return (const NSView*)_view->get_native_view();
//  }
//};
//#endif // __APPLE__
//
//
//
////
////
////
////
////
////
////
////
//
//window::window(const nu::size& size, const std::string& title)
//  : _view(nu::rect({0, 0}, size)){
//  _pimpl = new pimpl(&_view, nu::rect({0, 0}, size), title);
//}
//
//window::~window() {
//  if(_pimpl) {
//    delete _pimpl;
//  }
//}
//
//void window::show() {
//  _pimpl->show();
//}
//
//nu::component& window::get_root() {
//  return _view.get_root();
//}
//
//const nu::component& window::get_root() const {
//  return _view.get_root();
//}
// 
//nu::size window::get_size() const {
//  return _pimpl->get_size();
//}
//} // namespace nu.
