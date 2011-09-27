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
    
    int imagesReloading;
    
    NSWindow* splashWindow;
        
    NSView* mainView;
    
    NSString* m_filename;
    
    NSString* tempDirectory;
    
    NSMenuItem *commonPropertiesMenuItem;
    NSMenuItem *waveformPropertiesMenuItem;
    NSMenuItem *spectrogramPropertiesMenuItem;
    
    NSMenuItem *exportWaveform;
    NSMenuItem *exportSpectrogram;
    NSMenuItem *exportPower;

    NSToolbarItem *refreshWaveformButton;
    NSToolbarItem *refreshSpectrogramButton;
    NSToolbarItem *refreshPowerButton;

    NSToolbarItem *optionsGeneral;
    NSToolbarItem *optionsWaveform;
    NSToolbarItem *optionsSpectrogram;
    NSToolbarItem *optionsPower;
    NSTabView *optionsTabView;

    NSTextFieldCell *sampleRate;
    NSTextFieldCell *frames;
    NSTextFieldCell *duration;
    
    NSImageView *waveformView;
    NSImageView *spectrogramView;
    NSImageView *powerView;

    NSPanel *optionsPanel;
    
    //Common
    NSPopUpButton *timeAxis;
    NSTextField *timeStart;
    NSTextField *timeEnd;
    NSButton *timeShiftToZero;
    NSTextField *lineWidth;
    NSTextField *fontSize;
    
    //Waveform
    NSSlider *waveformQuality;
    NSTextField *waveformWidth;
    NSTextField *waveformHeight;
    NSTextField *waveformLineWidth;
    
    //Spectrogram
    NSTextField *spectrogramWidth;
    NSTextField *spectrogramHeight;
    NSPopUpButton *frequencyAxis;
    NSPopUpButton *fftWindow;
    NSPopUpButton *fftPoints;    
    NSTextField *fftOverlap;    
    NSSlider *spectrogramFloor;    
    NSPopUpButton *spectrogramPalette;
}

@property (nonatomic, retain) IBOutlet NSWindow *splashWindow;
@property (nonatomic, retain) IBOutlet NSView *mainView;

@property (retain) NSString* tempDirectory;

@property (nonatomic, retain) IBOutlet NSPanel *optionsPanel;

@property (nonatomic, retain) IBOutlet NSMenuItem *commonPropertiesMenuItem;
@property (nonatomic, retain) IBOutlet NSMenuItem *waveformPropertiesMenuItem;
@property (nonatomic, retain) IBOutlet NSMenuItem *spectrogramPropertiesMenuItem;
@property (nonatomic, retain) IBOutlet NSMenuItem *exportWaveform;
@property (nonatomic, retain) IBOutlet NSMenuItem *exportSpectrogram;
@property (nonatomic, retain) IBOutlet NSMenuItem *exportPower;

@property (nonatomic, retain) IBOutlet NSToolbarItem *refreshWaveformButton;
@property (nonatomic, retain) IBOutlet NSToolbarItem *refreshSpectrogramButton;
@property (nonatomic, retain) IBOutlet NSToolbarItem *refreshPowerButton;

@property (nonatomic, retain) IBOutlet NSToolbarItem *optionsGeneral;
@property (nonatomic, retain) IBOutlet NSToolbarItem *optionsWaveform;
@property (nonatomic, retain) IBOutlet NSToolbarItem *optionsSpectrogram;
@property (nonatomic, retain) IBOutlet NSToolbarItem *optionsPower;


@property (nonatomic, retain) IBOutlet NSTextFieldCell *sampleRate;
@property (nonatomic, retain) IBOutlet NSTextFieldCell *frames;
@property (nonatomic, retain) IBOutlet NSTextFieldCell *duration;

@property (nonatomic, retain) IBOutlet NSImageView *waveformView;
@property (nonatomic, retain) IBOutlet NSImageView *spectrogramView;
@property (nonatomic, retain) IBOutlet NSImageView *powerView;

@property (nonatomic, retain) IBOutlet NSTabView *optionsTabView;

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
@property (nonatomic, retain) IBOutlet NSPopUpButton *fftPoints;
@property (nonatomic, retain) IBOutlet NSSlider *spectrogramFloor;
@property (nonatomic, retain) IBOutlet NSTextField *fftOverlap;
@property (nonatomic, retain) IBOutlet NSPopUpButton *spectrogramPalette;


- (IBAction)openDocument: (id) pID;
- (IBAction)refreshWaveform: (id) pId;
- (IBAction)refreshSpectrogram: (id) pId;
- (IBAction)refreshPower: (id) pId;

- (IBAction)changeOptionsTab: (id) pId;
- (BOOL) openSoundFile: (NSString *) filename;
- (void) windowDidResize:(NSNotification *)notification;

@end
