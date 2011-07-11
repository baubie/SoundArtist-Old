//
//  MainWindow.m
//  SoundArtist
//
//  Created by Brandon Aubie on 11-07-10.
//  Copyright 2011 McMaster University. All rights reserved.
//

#import "MainWindow.h"


@implementation MainWindow

@synthesize commonPropertiesPanel;
@synthesize waveformPropertiesPanel;
@synthesize spectrogramPropertiesPanel;
@synthesize commonPropertiesMenuItem;
@synthesize waveformPropertiesMenuItem;
@synthesize spectrogramPropertiesMenuItem;

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        
    }
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)windowDidLoad
{
    [super windowDidLoad];
}

- (IBAction)refreshWaveform: (id) pId {

}


- (IBAction)refreshSpectrogram: (id) pId {
    
}

- (IBAction)showFileInformation: (id) pId {
    
}

- (IBAction)openDocument: (id) pID {

    NSOpenPanel* openDlg = [NSOpenPanel openPanel];    
    [openDlg setCanChooseFiles:YES];
    [openDlg setAllowsMultipleSelection:NO];
    [openDlg setCanChooseDirectories:NO];
    
    if ( [openDlg runModalForDirectory:nil file:nil] == NSOKButton )
    {
        [self openSoundFile:[openDlg filename]];
    }
}

- (BOOL) openSoundFile:(NSString *)filename {
    
    if (sf != nil) {
        [sf release];
    }
    sf = [[SoundFile alloc] init];

    if ([sf openFile:filename])
    {
        // Add to Open Recent menu
        [[NSDocumentController sharedDocumentController] noteNewRecentDocumentURL:[NSURL fileURLWithPath:filename]];
        return true;
    }
    return false;
}

- (IBAction)toggleProperties:(id) pId {

    NSPanel *panel = nil;
    
    if (pId  == commonPropertiesMenuItem)
    {
        panel = commonPropertiesPanel;
    }
    if (pId  == waveformPropertiesMenuItem)
    {
        panel = waveformPropertiesPanel;
    }
    if (pId == spectrogramPropertiesMenuItem)
    {
        panel = spectrogramPropertiesPanel;
    }
    
    if (panel != nil)
    {
        if([panel isVisible]) {
            [pId setState:NSOffState];
            [panel close];
        } else {
            [pId setState:NSOnState];
            [panel makeKeyAndOrderFront:self];
        }  
    }
}

- (void) windowWillClose: (NSNotification *) notification {
    
    if ([notification object] == commonPropertiesPanel)
    {
        [commonPropertiesMenuItem setState:NSOffState];
    } else 
    if ([notification object] == waveformPropertiesPanel)
    {
        [waveformPropertiesMenuItem setState:NSOffState];
    } else 
    if ([notification object] == spectrogramPropertiesPanel)
    {
        [spectrogramPropertiesMenuItem setState:NSOffState];
    }
}


@end
