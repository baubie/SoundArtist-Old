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
    
    NSString* m_filename;

}

@property (retain) NSString* tempDirectory;

@property (nonatomic, retain) IBOutlet NSPanel *commonPropertiesPanel;
@property (nonatomic, retain) IBOutlet NSPanel *waveformPropertiesPanel;
@property (nonatomic, retain) IBOutlet NSPanel *spectrogramPropertiesPanel;

@property (nonatomic, retain) IBOutlet NSMenuItem *commonPropertiesMenuItem;
@property (nonatomic, retain) IBOutlet NSMenuItem *waveformPropertiesMenuItem;
@property (nonatomic, retain) IBOutlet NSMenuItem *spectrogramPropertiesMenuItem;

@property (nonatomic, retain) IBOutlet NSToolbarItem *refreshWaveformButton;
@property (nonatomic, retain) IBOutlet NSToolbarItem *refreshSpectrogramButton;

@property (nonatomic, retain) IBOutlet NSTextFieldCell *sampleRate;
@property (nonatomic, retain) IBOutlet NSTextFieldCell *frames;
@property (nonatomic, retain) IBOutlet NSTextFieldCell *duration;

@property (nonatomic, retain) IBOutlet NSImageView *waveformView;
@property (nonatomic, retain) IBOutlet NSImageView *spectrogramView;


//Common
@property (nonatomic, retain) IBOutlet NSPopUpButton *timeAxis;
@property (nonatomic, retain) IBOutlet NSTextField *timeStart;
@property (nonatomic, retain) IBOutlet NSTextField *timeEnd;
@property (nonatomic, retain) IBOutlet NSButton *timeShiftToZero;
@property (nonatomic, retain) IBOutlet NSTextField *lineWidth;
@property (nonatomic, retain) IBOutlet NSTextField *fontSize;

//Waveform
@property (nonatomic, retain) IBOutlet NSSlider *waveformQuality;
@property (nonatomic, retain) IBOutlet NSTextField *waveformWidth;
@property (nonatomic, retain) IBOutlet NSTextField *waveformHeight;
@property (nonatomic, retain) IBOutlet NSTextField *waveformLineWidth;

//Spectrogram
@property (nonatomic, retain) IBOutlet NSTextField *spectrogramWidth;
@property (nonatomic, retain) IBOutlet NSTextField *spectrogramHeight;
@property (nonatomic, retain) IBOutlet NSPopUpButton *frequencyAxis;
@property (nonatomic, retain) IBOutlet NSPopUpButton *fftWindow;
@property (nonatomic, retain) IBOutlet NSSlider *spectrogramFloor;



- (IBAction)toggleProperties: (id) pId;
- (IBAction)openDocument: (id) pID;
- (IBAction)refreshWaveform: (id) pId;
- (IBAction)refreshSpectrogram: (id) pId;

- (void) windowWillClose: (NSNotification *) notification;
- (BOOL) openSoundFile: (NSString *) filename;

@end
