//
//  SoundFile.h
//  SoundArtist
//
//  Created by Brandon Aubie on 11-07-11.
//  Copyright 2011 McMaster University. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "sndfile.h"
#import "fftw3.h"

enum windows {
    None,
    Flattop,
    Cosine,
    Hann,
    Hamming,
    Blackman
};

struct WR_WFINFO {
    float quality;
    enum {
        milliseconds,
        seconds
    } timeaxis;
    bool ylabels;
    float lwidth;
    float wflwidth; 
    float fontsize;
    float width;
    float height;
    float start;
    float end;
    bool startatzero;
};

struct WR_SPECINFO {
    float quality;
    enum {
        milliseconds,
        seconds
    } timeaxis;
    enum {
        hz,
        khz
    } freqaxis;
    enum windows window;
    float lwidth;
    float fontsize;
    float width;
    float height;
    float start;
    float end;
    float floor;
    int nwindow;
    int increment;
    bool color;
    int palette;
    bool startatzero;
    bool scalebar;
};

@interface SoundFile : NSObject {
@private
    
    NSString* m_filename;
    SF_INFO m_info;
}

@property (readonly) int sampleRate;
@property (readonly) int frames;
@property (readonly) double duration;

- (void) runGLE: (NSString *) filename;
- (BOOL) openFile: (NSString *) filename;
- (void) makeWindow: (enum windows) type length:(int) N array: (float *) window;


- (BOOL) makeWaveForm: (NSString*) folder info:(struct WR_WFINFO*) wfinfo;
- (BOOL) makeWaveformData: (NSString*) dataFilename info:(struct WR_WFINFO*) wfinfo;
- (NSString*) makeWaveformPlot: (NSString*) gleFilename info:(struct WR_WFINFO*) wfinfo;

- (BOOL) makeSpectrogram: (NSString*) folder info:(struct WR_SPECINFO*) specinfo;
- (BOOL) makeSpectrogramData: (NSString*) dataFilename info:(struct WR_SPECINFO*) specinfo;
- (NSString*) makeSpectrogramPlot: (NSString*) gleFilename info:(struct WR_SPECINFO*) specinfo;

- (BOOL) makePower: (NSString*) folder info:(struct WR_SPECINFO*) specinfo;
- (BOOL) makePowerData: (NSString*) dataFilename info:(struct WR_SPECINFO*) specinfo;
- (NSString*) makePowerPlot: (NSString*) gleFilename info:(struct WR_SPECINFO*) specinfo;

@end
