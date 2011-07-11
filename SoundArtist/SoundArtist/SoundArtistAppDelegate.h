//
//  SoundArtistAppDelegate.h
//  SoundArtist
//
//  Created by Brandon Aubie on 11-07-10.
//  Copyright 2011 McMaster University. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MainWindow.h"

@interface SoundArtistAppDelegate : NSObject <NSApplicationDelegate> {
@private
    NSWindow *window;
    MainWindow *windowController;
}

- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename;

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet MainWindow *windowController;

@end
