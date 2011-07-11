//
//  SoundArtistAppDelegate.m
//  SoundArtist
//
//  Created by Brandon Aubie on 11-07-10.
//  Copyright 2011 McMaster University. All rights reserved.
//

#import "SoundArtistAppDelegate.h"

@implementation SoundArtistAppDelegate

@synthesize window;
@synthesize windowController;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    windowController = [window delegate];
}

- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename {

    return [windowController openSoundFile:filename];
}


@end
