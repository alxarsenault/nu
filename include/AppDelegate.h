//
//  AppDelegate.h
//  fst_ui
//
//  Created by Alexandre Arsenault on 2021-04-09.
//

#pragma once
#import <Cocoa/Cocoa.h>
#include "view.h"

//class View;

@interface AppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* _ns_window;
  View* _view;
  
  
}



- (id)initWithRect:(NSRect)contentSize;

@end
