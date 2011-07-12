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

@synthesize tempDirectory;

@synthesize sampleRate;
@synthesize frames;
@synthesize duration;

@synthesize refreshWaveformButton;
@synthesize refreshSpectrogramButton;

@synthesize waveformView;
@synthesize spectrogramView;

// Common
@synthesize timeAxis;
@synthesize timeStart;
@synthesize timeEnd;
@synthesize timeShiftToZero;
@synthesize lineWidth;
@synthesize fontSize;

// Waveform
@synthesize waveformQuality;
@synthesize waveformWidth;
@synthesize waveformHeight;
@synthesize waveformLineWidth;

// Spectrogram
@synthesize spectrogramWidth;
@synthesize spectrogramHeight;
@synthesize frequencyAxis;
@synthesize fftWindow;
@synthesize spectrogramFloor;

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

    [refreshWaveformButton setEnabled:NO];
    [waveformView setImage:[NSImage imageNamed:@"NSRefreshTemplate"]];

    
    struct WR_WFINFO wfinfo;
    
    switch ([timeAxis indexOfSelectedItem]) {
        case 0:
            wfinfo.timeaxis = milliseconds;
            break;
        case 1:
            wfinfo.timeaxis = seconds;
            break;
    }
    
    wfinfo.quality = [waveformQuality floatValue];
    
    wfinfo.start = [timeStart floatValue];
    wfinfo.end = [timeEnd floatValue];
    wfinfo.startatzero = ([timeShiftToZero state] == NSOnState);
        
    wfinfo.width = [waveformWidth floatValue];
    wfinfo.height = [waveformHeight floatValue];
    wfinfo.wflwidth = [waveformLineWidth floatValue];
    wfinfo.lwidth = [lineWidth floatValue];
    wfinfo.fontsize = [fontSize floatValue];
    
    SoundFile *sf = [[SoundFile alloc] init];
    
    if ([sf openFile:m_filename])
    {
        
        [sf makeWaveForm:tempDirectory info:&wfinfo];
        
        NSString *pdfFilename = [[NSString alloc] initWithFormat:@"%@/waveform.pdf", tempDirectory];
        NSString *dataFilename = [[NSString alloc] initWithFormat:@"%@/waveform.dat", tempDirectory];
        NSString *gleFilename = [[NSString alloc] initWithFormat:@"%@/waveform.gle", tempDirectory];
        
        NSImage *pdf = [[NSImage alloc] initWithContentsOfFile:pdfFilename];
        [waveformView setImage:pdf];
        [pdf release];
        
        // Clean up
        NSError *error;
        [[NSFileManager defaultManager] removeItemAtPath:dataFilename error:&error];
        [[NSFileManager defaultManager] removeItemAtPath:gleFilename error:&error];
        [[NSFileManager defaultManager] removeItemAtPath:pdfFilename error:&error];
    }
    [refreshWaveformButton setEnabled:YES];
    
}


- (IBAction)refreshSpectrogram: (id) pId {
    
    
    [refreshSpectrogramButton setEnabled:NO];
    [spectrogramView setImage:[NSImage imageNamed:@"NSRefreshTemplate"]];
    
    
    struct WR_SPECINFO specinfo;
    specinfo.quality = 1;
    
    switch ([timeAxis indexOfSelectedItem]) {
        case 0:
            specinfo.timeaxis = milliseconds;
            break;
        case 1:
            specinfo.timeaxis = seconds;
            break;
    }
    
    switch ([frequencyAxis indexOfSelectedItem]) {
        case 0:
            specinfo.freqaxis = khz;
            break;
        case 1:
            specinfo.timeaxis = hz;
            break;
    }    
    
    switch ([fftWindow indexOfSelectedItem]) {
        case 0:
            specinfo.window = None;
            break;
        case 1:
            specinfo.window = Flattop;
            break;
        
        case 2:
            specinfo.window = Hamming;
            break;
        
        case 3:
            specinfo.window = Hann;
            break;
        
        case 4:
            specinfo.window = Cosine;
            break;
        
        case 5:
            specinfo.window = Blackman;
            break;
    }     
    
    specinfo.nwindow = 128;
    specinfo.increment = specinfo.nwindow * 0.5;
    
    specinfo.quality = 1;
    
    specinfo.start = [timeStart floatValue];
    specinfo.end = [timeEnd floatValue];
    specinfo.startatzero = ([timeShiftToZero state] == NSOnState);
    
    specinfo.width = [waveformWidth floatValue];
    specinfo.height = [waveformHeight floatValue];
    specinfo.lwidth = [lineWidth floatValue];
    specinfo.fontsize = [fontSize floatValue];
    specinfo.floor = [spectrogramFloor floatValue];
    
    SoundFile *sf = [[SoundFile alloc] init];
    
    if ([sf openFile:m_filename])
    {
        
        [sf makeSpectrogram:tempDirectory info:&specinfo];
        
        NSString *pdfFilename = [[NSString alloc] initWithFormat:@"%@/spectrogram.pdf", tempDirectory];
        NSString *dataFilename = [[NSString alloc] initWithFormat:@"%@/spectrogram.z", tempDirectory];
        NSString *gleFilename = [[NSString alloc] initWithFormat:@"%@/spectrogram.gle", tempDirectory];
        
        NSImage *pdf = [[NSImage alloc] initWithContentsOfFile:pdfFilename];
        [spectrogramView setImage:pdf];
        [pdf release];
               
        // Clean up
        NSError *error;
        [[NSFileManager defaultManager] removeItemAtPath:dataFilename error:&error];
        [[NSFileManager defaultManager] removeItemAtPath:gleFilename error:&error];
        [[NSFileManager defaultManager] removeItemAtPath:pdfFilename error:&error];
    }
    [refreshSpectrogramButton setEnabled:YES];    
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
    
    [refreshSpectrogramButton setEnabled:NO];
    [refreshWaveformButton  setEnabled:NO];
    [waveformView setImage:nil];  
    
    SoundFile *sf = [[SoundFile alloc] init];

    if ([sf openFile:filename])
    {
        m_filename = [[NSString alloc] initWithString:filename];
        
        // Add to Open Recent menu
        [[NSDocumentController sharedDocumentController] noteNewRecentDocumentURL:[NSURL fileURLWithPath:filename]];
        [sampleRate setTitle:[NSString stringWithFormat:@"%i Hz", [sf sampleRate]]];
        [frames setTitle:[NSString stringWithFormat:@"%i", [sf frames]]];
        [duration setTitle:[NSString stringWithFormat:@"%1.2f ms", [sf duration]]];
        
        [timeStart setTitleWithMnemonic:@"0.0"];
        [timeEnd setTitleWithMnemonic:[NSString stringWithFormat:@"%1.2f", [sf duration]]];
        
        [[self window] setTitle:[NSString stringWithFormat:@"Sound Artist - %@", [[NSFileManager defaultManager] displayNameAtPath:filename]]];

        [[self commonPropertiesPanel] makeKeyAndOrderFront:self];
        [[self waveformPropertiesPanel] makeKeyAndOrderFront:self];
        [[self spectrogramPropertiesPanel] makeKeyAndOrderFront:self];

        [self showWindow:self];

        [self refreshWaveform:self];
        [self refreshSpectrogram:self];
        [sf release];
        
        [refreshSpectrogramButton setEnabled:YES];
        [refreshWaveformButton  setEnabled:YES];       
        
        return true;
    }
    [sf release];
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
