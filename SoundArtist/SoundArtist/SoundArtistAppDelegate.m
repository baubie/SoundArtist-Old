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
    
    // Setup temporary folders
    NSString *tempDirectoryTemplate =
    [NSTemporaryDirectory() stringByAppendingPathComponent:@"soundartist.XXXXXX"];
    const char *tempDirectoryTemplateCString =
    [tempDirectoryTemplate fileSystemRepresentation];
    char *tempDirectoryNameCString =
    (char *)malloc(strlen(tempDirectoryTemplateCString) + 1);
    strcpy(tempDirectoryNameCString, tempDirectoryTemplateCString);
    
    char *result = mkdtemp(tempDirectoryNameCString);
    if (!result)
    {
        // handle directory creation failure
    }
    NSString *tempDirectoryPath =
    [[NSFileManager defaultManager]
     stringWithFileSystemRepresentation:tempDirectoryNameCString
     length:strlen(result)];
    free(tempDirectoryNameCString);
    
    [windowController setTempDirectory:tempDirectoryPath];

}

- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename {

    return [windowController openSoundFile:filename];
}


@end
