//
//  MainWindow.h
//  SoundArtist
//
//  Created by Brandon Aubie on 11-07-10.
//  Copyright 2011 McMaster University. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SoundFile.h"

@interface MainWindow : NSWindowController <NSWindowDelegate> {
@private
    
    SoundFile *sf;
    
}

@property (nonatomic, retain) IBOutlet NSPanel *commonPropertiesPanel;
@property (nonatomic, retain) IBOutlet NSPanel *waveformPropertiesPanel;
@property (nonatomic, retain) IBOutlet NSPanel *spectrogramPropertiesPanel;

@property (nonatomic, retain) IBOutlet NSMenuItem *commonPropertiesMenuItem;
@property (nonatomic, retain) IBOutlet NSMenuItem *waveformPropertiesMenuItem;
@property (nonatomic, retain) IBOutlet NSMenuItem *spectrogramPropertiesMenuItem;


- (IBAction)toggleProperties: (id) pId;
- (IBAction)openDocument: (id) pID;
- (IBAction)refreshWaveform: (id) pId;
- (IBAction)refreshSpectrogram: (id) pId;
- (IBAction)showFileInformation: (id) pId;

- (void) windowWillClose: (NSNotification *) notification;
- (BOOL) openSoundFile: (NSString *) filename;

@end
