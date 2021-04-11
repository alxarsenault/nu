//
//  main.m
//  fst_ui
//
//  Created by Alexandre Arsenault on 2021-04-09.
//
//#import <Cocoa/Cocoa.h>
//#include "application.h"
//
//int main(int argc, const char * argv[]) {
//  return run(argc, argv);
////  @autoreleasepool {
////      // Setup code that might create autoreleased objects goes here.
////  }
////
//////  AppDelegate* app = [[AppDelegate alloc] init];
//////  AppDelegate* app = [[AppDelegate alloc] initWithRect:NSMakeRect(0, 0, 500, 500)];
//////  [[NSApplication sharedApplication] setDelegate:app];
////
////  return NSApplicationMain(argc, argv);
//}

#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"

int main(int argc, const char * argv[]) {
  @autoreleasepool {
      // Setup code that might create autoreleased objects goes here.
  }


  AppDelegate* app = [[AppDelegate alloc] initWithRect:NSMakeRect(0, 0, 500, 500)];
  [[NSApplication sharedApplication] setDelegate:app];

  return NSApplicationMain(argc, argv);
}
