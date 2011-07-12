//
//  MainWindow.m
//  SoundArtist
//
//  Created by Brandon Aubie on 11-07-10.
//  Copyright 2011 McMaster University. All rights reserved.
//

#import "MainWindow.h"


@implementation MainWindow

@synthesize splashWindow;
@synthesize mainView;
@synthesize waitMessage;

@synthesize commonPropertiesPanel;
@synthesize waveformPropertiesPanel;
@synthesize spectrogramPropertiesPanel;
@synthesize commonPropertiesMenuItem;
@synthesize waveformPropertiesMenuItem;
@synthesize spectrogramPropertiesMenuItem;
@synthesize exportWaveform;
@synthesize exportSpectrogram;

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
@synthesize fftPoints;
@synthesize fftOverlap;
@synthesize spectrogramFloor;

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        imagesReloading = 0;
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

- (void) windowDidResize:(NSNotification *)notification {
    
    NSRect viewSize = [mainView bounds];
    
    [waveformView setFrame:NSMakeRect(0, viewSize.size.height*0.5+1, viewSize.size.width, viewSize.size.height*0.5)];
    [spectrogramView setFrame:NSMakeRect(0, 0, viewSize.size.width, viewSize.size.height*0.5)];
    
}

- (IBAction)refreshWaveform: (id) pId {

    [refreshWaveformButton setEnabled:NO];
    [waveformView setImage:[NSImage imageNamed:@"NSRefreshTemplate"]];
    imagesReloading++;
    [waitMessage setHidden:NO];
    
    struct WR_WFINFO wfinfo;
    
    switch ([timeAxis indexOfSelectedItem]) {
        case 0:
            wfinfo.timeaxis = seconds;
            break;
        case 1:
            wfinfo.timeaxis = milliseconds;
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
        
        if (pId == exportWaveform) {
            
            NSSavePanel *save = [NSSavePanel savePanel];
            [save setCanCreateDirectories:YES];
            [save setTitle:@"Export Waveform"];
            [save setMessage:@"Select a folder to export the waveform GLE and PDF files to."];
            [save setPrompt:@"Export Waveform"];
            
            if ( [save runModal] == NSOKButton ) {
                NSError *error;
                [[NSFileManager defaultManager] copyItemAtPath:tempDirectory toPath:[save filename] error:&error];
            }                    
        }
        
        
        // Clean up
        NSError *error;
        [[NSFileManager defaultManager] removeItemAtPath:dataFilename error:&error];
        [[NSFileManager defaultManager] removeItemAtPath:gleFilename error:&error];
        [[NSFileManager defaultManager] removeItemAtPath:pdfFilename error:&error];
    }
    [refreshWaveformButton setEnabled:YES];

    imagesReloading--;
    if (imagesReloading == 0) {
        [waitMessage setHidden:YES];
    }
    
}


- (IBAction)refreshSpectrogram: (id) pId {
    
    
    [refreshSpectrogramButton setEnabled:NO];
    [spectrogramView setImage:[NSImage imageNamed:@"NSRefreshTemplate"]];
    imagesReloading++;
    [waitMessage setHidden:NO];
    
    struct WR_SPECINFO specinfo;
    specinfo.quality = 1;
    
    switch ([timeAxis indexOfSelectedItem]) {
        case 0:
            specinfo.timeaxis = seconds;
            break;
        case 1:
            specinfo.timeaxis = milliseconds;
            break;
    }
    
    switch ([frequencyAxis indexOfSelectedItem]) {
        case 0:
            specinfo.freqaxis = khz;
            break;
        case 1:
            specinfo.freqaxis = hz;
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
        
    specinfo.nwindow = [[[fftPoints selectedItem] title] intValue];
    float overlap = 0.01*[fftOverlap floatValue];
    if (overlap >= 1) overlap = 0.9999;
    if (overlap < 0) overlap = 0;
    specinfo.increment = specinfo.nwindow * (1-overlap) > 1 ? specinfo.nwindow * (1-overlap) : 1;
    specinfo.quality = 1;
    
    specinfo.start = [timeStart floatValue];
    specinfo.end = [timeEnd floatValue];
    specinfo.startatzero = ([timeShiftToZero state] == NSOnState);
    
    specinfo.width = [spectrogramWidth floatValue];
    specinfo.height = [spectrogramHeight floatValue];
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
               
        if (pId == exportSpectrogram) {
            
            NSSavePanel *save = [NSSavePanel savePanel];
            [save setCanCreateDirectories:YES];
            [save setTitle:@"Export Spectrogram"];
            [save setMessage:@"Select a folder to export the spectrogram GLE and PDF files to."];
            [save setPrompt:@"Export Spectrogram"];
            
            if ( [save runModal] == NSOKButton ) {
                NSError *error;
                [[NSFileManager defaultManager] copyItemAtPath:tempDirectory toPath:[save filename] error:&error];
            }                    
        }
        
        
        // Clean up
        NSError *error;
        [[NSFileManager defaultManager] removeItemAtPath:dataFilename error:&error];
        [[NSFileManager defaultManager] removeItemAtPath:gleFilename error:&error];
        [[NSFileManager defaultManager] removeItemAtPath:pdfFilename error:&error];
    }
    [refreshSpectrogramButton setEnabled:YES];    
    imagesReloading--;
    if (imagesReloading == 0) {
        [waitMessage setHidden:YES];
    }
}

- (IBAction)openDocument: (id) pID {
    
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];    
    [openDlg setCanChooseFiles:YES];
    [openDlg setAllowsMultipleSelection:NO];
    [openDlg setCanChooseDirectories:NO];
    
    if ( [openDlg runModalForDirectory:nil file:nil] == NSOKButton )
    {
        [splashWindow close];
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
